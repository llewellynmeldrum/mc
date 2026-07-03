
#include "ChunkStorage.hpp"
#include "DebugOptions.hpp"
#include "glm/ext/matrix_float4x4.hpp"

#include "Chunk.hpp"
#include "ChunkConcurrency.hpp"
#include "ChunkEntry.hpp"
#include "ChunkHelpers.hpp"
#include "CoordTypes.hpp"
#include "DebugFormat.hpp"
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
#include <tracy/Tracy.hpp>


#include "FormatSpecs.hpp"
void Engine::loop(){
    while (!win.shouldClose()) {
        profiler.start_frame();
        profiler.bench_start("frame");

        bool dbg_modify_chunks = false;
        bool dirty_current_chunk = false;
        input.handle(profiler,ui,win,player_cam,drone_cam,rend,paused,chunk_updates_paused,dbg_modify_chunks,dirty_current_chunk); 
        if (dbg_modify_chunks){
            dbg_modify_chunks = false;
            auto cur_chunk = toWorldChunkCoord(player_cam.pos);
            world.chunkMap.mark_neighbours_dirty(cur_chunk, "test");
            world.chunkMap.entries.if_contains(
                cur_chunk,
                [](ChunkEntry& entry){
                    entry.mark_mesh_dirty();
                    for (auto& block : entry.block_data){
                        if (block.type == BlockType::GRASS_BLOCK){
                            block = (BlockType::AIR);
                        }
                    }
                }
            );
        }
        if (dirty_current_chunk){
            auto cur_chunk = toWorldChunkCoord(player_cam.pos);
            world.chunkMap.entries.if_contains(
                cur_chunk,
                [](ChunkEntry& entry){
                    entry.mark_mesh_dirty();
                }
            );
        }

        ui.update();
        if (!paused){
            update_scene();
        }

        draw_scene(); 
        if (DebugOption::showDebugUI){
            ui.draw(); 
        }

        profiler.bench_start("render");
        {
            ZoneScopedN("Swap");
            win.swapBuffers();
        }
        profiler.bench_end("render");

        profiler.bench_end("frame");
        profiler.end_frame();
    }
}


void Engine::refresh_visible_chunks(){
    auto load_mesh = [](IndexedMesh& mesh){
        mesh.load();
    };
    auto unload_mesh = [](IndexedMesh& mesh){
        mesh.unload();
    };
    auto inside_frustum = [](Engine* eng){
        return [eng](IndexedMesh& mesh){
            return eng->is_chunk_in_frustum(eng->player_cam.getCullFrustum(), mesh.chunkCoord);
        };
    };
    // load meshes inside frustum, unload meshes outside frustum
    rend.opaqueChunkMeshes.for_each_if_else(inside_frustum(this), load_mesh, unload_mesh);
    rend.transparentChunkMeshes.for_each_if_else(inside_frustum(this), load_mesh, unload_mesh);
}

void Engine::evict_meshes_outside_radius(i32 radius){
    // if a mesh is outside of render dist, but still being meshed, cull it.
    auto lo = toWorldChunkCoord(player_cam.pos) + ChunkOffset{-radius};
    auto hi = toWorldChunkCoord(player_cam.pos) + ChunkOffset{radius};


    // Prior to a meshes' erasure, update its chunk entry to reflect its erasure
    auto outside_range = [](ChunkMap& map, auto lo, auto hi){
        return [&map,lo,hi](IndexedMesh& mesh){
            bool out_of_bounds = !LM::isVecInBounds(mesh.chunkCoord, lo,hi);
            if (out_of_bounds){
                auto coord = mesh.chunkCoord;
                map.entries.if_contains(
                    coord,
                    [](ChunkEntry& entry){
                        if (entry.state.mesh == MeshState::done){
                            entry.mark_mesh_deleted();
                        }
                    }
                );
            }
            return out_of_bounds;
        };
    };

    // erase all elements which are out of bounds:
    rend.opaqueChunkMeshes.erase_if(outside_range(world.chunkMap,lo,hi));
    rend.transparentChunkMeshes.erase_if(outside_range(world.chunkMap, lo,hi));


}

void Engine::update_scene() {
    profiler.bench_start("update");
    ZoneScoped;
    if (DebugOption::fill_chunk_boundaries){
        rend.dbg_rend.update(player_cam,this);
    }
    if (DebugOption::showDebugUI){
        rend.update_player_cam_frustum_lines(this);
    }

    if (!chunk_updates_paused){
        if (director.did_player_cross_chunk_boundary(player_cam.pos)){
            genJobsThisFrame = submit_gen_jobs(maxGenJobsPerFrame);
            // we need this to probably not run every frame, but also we need to check it more often.
            // TODO: move chunk reordering here (for transparency, maybe also for opaque to reduce overdraw)
        }else{
            genJobsThisFrame = 0;
        }

        genResultsThisFrame = upload_gen_results(maxGenUploadsPerFrame);

        evict_meshes_outside_radius(MESH_CULL_DIST());
        refresh_visible_chunks();
        meshJobsThisFrame = submit_mesh_jobs(maxMeshJobsPerFrame);

        meshResultsThisFrame = upload_mesh_results(maxMeshUploadsPerFrame);
    }


    count_states();

    update_drone_cam(drone_cam, player_cam.pos);


    director.start_frame(player_cam.pos);
    director.end_frame();
    profiler.bench_end("update");
}


void Engine::update_drone_cam(Camera& drone_cam, WorldFloatPos target_pos, f32 fly_height){
    auto follow_pos = WorldFloatPos{player_cam.pos.raw()+glm::vec3{0,100,0}};
    drone_cam.set_pos_ori(follow_pos, -89.0, 0.0);
}

std::vector<WorldChunkCoord> Engine::findChunksForGeneration(i64 maxJobs){
    std::vector<WorldChunkCoord> candidates;
    const auto chunkCoord = toWorldChunkCoord(player_cam.pos);
    // enumerate them based on their range to the player, such that nearest chunks come first.

    for_each_spiral(
        maxJobs,
        chunkCoord, 
        GENERATION_DIST, 
        [&](i32 x, i32 z) -> bool {
            const auto key = WorldChunkCoord{x,z};
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

i64 Engine::submit_gen_jobs(i64 maxJobs){
    profiler.bench_start("enqueueGen");
    const auto candidates = findChunksForGeneration(maxJobs);
    i64 count = 0;
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

std::vector<MeshJob> Engine::find_mesh_jobs(i64 maxJobs){
    
    const auto camChunkCoord = toWorldChunkCoord(player_cam.pos);
    auto meshReadyChunksInRad = [&](WorldChunkCoord chunkCoord, i32 radius, i32 maxChunks=0) {
        std::vector<WorldChunkCoord> candidates;
        auto add = [&](i32 x, i32 z){
            const auto key = WorldChunkCoord{x,z};
            bool added = world.chunkMap.entries.if_contains_else(
                key,
                [&](ChunkEntry& entry){
                    // TODO: Figure out if this is correct.
                    // Maybe mesh chunks in a wider frustum?
                    // Add an option to generate a frustum with a given FOV, say 180 for wider range
                    if (!is_chunk_in_frustum(player_cam.getCullFrustum(),key)){
                        return false;
                    }
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
        for_each_spiral(maxChunks, chunkCoord, radius, add);
        return candidates;
    };
    const auto candidateList = meshReadyChunksInRad(camChunkCoord, RENDER_DIST, maxJobs);

    std::vector<MeshJob> res;
    for (const auto candidateCoord: candidateList){
        auto* entry = AT(world.chunkMap.entries,candidateCoord);
        std::vector<std::optional<ChunkSlice2D>> neighbours_copy;
        for (const auto& [dir, dir_idx]: eachDirIndex2D){
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

i64 Engine::submit_mesh_jobs(i64 maxJobs){
    profiler.bench_start("enqueueMesh");
    auto candidates = find_mesh_jobs(maxJobs);

    i64 count = 0;
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
    if (count>0 || candidates.size()>0){
    //    std::println("found {} mesh-ready candidates. {} uploaded.", candidates.size(),count);
    }
    return count;

}


i64 Engine::upload_mesh_results(i64 maxUploads){
    profiler.bench_start("drainMesh");
    i64 count = 0;
    auto drain_mesh_results = [](auto& queue,auto maxUploads)->std::vector<MeshResult>{
        for (i64 mesh_dq_attempts = 0; mesh_dq_attempts < maxUploads; mesh_dq_attempts++){
            auto res = queue.try_batch_dequeue(maxUploads);
            if (res) return res.value();
        }
        return {};
    };
    auto candidate_results = drain_mesh_results(rend.meshers.meshResultQueue,maxUploads);
    for (const auto& [candidate_revision, chunk_coord, opaque, transparent] : candidate_results){
        auto log_fail_upload = [&](std::string_view str){
            log_to_chunk(chunk_coord, "Mesh upload rejected: {}.",str);
        };
//        if (director.is_chunk_outside_cull_distance(chunkCoord,MESH_CULL_DIST())){
//            // skip the result, it would otherwise be culled instantly 
//            continue;
//        }
        auto* entry = world.chunkMap.entries.try_get(chunk_coord);
        if (!entry){
            // NOTE: this should probably be an assert.
            log_fail_upload("Entry does not exist????");
            continue;
        }

        if (entry->is_mesh_on_queue()){
            if (entry->is_candidate_mesh_newer_than_loaded(candidate_revision)){
                log_to_chunk(chunk_coord, "Mesh upload success! ({}->{})",entry->loaded_mesh_revision,candidate_revision);
                log_to_chunk(chunk_coord,"opaque new: {}",opaque.vertices.size());
                log_to_chunk(chunk_coord,"transp new: {}",transparent.vertices.size());
                if (rend.opaqueChunkMeshes.contains(chunk_coord)){
                    log_to_chunk(chunk_coord,"opaque before: {}",rend.opaqueChunkMeshes.at(chunk_coord));
                }
                if (opaque.vertices.size()>0){
                    rend.uploadMesh(chunk_coord, std::move(opaque));
                } 
                if (transparent.vertices.size()>0){
                    rend.uploadMesh(chunk_coord, std::move(transparent));
                } 
                log_to_chunk(chunk_coord,"opaque after: {}",rend.opaqueChunkMeshes.at(chunk_coord));
            }else{
                log_fail_upload(std::format("Candidate rev ({}) is older than loaded ({}).",
                                candidate_revision,entry->loaded_mesh_revision));
            }
            entry->loaded_mesh_revision = candidate_revision;
            entry->state_transition(mesh_dequeue);
            this->chunksMeshed++;
            count++;
        }else{
            log_fail_upload(std::format("Result popped, however state!=on_queue, rather:{}",entry->state));
            continue;
        }
        
    }
    profiler.bench_end("drainMesh");
    return count;

}


i64 Engine::upload_gen_results(i64 maxUploads){
    profiler.bench_start("drainGen");
    auto drain_gen_results = [](Queue<GenResult>& queue, i64 maxUploads){
        std::vector<GenResult> output; output.reserve(maxUploads);

        for (i64 mesh_count = 0; mesh_count < maxUploads; mesh_count++){
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
            world.chunkMap.upload_generated_chunk(newGen);

        }else{
 //           LOG_DEBUG("(gen_upload): Discarded homeless chunk gen data @{}.", newGen.chunkCoord);
        }
    }
    profiler.bench_end("drainGen");
    return genResults.size();
}

void Engine::draw_chunk_boundaries(Camera& cam, RenderTargetView target ){
    rend.draw_debugChunks_to(player_cam,this,screen_view());
    rend.draw_3DLines_to(cam,rend.dbg_rend.chunk_outlines,target);
}
void Engine::draw_scene() {
    ZoneScoped;
    profiler.bench_start("draw");


    player_cam.aspectRatio = win.aspect();

    rend.debug.reset_per_frame();
    rend.clear_to(screen_view());
    rend.clear_to(secondaryView());

    rend.draw_to(player_cam, screen_view());
    rend.draw_to(drone_cam, secondaryView());


    // Drone cam sees players' frustum lines 
    rend.draw_3DLines_to(drone_cam, rend.player_cam_frustum_lines, secondaryView());
    
    if(DebugOption::fill_chunk_boundaries){
        draw_chunk_boundaries(player_cam, screen_view());
        draw_chunk_boundaries(drone_cam, secondaryView());
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

void Engine::set_debug_params() {
    g_StyleConfig::disabled = unix::is_debugger_present();

    // This relies on the Makefile, which exports an environment variable based on the target.
    // Each make target has a certain optimisation level set, (e.g `make fast`=O2, `make faster` is O3)
    // Theres probably a better way to do this (like ask the compiler?) but I figured this is good enough.
    auto optimization_level = unix::get_env<int>("OPT_LEVEL");
    if (optimization_level){
        LOG_DEBUG("{}",*optimization_level);
        DebugOption::compiler_optimisation_level = *optimization_level;
    }else{
        LOG_WARN("No OPT_LEVEL env found.");
        DebugOption::compiler_optimisation_level = -1; // To signify an error
    }
}

void Engine::setup() {
    cpptrace::register_terminate_handler(); // gives us stack traces in std::terminate
    set_debug_params();                 

    program_epoch_ns = get_current_ns();



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

    player_cam.is_main_camera=true;
    drone_cam.vertical_fov = 50.0f;
    player_cam.set_pos_ori({0, 168, 0}, -23.4, 56.3);
    director.init(player_cam.pos);
    LOG_DEBUG("Finished Camera setup.");


    ui.init(win.ptr);
    LOG_DEBUG("Finished UI setup.");

    world.setup();
    LOG_DEBUG("Finished World setup.");

    // enqueue the starting chunks
    genJobsThisFrame = submit_gen_jobs(maxGenJobsPerFrame);
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
RenderTargetView Engine::screen_view() {
    return {
        .pos = {win.x,win.y},
        .size = {win.px_w, win.px_h},
    };
}
RenderTargetView Engine::secondaryView() {
    return fixedCamTarget.view();
}

// =======
// debugging
// ========
void Engine::count_states(){
    rb_genJobsAdded.write(genJobsThisFrame);
    rb_genResultsAdded.write(genResultsThisFrame);
    rb_meshJobsAdded.write(meshJobsThisFrame);
    rb_meshResultsAdded.write(meshResultsThisFrame);

    sizeof(long long int);
    sizeof(i64);
    n_generating = std::max(0ll,n_generating + genJobsThisFrame - genResultsThisFrame);
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
