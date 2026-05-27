
#include "Chunk.hpp"
#include "ChunkConcurrency.hpp"
#include "ChunkHelpers.hpp"
#include "CoordTypes.hpp"
#include "DebugFormat.hpp"
#include "DebugFormatSpecializations.hpp"

#include "Context.hpp"

#include "Profiler.hpp"
#include "Logger.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include <algorithm>

using namespace glm;
glm::mat4 h;
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
    rend.setup();
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
    genJobsThisFrame = enqueueGenerationJobs(maxGenJobsPerFrame);
    time.bench_end("enqueueGen");
    rb_genJobsAdded.write(genJobsThisFrame);

    time.bench_start("drainGen");
    genResultsThisFrame = drainAndUploadGenResults(maxGenUploadsPerFrame);
    time.bench_end("drainGen");
    rb_genResultsAdded.write(genResultsThisFrame);

    time.bench_start("enqueueMesh");
    meshJobsThisFrame = enqueueMeshingJobs(maxMeshJobsPerFrame);
    time.bench_end("enqueueMesh");
    rb_meshJobsAdded.write(meshJobsThisFrame);

    time.bench_start("drainMesh");
    meshResultsThisFrame = drainAndUploadMeshResults(maxMeshUploadsPerFrame);
    time.bench_end("drainMesh");
    rb_meshResultsAdded.write(meshResultsThisFrame);
}

enum struct IterationSignal{
    CONTINUE,
    BREAK,
};
std::vector<WorldChunkCoord> Simulation::findChunksForGeneration(std::size_t maxJobs){
    std::vector<WorldChunkCoord> candidates;
    const auto chunkCoord = toWorldChunkCoord(cam.pos);
    // enumerate them based on their range to the player, such that nearest chunks come first.
    auto func = [this, &candidates, &maxJobs](i32 x, i32 y, i32 z) -> IterationSignal{
        const auto key = WorldChunkCoord{x,y,z}; // dont you have to 
        const ChunkEntry* entry = world.chunkMap.make_or_getEntry(key);
        if (entry->status.qualifiesForGeneration()){
            candidates.emplace_back(key);
        }
        if (candidates.size()>=maxJobs){
            return IterationSignal::BREAK;
        }
        return IterationSignal::CONTINUE;
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
        // center point
        if(func(x,y,z)==IterationSignal::BREAK){
            goto EXIT_LOOP;
        }
        for (int r = 1; r<= dist; r++){
            const int r2 = 2*r;
            // move out of the centre point
            if(func(--x,y,z)==IterationSignal::BREAK){
                goto EXIT_LOOP;
            }
            for (int j = 0; j<r2 -1;j++)    {
                // traverse the remaining (-X) edge
                if(func(x,y,++z)==IterationSignal::BREAK){
                    goto EXIT_LOOP;
                }
            }
            for (int j = 0; j<r2 ; j++){
                // traverse the whole     (Z+) edge
                if(func(++x,y,z)==IterationSignal::BREAK){ 
                    goto EXIT_LOOP;
                }
            }
            for (int j = 0; j<r2 ; j++){
                if(func(x,y,--z)==IterationSignal::BREAK){ // traverse the whole     (+X) edge
                    goto EXIT_LOOP;
                }
            }
            for (int j = 0; j<r2 ; j++){
                if(func(--x,y,z)==IterationSignal::BREAK){ // traverse the whole     (+X) edge
                    goto EXIT_LOOP;
                }
            }
        }
    }
EXIT_LOOP:
    return candidates;
}

std::size_t Simulation::enqueueGenerationJobs(std::size_t maxJobs){
    // BUG: we are enqueing the duplicate jobs.
    const auto candidates = findChunksForGeneration(maxJobs);
//    LOG_DEBUG("Chunks in range found:{}",chunkWorldPosForGen.size());
    std::size_t count = 0;
    for (const auto& chunkWorldPos: candidates){

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
            count++;
            entry->status.beginGeneration();
        }
    }
    return count;
}

std::vector<MeshJob> Simulation::findMeshJobs(std::size_t maxJobs){

    auto in_frustum = [](const Frustum& frustum, const World& world) {
        return [frustum, &world](const WorldChunkCoord& chunk_coord) {
            return frustum.isAABBInside(world.chunkMap.getBoundingBox(chunk_coord));
        };
    };

    
    const auto camChunkCoord = toWorldChunkCoord(cam.pos);
    const auto candidateList = world.meshReadyChunksInRad(camChunkCoord, RENDER_DIST, maxJobs);

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

std::size_t Simulation::enqueueMeshingJobs(std::size_t maxJobs){
    auto jobsForMeshing = findMeshJobs(maxJobs);

    std::size_t count = 0;
    for (std::size_t attempts = 0; attempts<maxJobs; attempts++){
        auto& q = rend.mesher.meshJobQueue;
        count = q.try_batch_enqueue(jobsForMeshing);
        if (count >= 0) break; // we have successfully enqueued the batch
    }

    for (std::size_t i =0; i < count; i++){
        const auto& entry = world.chunkMap.get_entry(jobsForMeshing[i].chunkCoord);
        entry->status.beginMeshing();
    }
    return count;
}


std::vector<GenResult> drainGenResults(Queue<GenResult>& queue, std::size_t maxUploads){
    std::vector<GenResult> output; output.reserve(maxUploads);

    for (std::size_t mesh_count = 0; mesh_count < maxUploads; mesh_count++){
        std::optional<GenResult> result = queue.try_dequeue();
        bool queueIsEmpty = !result.has_value();
        if (queueIsEmpty){
            break; // give up this frame?
        }
        output.emplace_back(*result);
    }
    return output;
}

std::vector<MeshResult> drainMeshResultQueue(Queue<MeshResult>& queue,std::size_t maxUploads){
    for (std::size_t mesh_dq_attempts = 0; mesh_dq_attempts < maxUploads; mesh_dq_attempts++){
        std::optional<std::vector<MeshResult>> res = queue.try_batch_dequeue(maxUploads);
        if (res){
            return *res;
        }
        if (!res){
            continue;
        } 
    }

    return {};
}
std::size_t Simulation::drainAndUploadMeshResults(std::size_t maxUploads){
    std::size_t count = 0;
    auto candidateMeshes = drainMeshResultQueue(rend.mesher.meshResultQueue,maxUploads);
    for (const auto& [candidateGen, chunkCoord, vertices, indices] : candidateMeshes){
        if (vertices.size()>0){
            rend.uploadMesh(chunkCoord, vertices, indices);
            count++;
            this->chunksMeshed++;
        } 
        world.chunkMap.get_entry(chunkCoord)->status.endMeshing();
    }
    return count;
}

std::size_t Simulation::drainAndUploadGenResults(std::size_t maxUploads){
    auto drainedMeshes = drainGenResults(world.chunkMap.generator.genResultQueue,maxUploads);
    auto count = drainedMeshes.size();
    for (const auto& newGen : drainedMeshes){
        world.chunkMap.uploadGeneratedChunk(newGen);
    }
    return count;
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
    rend.draw_debugChunks(cam,world);
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
