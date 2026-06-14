
#include "Chunk.hpp"
#include "ChunkConcurrency.hpp"
#include "ChunkEntry.hpp"
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
#include "Assertion.hpp"

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
    world.chunkMap.entries.clear();
    world.chunkMap.states.clear();
}

void Simulation::unMeshAllChunks(){
    for (auto& [worldCoord, state]: world.chunkMap.states){
        state.mesh = ChunkMeshState::NoMesh;
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
        update();

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


[[deprecated]]void Simulation::markInsideFrustum(){
}
void Simulation::cullMeshes(bool enableFrustumCulling){
    // if a mesh is outside of render dist, but still being meshed, cull it.
    auto lo = toWorldChunkCoord(playerCam.pos) + ChunkOffset{-MESH_CULL_DIST};
    auto hi = toWorldChunkCoord(playerCam.pos) + ChunkOffset{MESH_CULL_DIST};

    // this shit is smelly. Needs a rewrite
    auto pred = [this](auto* ptr, auto& chunkMap, auto lo, auto hi){
        return [lo,hi, &chunkMap, ptr, this](IndexedMesh& mesh){
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
    // TODO: Fix these 
//    auto n_erased_op = std::erase_if(rend.opaqueChunkMeshes, pred(this, world.chunkMap,lo,hi));
//    auto n_erased= std::erase_if(rend.transparentChunkMeshes, pred(this, world.chunkMap,lo,hi));

}

void Simulation::update() {
    profiler.bench_start("update");

    genJobsThisFrame = enqueueGenerationJobs(maxGenJobsPerFrame);
    rb_genJobsAdded.write(genJobsThisFrame);

    genResultsThisFrame = drainAndUploadGenResults(maxGenUploadsPerFrame);
    rb_genResultsAdded.write(genResultsThisFrame);

    //markInsideFrustum();
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
    profiler.bench_end("update");
}


std::vector<WorldChunkCoord> Simulation::findChunksForGeneration(std::size_t maxJobs){
    std::vector<WorldChunkCoord> candidates;
    const auto chunkCoord = toWorldChunkCoord(playerCam.pos);
    // enumerate them based on their range to the player, such that nearest chunks come first.

    const i32& dist = SIMULATION_DIST;
    const i32& oy = chunkCoord.y;

    i32 minY = oy-dist;
    i32 maxY = oy+dist;
    SpiralIterateRange(chunkCoord, SIMULATION_DIST*2, SIMULATION_DIST, [this, &candidates](i32 x, i32 y, i32 z) -> bool {
        // 1. if an state entry exists, check if it needs regeneration.
        const auto key = WorldChunkCoord{x,y,z};
        const auto state = world.chunkMap.try_get_state(key);
        if(state.has_value()){
            if ((*state)->needsRegeneration()){
                candidates.emplace_back(key);
                return true;
            }else {
                return false;
            }
        }

        // 2. if no state entry exists; then the chunk hasnt been generated => it qualifies.
        candidates.emplace_back(key);
        return true;

    });
    return candidates;
}

std::size_t Simulation::enqueueGenerationJobs(std::size_t maxJobs){
    profiler.bench_start("enqueueGen");
    const auto candidates = findChunksForGeneration(maxJobs);
    LOG_DEBUG("[GEN ENQUEUE]: Chunks in range found:{}",candidates.size());
    std::size_t count = 0;
    auto& genQ = world.chunkMap.generator.genJobQueue;
    for (const auto& chunkWorldPos: candidates){

        bool success = genQ.try_emplace(
            chunkWorldPos, 
            WORLD_SEED,
            world.genConfig
        );
        if (success){
            auto state = world.chunkMap.try_get_state(chunkWorldPos);
            if (state.has_value()){
                (*state)->logDirtyGenEnqueue();
            } else{
                bool inserted = world.chunkMap.make_state_entry(chunkWorldPos);
                assert(inserted);
            }
            count++;
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
        auto state = world.chunkMap.get_state(candidateCoord);
        // TODO: to 4-5x reduce the size of a mesh jobs allocation, 
        // i can reduce the surrounding Chunks block storage to only contain the boundary blocks,
        // i.e the ones bordering the actual chunk in question.
        // make this construct based on the entry by const reference or something
        auto current_revision = world.chunkMap.get_current_mesh_revision(candidateCoord);
        res.emplace_back(
            current_revision,
            candidateCoord,
            &rend.atlas,
            entry
        );
    }
    return res;
}

std::size_t Simulation::enqueueMeshingJobs(std::size_t maxJobs){
    profiler.bench_start("enqueueMesh");
    auto jobsForMeshing = findMeshJobs(maxJobs);

    std::size_t count = 0;
    for (std::size_t attempts = 0; attempts<maxMeshDequeueAttempts; attempts++){
        auto& q = rend.meshers.meshJobQueue;
        count = q.try_batch_enqueue(jobsForMeshing);
        if (count >= 0) break; // we have successfully enqueued the batch
    }

    for (std::size_t i =0; i < count; i++){
        const auto entry = world.chunkMap.get_entry(jobsForMeshing[i].chunkCoord);
        const auto state = world.chunkMap.get_state(jobsForMeshing[i].chunkCoord);
        if (state->mesh==ChunkMeshState::NoMesh)
            state->logNewMeshEnqueue();
        else if (state->mesh==ChunkMeshState::DirtyMeshed)
            state->logDirtyMeshEnqueue();
    }
    profiler.bench_end("enqueueMesh");
    return count;
}


std::vector<MeshResult> drainMeshResultQueue(Queue<MeshResult>& queue,std::size_t maxUploads){
    for (std::size_t mesh_dq_attempts = 0; mesh_dq_attempts < maxUploads; mesh_dq_attempts++){
        std::optional<std::vector<MeshResult>> res = queue.try_batch_dequeue(maxUploads);
        if (res) return *res;
        if (!res) continue; // retry
    }

    return {};
}
std::size_t Simulation::drainAndUploadMeshResults(std::size_t maxUploads){
    profiler.bench_start("drainMesh");
    std::size_t count = 0;
    auto candidateMeshes = drainMeshResultQueue(rend.meshers.meshResultQueue,maxUploads);
    for (const auto& [candidateRevision, chunkCoord, opaque, transparent] : candidateMeshes){
        // TODO: consider making meshes optional?
        // A mesh might need to be uploaded evne if empty, and the draw just gets nullified

        auto state = world.chunkMap.try_get_state(chunkCoord);
        auto entry = world.chunkMap.try_get_entry(chunkCoord);
        auto loaded_mesh = world.chunkMap.try_get_meshEntry(chunkCoord);
        if (!loaded_mesh){
            world.chunkMap.assign_mesh_entry(chunkCoord,candidateRevision);
        }
        if (!loaded_mesh || loaded_mesh.value()->revisionID < candidateRevision){
            if (opaque.vertices.size()>0){
                rend.uploadMesh(chunkCoord, std::move(opaque));
            } 
            if (transparent.vertices.size()>0){
                rend.uploadMesh(chunkCoord, std::move(transparent));
            } 
        }
        if (loaded_mesh){
            if (state && state.value()->isOnMeshQueue()){
                if (candidateRevision >= state.value()->goal_meshRevisionID){
                    (*state)->logCleanMeshDequeue();
                }else{
                    (*state)->logDirtyMeshDequeue();
                }

                this->chunksMeshed++;
            }else{
                LOG_WARN("Homeless mesh not uploaded because its entry was deleted whilst it was being meshed.");
                LOG_DEBUG("Discarded chunk mesh data @{}; {}", chunkCoord, state ? "No entry" : "Not on gen queue");
                continue;
            }
        }
    }
    profiler.bench_end("drainMesh");
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

std::size_t Simulation::drainAndUploadGenResults(std::size_t maxUploads){
    profiler.bench_start("drainGen");
    auto genResults = drainGenResults(world.chunkMap.generator.genResultQueue,maxUploads);
    auto count = genResults.size();
    for (const auto& newGen : genResults){
        auto state = world.chunkMap.try_get_state(newGen.chunkCoord);
        if (state && state.value()->isOnGenQueue()){
            (*state)->logGenDequeue();
            world.chunkMap.uploadGeneratedChunk(newGen);
        }else{
            LOG_DEBUG("Discarded chunk gen data @{}; {}", newGen.chunkCoord, state ? "No entry" : "Not on gen queue");
        }
    }
    profiler.bench_end("drainGen");
    return genResults.size();
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
        const auto* state = world.chunkMap.get_state(key);
        if (inPlayerFrustum(key)){
            if (DebugChunkRenderer::HIDE_CLEAN_CHUNKS && state->isCleanMeshed()){
                continue; 
            }
            if (DebugChunkRenderer::HIDE_AIR_CHUNKS && entry->block_data.isAllAir()){
                continue; 
            }
            chunkOutlines.append_range(entry->bounding_box.getLines(MeshDebugOutlineColor(state->mesh)));
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
        rend.draw_debugChunks_to(playerCam,this,screenView());
        rend.draw_debugChunks_to(droneCam,this,secondaryView());
        rend.draw_3DLines_to(playerCam,chunkOutlines,screenView());
        rend.draw_3DLines_to(droneCam,chunkOutlines,secondaryView());
    }



    static bool first_draw = true;
    if (first_draw) {
        LOG_DEBUG("Finished first draw");
        first_draw = false;
    }
}

