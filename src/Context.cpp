
#include "Chunk.hpp"
#include "ChunkConcurrency.hpp"
#include "ChunkHelpers.hpp"
#include "CoordTypes.hpp"
#include "DebugFormat.hpp"
#include "DebugFormatSpecializations.hpp"

#include "Context.hpp"

#include "Profiler.hpp"
#include "Logger.hpp"
#include <algorithm>

using namespace glm;
void Simulation::setupContext() {
    program_epoch_ns = get_current_ns();
    LOG_EXPR(this);
    win.setupWindow(static_cast<void*>(this));
    input.setupInput(win.ptr);
    time.setupTimer(
        "frame",
        "input",
        "update",
        "enqueueGen",
        "drainGen",
        "enqueueMesh",
        "drainMesh",
        "draw",
        "render"
    );
    cam.setupCamera();
    rend.setupRenderer();
    ui.setupDebugUI(win.ptr);
    world.setupWorld();
}

void Simulation::freeCursor(){
    win.freeCursor();
    cam.disableMousePanning();
}
void Simulation::captureCursor(){
    win.captureCursor();
    cam.enableMousePanning();
}

void Simulation::loop(){
    time.start_frame();
    time.bench_start("frame");

    // INPUT
    time.bench_start("input");
    input.poll();
    handleInputs(); 
    time.bench_end("input");

    if (isPaused() == false){
        // UPDATE:
        time.bench_start("update");
        update();
        time.bench_end("update");

    }
    // DRAWING
    time.bench_start("draw");
    draw(); 
    time.bench_end("draw");
    ui.updateUI();
    ui.drawDebugUI(); 
    ui.render();

    time.bench_start("render");
    win.swapBuffers();
    time.bench_end("render");

    time.bench_end("frame");
    time.end_frame();
}

void Simulation::update() {
    time.bench_start("enqueueGen");
    enqueueGenerationJobs();
    time.bench_end("enqueueGen");

    time.bench_start("drainGen");
    drainAndUploadGenResults();
    time.bench_end("drainGen");

    time.bench_start("enqueueMesh");
    enqueueMeshingJobs();
    time.bench_end("enqueueMesh");

    time.bench_start("drainMesh");
    drainAndUploadMeshResults();
    time.bench_end("drainMesh");
}

std::vector<ivec3> Simulation::findChunksForGeneration(){
    std::vector<ivec3> candidates;
    const ivec3 chunkCoord = toWorldChunkCoord(cam.pos);
    // enumerate them based on their range to the player, such that nearest chunks come first.
    auto func = [this, &candidates](i32 x, i32 y, i32 z){
        const ivec3 key = ivec3{x,y,z}; // dont you have to 
        const ChunkEntry* entry = world.chunkMap.make_or_getEntry(key);
        if (entry->status.qualifiesForGeneration()){
            candidates.emplace_back(key);
        }
    };

    const i32& dist = SIMULATION_DIST;
    const i32& oy = chunkCoord.y;

    i32 minY = oy-dist;
    i32 maxY = oy+dist;
    // this genuinely took like 2 hours to make, holy christ man i hate geometric shit
    // Anyways this iterates in 'expanding spirals', per horizontal slice, such that we get 
    // chunks for meshing in a reasonable-ish order, (close->far, +Y first)
    for (i32 y = maxY; y>=minY; y--){
        i32 x{chunkCoord.x}, z{chunkCoord.z};
        func(x,y,z); // center point
        for (int r = 1; r<= dist; r++){
            const int r2 = 2*r;
            func(--x,y,z); // move out of the centre point
            for (int j = 0; j<r2 -1;j++)    func(x,y,++z); // traverse the remaining (-X) edge
            for (int j = 0; j<r2 ; j++)     func(++x,y,z); // traverse the whole     (Z+) edge
            for (int j = 0; j<r2 ; j++)     func(x,y,--z); // traverse the whole     (+X) edge
            for (int j = 0; j<r2 ; j++)     func(--x,y,z); // traverse the whole     (+X) edge
        }
    }
    return candidates;
}

std::size_t Simulation::enqueueGenerationJobs(){
    // BUG: we are enqueing the duplicate jobs.
    const auto candidates = findChunksForGeneration();
//    LOG_DEBUG("Chunks in range found:{}",chunkWorldPosForGen.size());
    std::size_t res = 0;
    for (const ivec3& chunkWorldPos: candidates){

        const auto& entry = world.chunkMap.get_entry(chunkWorldPos);
        if (entry->status.qualifiesForGeneration() == false){
            continue; 
        }
        bool success = world.chunkMap.generator.genJobQueue.try_emplace(
            chunkWorldPos, 
            WORLD_SEED,
            world.genConfig
        );
        if (success){
            res++;
            entry->status.beginGeneration();
        }
    }
    return res;
}

std::vector<MeshJob> Simulation::findMeshJobs(){

    auto in_frustum = [](const Frustum& frustum, const World& world) {
        return [frustum, &world](const WorldChunkCoord& chunk_coord) {
            return frustum.isAABBInside(world.chunkMap.getBoundingBox(chunk_coord));
        };
    };

    
    const auto camChunkCoord = toWorldChunkCoord(cam.pos);
    const auto candidateList = world.generatedChunkCoordsInRadius(camChunkCoord, RENDER_DIST, maxMeshesPerFrame);

    std::vector<MeshJob> res;
    for (const auto candidateCoord: candidateList){
        // TODO: to 4-5x reduce the size of a mesh jobs allocation, 
        // i can reduce the surrounding Chunks block storage to only contain the boundary blocks,
        // i.e the ones bordering the actual chunk in question.
        auto entry = world.chunkMap.get_entry(candidateCoord);
        // make this construct based on the entry by const reference or something
        res.emplace_back(
            candidateCoord,
            &rend.atlas,
            entry
        );

    }
    return res;
}

std::size_t Simulation::enqueueMeshingJobs(){
    auto jobsForMeshing = findMeshJobs();

    std::size_t res = 0;
    for (auto& job: jobsForMeshing){
        const auto& entry = world.chunkMap.get_entry(job.chunkCoord);
        auto& q = rend.mesher.meshJobQueue;

        const bool queueIsFull = !q.try_enqueue(std::move(job));
        if (queueIsFull){
            break;
        } else{
            entry->status.beginMeshing();
            res++;
        }

        //NOTE:
        // its possible that using an emplace method could be faster,
        // since we avoid constructing the object in case of lock contention.
        // Either way, we have to make the chunk entry lookup, so im unsure of the benefit here.
        // Cant be that bad, since they released unordered_map::emplace() which emplaces regardless
        // of insertion, and it took them until c++17 to fix that with try_emplace.
    }
    LOG_DEBUG("enqueued_for_meshing this frame:{}/{}",res,jobsForMeshing.size());
    return res;
}


std::vector<GenResult> drainGenResults(Queue<GenResult>& queue){
    constexpr std::size_t MaxGenUploadsPerFrame = 12;
    std::vector<GenResult> output; output.reserve(MaxGenUploadsPerFrame);

    for (std::size_t mesh_count = 0; mesh_count < MaxGenUploadsPerFrame; mesh_count++){
        std::optional<GenResult> result = queue.try_dequeue();
        bool queueIsEmpty = !result.has_value();
        if (queueIsEmpty){
            break; // give up this frame?
        }
        output.emplace_back(*result);
    }
    return output;
}

std::vector<MeshResult> drainMeshResultQueue(Queue<MeshResult>& queue){
    constexpr std::size_t MaxMeshUploadsPerFrame = 128;
    std::vector<MeshResult> output;
    output.reserve(MaxMeshUploadsPerFrame);
    constexpr i32 meshDequeueAttempts = 8;
    for (std::size_t mesh_count = 0; mesh_count < MaxMeshUploadsPerFrame; mesh_count++){
        for (i32 i =0; i<meshDequeueAttempts; i++){
            std::optional<MeshResult> meshData = queue.try_dequeue();
            if (!meshData){
                continue;
            }
            output.emplace_back(std::move(*meshData));
        }
    }

    return output;
}
std::size_t Simulation::drainAndUploadMeshResults(){
    std::size_t res = 0;
    auto candidateMeshes = drainMeshResultQueue(rend.mesher.meshResultQueue);
    for (const auto& [candidateGen, chunkCoord, vertices, indices] : candidateMeshes){
        if (vertices.size()>0){
            rend.uploadMesh(chunkCoord, vertices, indices);
            res++;
            this->chunksMeshed++;
        } 
        world.chunkMap.get_entry(chunkCoord)->status.endMeshing();
    }
    return res;
}

std::size_t Simulation::drainAndUploadGenResults(){
    auto drainedMeshes = drainGenResults(world.chunkMap.generator.genResultQueue);
    auto res = drainedMeshes.size();
    for (const auto& newGen : drainedMeshes){
        world.chunkMap.uploadGeneratedChunk(newGen);
    }
    return res;
}

void Simulation::pause(){
    freeCursor();
    paused=true;
}
void Simulation::unpause(){
    captureCursor();
    paused=false;
}
bool Simulation::isPaused(){
    return paused;
}
void Simulation::togglePause(){
    if (paused) unpause();
    else pause();
}
void Simulation::draw() {
    rend.clear({ 0.25, 0.5, 0.85, 1.0 });
    ScopeTimer t_mesh_chunks{ "Chunk meshing", "chunk" };

    rend.draw(cam);
    static bool first_draw = true;
    if (first_draw) {
        LOG_DEBUG("Finished first draw");
        first_draw = false;
    }
}

void Simulation::handleInputs() {
    auto signal = 
    input.mapToggleKey(KEY_ESCAPE, [this]{
        if (isPaused()){
            unpause();
            return InputSignal::CONTINUE;
        } else{
            win.scheduleClose();
            return InputSignal::RETURN;
        }
    });
    if (signal == InputSignal::RETURN){ return; }

    input.updateCooldowns(time.dt_s);
    const f32 scaled_dt = time.dt_s * 60;

    input.mapToggleKey(KEY_P, [this]{
        togglePause();
    });

    if (isPaused()) return;
    // WARNING: Anything below here is ignored during paused frames

    if (input.mousepos != input.prevmousepos) {
        const vec2 diff = input.prevmousepos - input.mousepos;
        cam.rotateByMouse(diff, scaled_dt);
    }

    input.mapToggleKey(KEY_T, [this]{
        rend.debug.wireframe = !rend.debug.wireframe;
    });
    input.mapToggleKey(KEY_R,[this]{
        for (const auto& [worldCoord, entry]: world.chunkMap.entries){
            entry->requestMeshRegen();
        }
        rend.visibleChunkMeshes.clear();
    });

    static_assert(KEY_MAX>=KEY_LEFT_SHIFT && KEY_LEFT_SHIFT>KEY_MIN);
    input.mapHeldKey(KEY_LEFT_SHIFT,[this](bool isHeld){
        if (isHeld){
            cam.moveSpeed = Camera::SPRINT_MOVESPEED;
        }else{
            cam.moveSpeed = Camera::BASE_MOVESPEED;
        }
    });
    input.mapHeldKey(KEY_W,[this]{
		cam.move(Direction::FORWARD, time.dt_s);
	});
    input.mapHeldKey(KEY_S,[this]{
		cam.move(Direction::BACKWARD, time.dt_s);
	});
    input.mapHeldKey(KEY_A,[this]{
		cam.move(Direction::LEFT, time.dt_s);
	});
    input.mapHeldKey(KEY_D,[this]{
		cam.move(Direction::RIGHT, time.dt_s);
	});
    input.mapHeldKey(KEY_SPACE,[this]{
		cam.move(Direction::UP, time.dt_s);
	});
    input.mapHeldKey(KEY_E,[this]{
		cam.move(Direction::UP, time.dt_s);
	});
    input.mapHeldKey(KEY_Q,[this]{
		cam.move(Direction::DOWN, time.dt_s);
	});

    input.mapHeldKey(KEY_LEFT,[this]{
		cam.rotate(Direction::LEFT, time.dt_s);
	});
    input.mapHeldKey(KEY_RIGHT,[this]{
		cam.rotate(Direction::RIGHT, time.dt_s);
	});
    input.mapHeldKey(KEY_UP,[this]{
		cam.rotate(Direction::UP, time.dt_s);
	});
    input.mapHeldKey(KEY_DOWN,[this]{
		cam.rotate(Direction::DOWN, time.dt_s);
	});
    input.prevmousepos = input.mousepos;
}
