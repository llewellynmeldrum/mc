#include "ChunkStorage.hpp"
#include "glm/ext/matrix_float4x4.hpp"

#include "Chunk.hpp"
#include "ChunkConcurrency.hpp"
#include "ChunkEntry.hpp"
#include "ChunkHelpers.hpp"
#include "CoordTypes.hpp"
#include "DebugFormat.hpp"
#include "FormatSpecs.hpp"
#include "CoordIteration.hpp"

#include "Engine.hpp"

#include "LM.hpp"
#include "Line3D.hpp"
#include "Profiler.hpp"
#include "Logger.hpp"
#include "Assertion.hpp"
#include "FmtStyle.hpp"
#include "UnixHelpers.hpp"

#include <algorithm>
#include <optional>
#include <print>


void Engine::loop(){
    while (!win.shouldClose()) {
        profiler.start_frame();
        profiler.bench_start("frame");

        input.handle(profiler,ui,win,playerCam,droneCam,rend,paused); 

        ui.update();
        if (!paused){
            update_scene();
        }

        draw_scene(); 
        if (ui.dbg_view.showDebugUI){
            ui.draw(); 
        }

        profiler.bench_start("render");
        win.swapBuffers();
        profiler.bench_end("render");

        profiler.bench_end("frame");
        profiler.end_frame();
    }
}



void Engine::cullMeshes(bool enableFrustumCulling){
    // if a mesh is outside of render dist, but still being meshed, cull it.
    auto lo = toWorldChunkCoord(playerCam.pos) + ChunkOffset{-MESH_CULL_DIST};
    auto hi = toWorldChunkCoord(playerCam.pos) + ChunkOffset{MESH_CULL_DIST};

    auto unload_mesh = [](IndexedMesh& mesh){
        mesh.unload();
    };
    auto load_mesh= [](IndexedMesh& mesh){
        mesh.load();
    };

    auto inside_frustum = [](Engine* eng){
        return [eng](IndexedMesh& mesh){
            return eng->is_chunk_in_frustum(eng->playerCam.getFrustum(), mesh.chunkCoord);
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
void Engine::count_states(){
    rb_genJobsAdded.write(genJobsThisFrame);
    rb_genResultsAdded.write(genResultsThisFrame);
    rb_meshJobsAdded.write(meshJobsThisFrame);
    rb_meshResultsAdded.write(meshResultsThisFrame);

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

void Engine::update_scene() {
    profiler.bench_start("update");
    if (ui.dbg_view.showChunkBoundaries){
        rend.dbg_rend.update(playerCam,this);
    }
    if (ui.dbg_view.showDebugUI){
        rend.update_player_cam_frustum_lines(this);
    }

    if (director.did_player_cross_chunk_boundary(playerCam.pos)){
        genJobsThisFrame = enqueueGenerationJobs(maxGenJobsPerFrame);
    }else{
        genJobsThisFrame =0;
    }

    genResultsThisFrame = drainAndUploadGenResults(maxGenUploadsPerFrame);

    cullMeshes(true);
    meshJobsThisFrame = enqueueMeshingJobs(maxMeshJobsPerFrame);

    meshResultsThisFrame = drainAndUploadMeshResults(maxMeshUploadsPerFrame);


    count_states();

    droneCam.pos = WorldFloatPos{playerCam.pos.raw()+glm::vec3{0,100,0}};
    droneCam.cached_viewMatrix.invalidate();
    droneCam.cached_frustum.invalidate();


    director.end_frame(playerCam.pos);
    profiler.bench_end("update");
}


std::vector<WorldChunkCoord> Engine::findChunksForGeneration(std::size_t maxJobs){
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

std::size_t Engine::enqueueGenerationJobs(std::size_t maxJobs){
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

std::vector<MeshJob> Engine::findMeshJobs(std::size_t maxJobs){
    
    const auto camChunkCoord = toWorldChunkCoord(playerCam.pos);
    auto meshReadyChunksInRad = [&](WorldChunkCoord chunkCoord, glm::ivec3 extents, i32 maxChunks=0) {
        std::vector<WorldChunkCoord> candidates;
        auto add = [&](i32 x, i32 y, i32 z){
            const auto key = WorldChunkCoord{x,y,z};
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
    const auto candidateList = meshReadyChunksInRad(camChunkCoord, RENDER_EXTENTS(), maxJobs);

    std::vector<MeshJob> res;
    for (const auto candidateCoord: candidateList){
        auto* entry = AT(world.chunkMap.entries,candidateCoord);
        std::vector<std::optional<ChunkSlice2D>> neighbours_copy;
        for (const auto& [dir, dir_idx]: eachDirIndex){
            const auto& neighbour_coord = entry->neighbours[dir_idx];
            if (!neighbour_coord){
                neighbours_copy.emplace_back(std::nullopt);
                continue;
            }
            ChunkBlockPos p0{}, p1{};
            constexpr auto XE = CHUNK_XWIDTH;
            constexpr auto YE = CHUNK_HEIGHT;
            constexpr auto ZE = CHUNK_ZWIDTH;
            SliceType slice_type = {};
            switch (dir){
                // -Z
                case Direction ::BACKWARD: p0 = {0,0,0}; p1 ={XE,YE,1}; slice_type = SliceType::Z; break;
                // +Z
                case Direction ::FORWARD: p0 = {0,0,ZE-1}; p1 ={XE,YE,ZE}; slice_type = SliceType::Z; break;

                // -X
                case Direction ::RIGHT: p0 = {0,0,0}; p1 ={1,YE,ZE}; slice_type = SliceType::X; break;
                // +X
                case Direction ::LEFT   : p0 = {XE-1,0,0}; p1 ={XE,YE,ZE}; slice_type = SliceType::X; break;

                // - Y
                case Direction ::UP :p0 = {0,0,0}; p1 ={XE,1,ZE}; slice_type = SliceType::Y; break;
                // + Y
                case Direction ::DOWN     :p0 = {0,YE-1,0}; p1 ={XE,YE,ZE}; slice_type = SliceType::Y; break;
                default:
                    break;
            }
            world.chunkMap.entries.if_contains_else(
                neighbour_coord.value(),
                [&](ChunkEntry& neighbour){
                    std::optional<ChunkSlice2D> copy = std::make_optional<ChunkSlice2D>(
                       &neighbour.block_data,
                        slice_type,
                        p0,
                        p1
                    );
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

std::size_t Engine::enqueueMeshingJobs(std::size_t maxJobs){
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
                    entry.inflight_mesh_revision = entry.target_mesh_revision;
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


std::size_t Engine::drainAndUploadMeshResults(std::size_t maxUploads){
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


std::size_t Engine::drainAndUploadGenResults(std::size_t maxUploads){
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

void Engine::draw_chunk_boundaries(Camera& cam, RenderTargetView target ){
    rend.draw_debugChunks_to(playerCam,this,screenView());
    rend.draw_3DLines_to(cam,rend.dbg_rend.chunk_outlines,target);
}
void Engine::draw_scene() {
    profiler.bench_start("draw");


    playerCam.aspectRatio = win.aspect();

    rend.debug.reset_per_frame();
    rend.clear_to(screenView());
    rend.clear_to(secondaryView());

    rend.draw_to(playerCam, screenView());
    rend.draw_to(droneCam, secondaryView());


    // Drone cam sees players' frustum lines 
    rend.draw_3DLines_to(droneCam, rend.player_cam_frustum_lines, secondaryView());
    
    if(ui.dbg_view.showChunkBoundaries){
        draw_chunk_boundaries(playerCam, screenView());
        draw_chunk_boundaries(droneCam, secondaryView());
    }



    static bool first_draw = true;
    if (first_draw) {
        LOG_DEBUG("Finished first draw");
        first_draw = false;
    }
    profiler.bench_end("draw");
}




// BUG: 
// These functions are kinda clarted. Im not clear on whether or not they properly interact with 
// all the queues and stuff. They generate so many edge cases I question their usefulness.
// For example:
// -> How do we inform meshes on the queue that their entries have been nuked?
//      What do we do with those homeless meshes? we must check for and discard them. 
void Engine::unGenerateAllChunks(){
    world.chunkMap.entries.clear();
}
void Engine::unMeshAllChunks(){
    world.chunkMap.entries.for_each([](WorldChunkCoord coord, ChunkEntry& entry){
        entry.mark_mesh_dirty();
    });
    rend.opaqueChunkMeshes.clear();
    rend.transparentChunkMeshes.clear();
}

void Engine::setup() {
    cpptrace::register_terminate_handler(); // gives us stack traces in std::terminate
    g_StyleConfig::disabled = unix::is_debugger_present();
    auto optimization_level = unix::get_env<int>("OPT_LEVEL");
    if (optimization_level){
        std::println("{}",*optimization_level);
        ui.dbg_params.opt_lvl = *optimization_level;
    }else{
        std::println("No OPT_LEVEL env found.");
    }

    program_epoch_ns = get_current_ns();

    init_state_transition_logger(this);
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
    director.end_frame(playerCam.pos);
    LOG_DEBUG("Finished Camera setup.");


    ui.init(win.ptr);
    LOG_DEBUG("Finished UI setup.");

    world.setup();
    LOG_DEBUG("Finished World setup.");

    // enqueue the starting chunks
    genJobsThisFrame = enqueueGenerationJobs(maxGenJobsPerFrame);
}


i32 Engine::exit(i32 exit_code) {
    ui.destroy();
    win.terminate();
    std::exit(exit_code);
    return exit_code;
}

// =========
// Helpers 
// =========
bool Engine::is_chunk_in_frustum(const Frustum& frustum, WorldChunkCoord coord) const{
    return frustum.isAABBInside(world.chunkMap.getBoundingBox(coord));
}
RenderTargetView Engine::screenView() {
    return {
        .pos = {win.x,win.y},
        .size = {win.px_w, win.px_h},
    };
}
RenderTargetView Engine::secondaryView() {
    return fixedCamTarget.view();
}
