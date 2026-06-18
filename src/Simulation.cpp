#include "glm/ext/matrix_float4x4.hpp"
#include <algorithm>
#include <print>

#include "Chunk.hpp"
#include "ChunkConcurrency.hpp"
#include "ChunkEntry.hpp"
#include "ChunkHelpers.hpp"
#include "CoordTypes.hpp"
#include "DebugFormat.hpp"
#include "FormatSpecs.hpp"
#include "CoordIteration.hpp"

#include "Simulation.hpp"

#include "LM.hpp"
#include "Line3D.hpp"
#include "Profiler.hpp"
#include "Logger.hpp"
#include "Assertion.hpp"

using namespace glm;
void Simulation::setupSimulation() {
    init_state_transition_logger(this);
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
    draw(); 
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
    SpiralIterateRange(chunkCoord, SIMULATION_DIST*2, SIMULATION_DIST, 
                       [this, &candidates](i32 x, i32 y, i32 z) -> bool {
        const auto key = WorldChunkCoord{x,y,z};
        // 1. if an state entry exists, check if it needs regeneration.
        auto* state = world.chunkMap.states.try_get(key);
        if(state){
            if (state->gen.isDirty()){
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
    std::size_t count = 0;
    auto& genQ = world.chunkMap.generator.genJobQueue;
    for (const auto& chunkWorldPos: candidates){

        bool success = genQ.try_emplace(
            chunkWorldPos, 
            WORLD_SEED,
            world.genConfig
        );
        if (success){
            world.chunkMap.states.if_contains_else(chunkWorldPos, 
                [&](ChunkState& state){
                    state.transition(gen_enqueue);
                },
                [&](){
                    bool inserted = world.chunkMap.states.try_emplace(chunkWorldPos,chunkWorldPos);
                    assert(inserted);
                }
            );
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
        auto entry = world.chunkMap.chunk_entries.at(candidateCoord);
        auto state = world.chunkMap.states.at(candidateCoord);
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

    for (const auto& job: jobsForMeshing){
        const auto entry = world.chunkMap.chunk_entries.at(job.chunkCoord);
        const auto state = world.chunkMap.states.at(job.chunkCoord);
        state->transition(mesh_enqueue);
    }
    profiler.bench_end("enqueueMesh");
    return count;
}


std::size_t Simulation::drainAndUploadMeshResults(std::size_t maxUploads){
    profiler.bench_start("drainMesh");
    std::size_t count = 0;
    auto drain_mesh_results = [](Queue<MeshResult>& queue,std::size_t maxUploads)->std::vector<MeshResult>{
        for (std::size_t mesh_dq_attempts = 0; mesh_dq_attempts < maxUploads; mesh_dq_attempts++){
            auto res = queue.try_batch_dequeue(maxUploads);
            if (res) return res.value();
        }
        return {};
    };
    auto candidateMeshes = drain_mesh_results(rend.meshers.meshResultQueue,maxUploads);
    for (const auto& [candidate_revision, chunkCoord, opaque, transparent] : candidateMeshes){
        // TODO: consider making meshes optional?
        // A mesh might need to be uploaded evne if empty, and the draw just gets nullified

        auto state = world.chunkMap.states.try_get(chunkCoord);
        auto entry = world.chunkMap.chunk_entries.try_get(chunkCoord);
        auto loaded_mesh_entry = world.chunkMap.mesh_entries.get_or_insert(chunkCoord,candidate_revision);

        // only accept newer or equal 
        if (candidate_revision >= loaded_mesh_entry->revisionID){
            if (opaque.vertices.size()>0){
                rend.uploadMesh(chunkCoord, std::move(opaque));
            } 
            if (transparent.vertices.size()>0){
                rend.uploadMesh(chunkCoord, std::move(transparent));
            } 
        }
        if (state && state->mesh.stage==MeshStage::on_queue){
            state->transition(mesh_dequeue);
            if (candidate_revision >= state->goal_meshRevisionID){
                state->mark_clean_mesh(candidate_revision);
            }

            this->chunksMeshed++;
        }else{
            LOG_WARN("(mesh_dequeue): Homeless mesh not uploaded because its entry was deleted whilst it was being meshed.\n"
                     "\t\t State:{}",state ? std::format("{}",*state):"n/a");
            // perhaps recreate the state entry here?
            continue;
        }
    }
    profiler.bench_end("drainMesh");
    return count;
}


std::size_t Simulation::drainAndUploadGenResults(std::size_t maxUploads){
    profiler.bench_start("drainGen");
    auto drain_gen_results = [](Queue<GenResult>& queue, std::size_t maxUploads){
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
    };
    auto genResults = drain_gen_results(world.chunkMap.generator.genResultQueue,maxUploads);
    auto count = genResults.size();
    for (const auto& newGen : genResults){
        bool err = world.chunkMap.states.if_contains_else(
            newGen.chunkCoord,
            [](ChunkState& state){
                return state.gen.stage==GenStage::on_queue;
            },
            [](){
                return false;
            }
        );
        if (err){
            LOG_DEBUG("(gen_upload): Discarded homeless chunk gen data @{}.", newGen.chunkCoord);
        }
    }
    profiler.bench_end("drainGen");
    return genResults.size();
}

void Simulation::draw() {
    profiler.bench_start("draw");
    chunkOutlines.clear();

    world.chunkMap.chunk_entries.for_each([&](WorldChunkCoord key, ChunkEntry& entry){
        const auto* state = world.chunkMap.states.at(key);
        if (inPlayerFrustum(key)){
            if (DebugChunkRenderer::HIDE_CLEAN_CHUNKS && state->mesh.isClean()){
                return;
            }
            if (DebugChunkRenderer::HIDE_AIR_CHUNKS && entry.block_data.isAllAir()){
                return;
            }
            auto color = ui.dbg_view.showMeshState    
                    ? MeshDebugOutlineColor(state->mesh)
                    : GenDebugOutlineColor(state->gen);

            chunkOutlines.append_range(entry.bounding_box.getLines(color));
        }
    });
    if (ui.dbg_view.showNeighbours){
        auto cam_chunk = toWorldChunkCoord(playerCam.pos);
        for (const auto& [dir, offset]: eachDirOffset){
            const auto neighbour = WorldChunkCoord{cam_chunk.raw()+offset};
            world.chunkMap.chunk_entries.if_contains(
                neighbour,
                [&](ChunkEntry& entry){
                    auto color = NeighbourDebugOutlineColor(128);
                    chunkOutlines.append_range(entry.bounding_box.getLines(color,true));
                });
        }
        world.chunkMap.chunk_entries.if_contains(
            cam_chunk,
            [&](ChunkEntry& entry){
                auto color = NeighbourDebugOutlineColor();
                chunkOutlines.append_range(entry.bounding_box.getLines(color,true));
            });
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
    profiler.bench_end("draw");
}


void Simulation::freeCursor(){
    win.freeCursor();
    playerCam.disableMousePanning();
}


// BUG: 
// These functions are kinda clarted. Im not clear on whether or not they properly interact with 
// all the queues and stuff. They generate so many edge cases I question their usefulness.
// For example:
// -> How do we inform meshes on the queue that their entries have been nuked?
//      What do we do with those homeless meshes? we must check for and discard them. 
void Simulation::unGenerateAllChunks(){
    world.chunkMap.chunk_entries.clear();
    world.chunkMap.states.clear();
}
void Simulation::unMeshAllChunks(){
    world.chunkMap.states.for_each([](WorldChunkCoord coord, ChunkState& state){
        state.mesh.stage = MeshStage::ready_for_enqueue;
    });
    rend.opaqueChunkMeshes.clear();
    rend.transparentChunkMeshes.clear();
}




void Simulation::captureCursor(){
    win.captureCursor();
    playerCam.enableMousePanning();
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
