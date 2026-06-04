
#include "Chunk.hpp"
#include "ChunkConcurrency.hpp"
#include "ChunkHelpers.hpp"
#include "CoordTypes.hpp"
#include "DebugFormat.hpp"
#include "DebugFormatSpecializations.hpp"
#include "CoordIteration.hpp"


#include "Simulation.hpp"

#include "LM.hpp"
#include "Line3D.hpp"
#include "Profiler.hpp"
#include "Logger.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include <algorithm>
#include <print>

using namespace glm;
void Simulation::setupSimulation() {
    program_epoch_ns = get_current_ns();
    playerCam.isPlayer=true;
    playerCam.lineColor = Color01::WHITE;
    playerCam.far_clip_z = 1000.0f;

    droneCam.vertical_fov=50.0f;

    win.set_callbacks(static_cast<void*>(this));
    LOG_DEBUG("Finished setting window callbacks.");

    input.set_callbacks(win.ptr);
    LOG_DEBUG("Finished Input setup.");

    profiler.init(
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
    LOG_DEBUG("Finished Profiler setup.");

    playerCam.set_pos_ori({0, 168, 0}, -23.4, 56.3);
    droneCam.set_pos_ori({0, 300, 0}, -89.0, 0.0);
    LOG_DEBUG("Finished Camera setup.");


    ui.init(win.ptr);
    LOG_DEBUG("Finished UI setup.");

    world.setup();
    LOG_DEBUG("Finished World setup.");
}

void Simulation::freeCursor(){
    win.freeCursor();
    playerCam.disableMousePanning();
}

void Simulation::unGenerateAllChunks(){
    for (auto& [worldCoord, entry]: world.chunkMap.entries){
        entry->requestWorldRegen();
    }
}
void Simulation::unMeshAllChunks(){
    for (const auto& [worldCoord, entry]: world.chunkMap.entries){
        if (entry->status.isCleanMeshed()){
            entry->makeDirty();
        }
    }
    rend.opaqueChunkMeshes.clear();
    rend.transparentChunkMeshes.clear();
}

void Simulation::captureCursor(){
    win.captureCursor();
    playerCam.enableMousePanning();
}

void Simulation::loop(){
    profiler.start_frame();
    profiler.bench_start("frame");

    // INPUT
    profiler.bench_start("input");
    input.poll();
    handleInputs(); 
    profiler.bench_end("input");

    if (isPaused() == false){
        // UPDATE:
        profiler.bench_start("update");
        update();
        profiler.bench_end("update");

    }
    // DRAWING
    profiler.bench_start("draw");
    draw(); 
    profiler.bench_end("draw");
    ui.update();
    if (rend.debug.showDebugUI){
        ui.draw(); 
    }

    profiler.bench_start("render");
    win.swapBuffers();
    profiler.bench_end("render");

    profiler.bench_end("frame");
    profiler.end_frame();
}
bool Simulation::inPlayerFrustum(WorldChunkCoord coord){
    return playerCam.getFrustum().isAABBInside(world.chunkMap.getBoundingBox(coord));
}

void Simulation::markInsideFrustum(){
    for (const auto& [coord, entry]: world.chunkMap.entries){
        if (inPlayerFrustum(coord)){
            entry->status.markInFrustum();
        }else{
            entry->status.markOutsideFrustum();
        }
    }
}
void Simulation::cullMeshes(bool enableFrustumCulling){
    // if a mesh is outside of render dist, but still being meshed, cull it.
    auto lo = toWorldChunkCoord(playerCam.pos) + ChunkOffset{-MESH_CULL_DIST};
    auto hi = toWorldChunkCoord(playerCam.pos) + ChunkOffset{MESH_CULL_DIST};

    // this shit is smelly
    auto pred = [this](auto* ptr, auto& chunkMap, auto lo, auto hi){
        return [lo,hi, &chunkMap, ptr, this](IndexedMesh& mesh){
            auto* entry = world.chunkMap.get_entry_safe(mesh.chunkCoord);
            bool outsidePaddedRenderDist = !LM::isVecInBounds(mesh.chunkCoord, lo,hi);
            bool outsideFrustum = !inPlayerFrustum(mesh.chunkCoord);
            // should a mesh be 'unloaded' instead of removed from the list?
            // i.e its not uploaded to the gpu but can be 'reloaded'?
            if (outsideFrustum){
                mesh.unload();
            }else{
                mesh.load();
            }
            return outsidePaddedRenderDist;
        };
    };
    // erase all elements which are out of bounds, no?
    auto n_erased_op = std::erase_if(rend.opaqueChunkMeshes, pred(this, world.chunkMap,lo,hi));
    auto n_erased= std::erase_if(rend.transparentChunkMeshes, pred(this, world.chunkMap,lo,hi));

}

void Simulation::update() {
    genJobsThisFrame = enqueueGenerationJobs(maxGenJobsPerFrame);
    rb_genJobsAdded.write(genJobsThisFrame);

    genResultsThisFrame = drainAndUploadGenResults(maxGenUploadsPerFrame);
    rb_genResultsAdded.write(genResultsThisFrame);

    markInsideFrustum();
    cullMeshes(true);
    meshJobsThisFrame = enqueueMeshingJobs(maxMeshJobsPerFrame);
    rb_meshJobsAdded.write(meshJobsThisFrame);

    meshResultsThisFrame = drainAndUploadMeshResults(maxMeshUploadsPerFrame);
    rb_meshResultsAdded.write(meshResultsThisFrame);


    droneCam.pos = WorldFloatPos{playerCam.pos.raw()+glm::vec3{0,100,0}};
    droneCam.cached_viewMatrix.invalidate();
    droneCam.cached_frustum.invalidate();

    lines3d.clear();
    auto makeCamFrustumLines = [this](Camera& cam){
        auto frustum = cam.getFrustum();
        frustum.path.publish(lines3d);
        lines3d.append_range(frustum.extra_lines);
    };
    makeCamFrustumLines(playerCam);
//    makeCamFrustumLines(droneCam);

}


std::vector<WorldChunkCoord> Simulation::findChunksForGeneration(std::size_t maxJobs){
    std::vector<WorldChunkCoord> candidates;
    const auto chunkCoord = toWorldChunkCoord(playerCam.pos);
    // enumerate them based on their range to the player, such that nearest chunks come first.
    auto add = [this, &candidates](i32 x, i32 y, i32 z) ->bool {
        const auto key = WorldChunkCoord{x,y,z}; // dont you have to 
        const ChunkEntry* entry = world.chunkMap.make_or_getEntry(key);
        if (entry->status.qualifiesForGeneration()){
            candidates.emplace_back(key);
            return true;
        }else{
            return false;
        }
    };

    const i32& dist = SIMULATION_DIST;
    const i32& oy = chunkCoord.y;

    i32 minY = oy-dist;
    i32 maxY = oy+dist;
    // this genuinely took like 2 hours to make, holy christ man i hate geometric shit
    // Anyways this iterates in 'expanding spirals', per horizontal slice, such that we get 
    // chunks for meshing in a reasonable-ish order, (close->far, +Y first)
    SpiralIterateRange(chunkCoord,SIMULATION_DIST*2, SIMULATION_DIST,add);
    return candidates;
}

std::size_t Simulation::enqueueGenerationJobs(std::size_t maxJobs){
    profiler.bench_start("enqueueGen");
    const auto candidates = findChunksForGeneration(maxJobs);
      LOG_DEBUG("Chunks in range found:{}",candidates.size());
    std::size_t count = 0;
    auto& genQ = world.chunkMap.generator.genJobQueue;
    for (const auto& chunkWorldPos: candidates){
        const auto& entry = world.chunkMap.get_entry(chunkWorldPos);
        if (entry->status.qualifiesForGeneration()){
            bool success = genQ.try_emplace(
                chunkWorldPos, 
                WORLD_SEED,
                world.genConfig
            );
            if (success){
                count++;
                entry->status.beginGeneration();
            }
        }
    }
    profiler.bench_end("enqueueGen");
    return count;
}

std::vector<MeshJob> Simulation::findMeshJobs(std::size_t maxJobs){


    
    const auto camChunkCoord = toWorldChunkCoord(playerCam.pos);
    const auto candidateList = world.meshReadyChunksInRad(camChunkCoord, RENDER_EXTENTS, maxJobs);

    std::vector<MeshJob> res;
    for (const auto candidateCoord: candidateList){
        auto entry = world.chunkMap.get_entry(candidateCoord);
        if(inPlayerFrustum(candidateCoord)){
            // TODO: to 4-5x reduce the size of a mesh jobs allocation, 
            // i can reduce the surrounding Chunks block storage to only contain the boundary blocks,
            // i.e the ones bordering the actual chunk in question.
            // make this construct based on the entry by const reference or something
        }else{
            res.emplace_back(
                candidateCoord,
                &rend.atlas,
                entry
            );
        }

    }
    return res;
}

std::size_t Simulation::enqueueMeshingJobs(std::size_t maxJobs){
    profiler.bench_start("enqueueMesh");
    auto jobsForMeshing = findMeshJobs(maxJobs);

    std::size_t count = 0;
    for (std::size_t attempts = 0; attempts<maxJobs; attempts++){
        auto& q = rend.meshers.meshJobQueue;
        count = q.try_batch_enqueue(jobsForMeshing);
        if (count >= 0) break; // we have successfully enqueued the batch
    }

    for (std::size_t i =0; i < count; i++){
        const auto& entry = world.chunkMap.get_entry(jobsForMeshing[i].chunkCoord);
        entry->status.beginMeshing();
    }
    profiler.bench_end("enqueueMesh");
    return count;
}


std::vector<GenResult> drainGenResults(Queue<GenResult>& queue, std::size_t maxUploads){
    std::vector<GenResult> output; output.reserve(maxUploads);

    for (std::size_t mesh_count = 0; mesh_count < maxUploads; mesh_count++){
        std::optional<GenResult> result = queue.try_dequeue();
        if (result.has_value()){
            output.emplace_back(*result);
        } else{
            break; // give up this frame?
        }
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
    profiler.bench_start("drainMesh");
    std::size_t count = 0;
    auto candidateMeshes = drainMeshResultQueue(rend.meshers.meshResultQueue,maxUploads);
    for (const auto& [candidateGen, chunkCoord, opaque, transparent] : candidateMeshes){
        if (opaque.vertices.size()>0){
            rend.uploadMesh(chunkCoord, std::move(opaque));
        } 
        if (transparent.vertices.size()>0){
            rend.uploadMesh(chunkCoord, std::move(transparent));
        } 

        if (world.chunkMap.has_entry(chunkCoord)){
            world.chunkMap.get_entry(chunkCoord)->status.endMeshing();
            count++;
            this->chunksMeshed++;
        } else {
            LOG_WARN("Homeless mesh not uploaded because its entry was deleted whilst it was meshed.");
        }
    }
    profiler.bench_end("drainMesh");
    return count;
}

std::size_t Simulation::drainAndUploadGenResults(std::size_t maxUploads){
    profiler.bench_start("drainGen");
    auto drainedMeshes = drainGenResults(world.chunkMap.generator.genResultQueue,maxUploads);
    auto count = drainedMeshes.size();
    for (const auto& newGen : drainedMeshes){
        world.chunkMap.uploadGeneratedChunk(newGen);
    }
    profiler.bench_end("drainGen");
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
RenderTargetView Simulation::screenView() {
    return {
        .pos = {win.x,win.y},
        .size = {win.px_w, win.px_h},
    };
}
RenderTargetView Simulation::secondaryView() {
    return fixedCamTarget.view();
}
void Simulation::draw() {
    chunkOutlines.clear();

    for(const auto& [key,entry] : this->world.chunkMap.entries){
        if (entry->status.inFrustum() && !entry->block_data.isAllAir()){
            if (DebugChunkRenderer::HIDE_CLEAN_CHUNKS && entry->status.isCleanMeshed()){
                continue; 
            }
            if (DebugChunkRenderer::HIDE_AIR_CHUNKS && entry->block_data.isAllAir()){
                continue; 
            }
            chunkOutlines.append_range(entry->bounding_box.getLines(entry->status.DebugOutlineColor()));
        }
    }

    ScopeTimer t_mesh_chunks{ "Chunk meshing", "chunk" };
    playerCam.aspectRatio = win.aspect();
    rend.debug.reset_per_frame();
    rend.clear_to(screenView());
    rend.clear_to(secondaryView());

    rend.draw_to(playerCam, screenView());
    rend.draw_to(droneCam, secondaryView());

    rend.draw_3DLines_to(playerCam,lines3d,screenView());
    rend.draw_3DLines_to(droneCam,lines3d,secondaryView());
    
    if(rend.debug.showChunkBoundaries){
        rend.draw_debugChunks(playerCam, playerCam,world,screenView());
        rend.draw_debugChunks(playerCam,droneCam,world,secondaryView());
        rend.draw_3DLines_to(playerCam,chunkOutlines,screenView());
        rend.draw_3DLines_to(droneCam,chunkOutlines,secondaryView());
    }



    static bool first_draw = true;
    if (first_draw) {
        LOG_DEBUG("Finished first draw");
        first_draw = false;
    }
}

