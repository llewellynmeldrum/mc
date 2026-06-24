#include "glm/ext/matrix_float4x4.hpp"
#include <algorithm>
#include <optional>
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


void Simulation::cullMeshes(bool enableFrustumCulling){
    // if a mesh is outside of render dist, but still being meshed, cull it.
    auto lo = toWorldChunkCoord(playerCam.pos) + ChunkOffset{-MESH_CULL_DIST};
    auto hi = toWorldChunkCoord(playerCam.pos) + ChunkOffset{MESH_CULL_DIST};

    auto unload_mesh = [](IndexedMesh& mesh){
        mesh.unload();
    };
    auto load_mesh= [](IndexedMesh& mesh){
        mesh.load();
    };

    auto inside_frustum = [](Simulation* sim){
        return [sim](IndexedMesh& mesh){
            return sim->inPlayerFrustum(mesh.chunkCoord);
        };
    };
    auto outside_range = [](auto lo, auto hi){
        return [lo,hi](IndexedMesh& mesh){
            return !LM::isVecInBounds(mesh.chunkCoord, lo,hi);
        };
    };

    // load meshes inside frustum, unload meshes outside frustum
    rend.opaqueChunkMeshes.for_each_if_else(inside_frustum(this), load_mesh, unload_mesh);
    rend.transparentChunkMeshes.for_each_if_else(inside_frustum(this), load_mesh, unload_mesh);

    // erase all elements which are out of bounds:
    rend.opaqueChunkMeshes.erase_if(outside_range(lo,hi));
    rend.transparentChunkMeshes.erase_if(outside_range(lo,hi));
}
void Simulation::count_states(){
    n_generating = n_generating + genJobsThisFrame - genResultsThisFrame;
    rb_generating.write(n_generating);

    n_meshing = n_meshing + meshJobsThisFrame - meshResultsThisFrame;
    rb_meshing.write(n_meshing);

    n_gen_on_queue               ={};
    n_gen_done                   ={};

    n_mesh_awaiting_generation   ={};
    n_mesh_ready_for_enqueue     ={};
    n_mesh_on_queue              ={};
    n_mesh_done                  ={};
    for (const auto& [key, val]: world.chunkMap.entries){
        switch(val.state.gen){
            case GenState::on_queue:
                n_gen_on_queue++;
            break;
            case GenState::done:
                n_gen_done++;
            break;

        }
        switch(val.state.mesh){
            case MeshState::awaiting_generation: n_mesh_awaiting_generation++; break;
            case MeshState::ready_for_enqueue  : n_mesh_ready_for_enqueue  ++; break;
            case MeshState::on_queue           : n_mesh_on_queue           ++; break;
            case MeshState::done               : n_mesh_done               ++; break;
        }
    }

};

void Simulation::update() {
    profiler.bench_start("update");

    genJobsThisFrame = enqueueGenerationJobs(maxGenJobsPerFrame);
    rb_genJobsAdded.write(genJobsThisFrame);

    genResultsThisFrame = drainAndUploadGenResults(maxGenUploadsPerFrame);
    rb_genResultsAdded.write(genResultsThisFrame);

    cullMeshes(true);
    meshJobsThisFrame = enqueueMeshingJobs(maxMeshJobsPerFrame);
    rb_meshJobsAdded.write(meshJobsThisFrame);

    meshResultsThisFrame = drainAndUploadMeshResults(maxMeshUploadsPerFrame);
    rb_meshResultsAdded.write(meshResultsThisFrame);


    count_states();

    droneCam.pos = WorldFloatPos{playerCam.pos.raw()+glm::vec3{0,100,0}};
    droneCam.cached_viewMatrix.invalidate();
    droneCam.cached_frustum.invalidate();

    lines3d.clear();
    auto makeCamFrustumLines = [this](Camera& cam){
        auto frustum = cam.getFrustum();
        frustum.path.publish(lines3d);
        lines3d.append_range(frustum.extra_lines);
    };
//    makeCamFrustumLines(playerCam);
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
    SpiralIterateRange(
        chunkCoord, 
        SIMULATION_DIST*2, 
        SIMULATION_DIST, 
        [this, &candidates](i32 x, i32 y, i32 z) -> bool {
            const auto key = WorldChunkCoord{x,y,z};
            bool candidate_qualifies = world.chunkMap.entries.if_contains_else(
                key,
                [&](ChunkEntry& entry){
                    // 1. if an entry exists, check if it needs regeneration.
                    return entry.is_gen_dirty();
                },
                [&](){
                    // 2. if no entry exists; then the chunk hasnt been generated => it qualifies.
                    return true;
                }
            );
            if (candidate_qualifies) candidates.emplace_back(key);
            return candidate_qualifies;
        }
    );
    return candidates;
}

std::size_t Simulation::enqueueGenerationJobs(std::size_t maxJobs){
    profiler.bench_start("enqueueGen");
    const auto candidates = findChunksForGeneration(maxJobs);
    std::size_t count = 0;
    auto& genQ = world.chunkMap.generator.genJobQueue;
    for (const auto& candidate_coord: candidates){

        bool success = genQ.try_emplace(
            candidate_coord, 
            WORLD_SEED,
            world.genConfig
        );
        if (success){
            world.chunkMap.entries.if_contains_else(
                candidate_coord, 
                [&](ChunkEntry& entry){
                    entry.state_transition(gen_enqueue);
                },
                [&](){
                    world.make_chunk_entry(candidate_coord);
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
    auto meshReadyChunksInRad = [&](WorldChunkCoord chunkCoord, glm::ivec3 extents, i32 maxChunks=0) {
        std::vector<WorldChunkCoord> candidates;
        auto add = [&](i32 x, i32 y, i32 z){
            const auto key = WorldChunkCoord{x,y,z}; // dont you have to 
            bool added = world.chunkMap.entries.if_contains_else(
                key,
                [&](ChunkEntry& entry){
                    if (entry.qualifies_for_mesh_enqueue() ){
                        return true;
                    }else{
                        return false;
                    }
                },
                [&](){
                    // no entry, no enqueue
                    return false;
                }
            );
            if (added){
                candidates.emplace_back(key);
            }
            return added;
        };
        SpiralIterateRange(maxChunks, chunkCoord,extents.y, extents.x, add);
        return candidates;
    };
    const auto candidateList = meshReadyChunksInRad(camChunkCoord, RENDER_EXTENTS, maxJobs);

    std::vector<MeshJob> res;
    for (const auto candidateCoord: candidateList){
        auto* entry = AT(world.chunkMap.entries,candidateCoord);
        std::vector<std::optional<ChunkStore>> neighbours_copy;
        for (const auto& neighbour_coord: entry->neighbours){
            if (!neighbour_coord){
                neighbours_copy.emplace_back(std::nullopt);
                continue;
            }
            world.chunkMap.entries.if_contains_else(
                neighbour_coord.value(),
                [&](ChunkEntry& neighbour){
                    std::optional<ChunkStore> copy = {neighbour.block_data};
                    neighbours_copy.emplace_back(copy);
                },
                [&](){
                    neighbours_copy.emplace_back(std::nullopt);
                }
            );
        }
        res.emplace_back(entry->target_mesh_revision,candidateCoord,&rend.atlas,entry,neighbours_copy);
    }
    return res;
}

std::size_t Simulation::enqueueMeshingJobs(std::size_t maxJobs){
    profiler.bench_start("enqueueMesh");
    auto candidates = findMeshJobs(maxJobs);

    std::size_t count = 0;
    for (const auto& job: candidates){

        auto& meshQ = rend.meshers.meshJobQueue;
        bool success = meshQ.try_emplace(job);
        if (success){
            world.chunkMap.entries.if_contains_else(
                job.chunkCoord, 
                [&](ChunkEntry& entry){
                    entry.scheduled_mesh_revision = entry.target_mesh_revision;
                    entry.state_transition(mesh_enqueue);
                },
                [&](){
       //             LOG_WARN("(mesh_enqueue): Homeless mesh not enqueued because it has no chunk entry");
                }
            );
            count++;
        }
    }

    profiler.bench_end("enqueueMesh");
    if (count>0 || candidates.size()>0)
        std::println("found {} mesh-ready candidates. {} uploaded.", candidates.size(),count);
    return count;
}


std::size_t Simulation::drainAndUploadMeshResults(std::size_t maxUploads){
    profiler.bench_start("drainMesh");
    std::size_t count = 0;
    auto drain_mesh_results = [](auto& queue,auto maxUploads)->std::vector<MeshResult>{
        for (std::size_t mesh_dq_attempts = 0; mesh_dq_attempts < maxUploads; mesh_dq_attempts++){
            auto res = queue.try_batch_dequeue(maxUploads);
            if (res) return res.value();
        }
        return {};
    };
    auto candidateMeshes = drain_mesh_results(rend.meshers.meshResultQueue,maxUploads);
    for (const auto& [candidate_revision, chunkCoord, opaque, transparent] : candidateMeshes){
        auto* entry = world.chunkMap.entries.try_get(chunkCoord);

        if (entry && entry->is_mesh_on_queue()){
            if (entry->is_candidate_mesh_newer_than_loaded(candidate_revision)){
                if (opaque.vertices.size()>0){
                    rend.uploadMesh(chunkCoord, std::move(opaque));
                } 
                if (transparent.vertices.size()>0){
                    rend.uploadMesh(chunkCoord, std::move(transparent));
                } 
            }
            entry->loaded_mesh_revision = candidate_revision;
            entry->state_transition(mesh_dequeue);
            this->chunksMeshed++;
            count++;
        }else{
        //    LOG_WARN("(mesh_dequeue): Homeless mesh not uploaded because its entry was deleted whilst it was being meshed.\n"
        //             "\t\t State:{}",entry ? std::format("{}",entry->state):"n/a");
        //    // perhaps recreate the state entry here?
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
                break; // give up this frame
            }
        }
        return output;
    };
    auto genResults = drain_gen_results(world.chunkMap.generator.genResultQueue,maxUploads);
    auto count = genResults.size();
    for (const auto& newGen : genResults){
        bool success = world.chunkMap.entries.if_contains_else(
            newGen.chunkCoord,
            [&](ChunkEntry& entry){
                if (entry.qualifies_for_gen_dequeue()){
                    entry.state_transition(gen_dequeue);
                    return true;
                }else{
//                    LOG_DEBUG("(gen_upload): Discarded homeless chunk gen data @{}.", newGen.chunkCoord);
                    return false;
                }
            },
            [](){
                return false;
            }
        );
        if (success){
            world.chunkMap.uploadGeneratedChunk(newGen);

        }else{
 //           LOG_DEBUG("(gen_upload): Discarded homeless chunk gen data @{}.", newGen.chunkCoord);
        }
    }
    profiler.bench_end("drainGen");
    return genResults.size();
}

void Simulation::draw() {
    profiler.bench_start("draw");
    chunkOutlines.clear();

    world.chunkMap.entries.for_each([&](WorldChunkCoord key, ChunkEntry& entry){
        if (inPlayerFrustum(key)){
            if (DebugChunkRenderer::HIDE_CLEAN_CHUNKS && entry.is_mesh_clean()){
                return;
            }
            if (DebugChunkRenderer::HIDE_AIR_CHUNKS && entry.block_data.isAllAir()){
                return;
            }
            const auto& state = entry.state;
            auto color = ui.dbg_view.showGenState
                    ? GenDebugOutlineColor(state.gen)
                    : MeshDebugOutlineColor(state.mesh);

            chunkOutlines.append_range(entry.bounding_box.getLines(color));
        }
    });
    if (ui.dbg_view.showNeighbours){
        auto cam_chunk = toWorldChunkCoord(playerCam.pos);
        for (const auto& [dir, offset]: eachDirOffset){
            const auto neighbour = WorldChunkCoord{cam_chunk.raw()+offset};
            world.chunkMap.entries.if_contains(
                neighbour,
                [&](ChunkEntry& entry){
                    const auto& state = entry.state;
                    auto color = ui.dbg_view.showGenState
                            ? GenDebugOutlineColor(state.gen)
                            : MeshDebugOutlineColor(state.mesh);
                    chunkOutlines.append_range(entry.bounding_box.getLines(color,true));
                });
        }
        world.chunkMap.entries.if_contains(
            cam_chunk,
            [&](ChunkEntry& entry){
                    const auto& state = entry.state;
                    auto color = ui.dbg_view.showGenState
                            ? GenDebugOutlineColor(state.gen)
                            : MeshDebugOutlineColor(state.mesh);
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
    world.chunkMap.entries.clear();
}
void Simulation::unMeshAllChunks(){
    world.chunkMap.entries.for_each([](WorldChunkCoord coord, ChunkEntry& entry){
        entry.mark_mesh_dirty();
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
