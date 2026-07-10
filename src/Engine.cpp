
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
#include "Renderer.hpp"
#include "TextureAtlas.hpp"

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
#include "tracy/Tracy.hpp"


#include "FormatSpecs.hpp"
// TODO: 
// 1. add coordinate copy command CMD+SHIFT+C gives you set_pos_ori params
// 2. Fix pending writes
//

void Engine::loop(){
    while (!win.shouldClose()) {
        profiler.start_frame();
        profiler.bench_start("frame");

        profiler.bench_start("input");
        input.poll(); 
        handle_input();
        profiler.bench_end("input");

        if (!paused){ update_scene(); }
        rend.dbg_rend.update(player_cam,this);
    
        if (DebugOption::show_debug_ui){
            rend.update_player_cam_frustum_lines(this);
        }
        ui.update();

        draw_scene(); 
        if (DebugOption::show_debug_ui){ ui.draw(); }

        {
            profiler.bench_start("render");
            ZoneScopedN("Swap");
            win.swapBuffers();
            profiler.bench_end("render");
        }

        input.end_frame();
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
    rend.opaque_chunk_meshes.for_each_if_else(inside_frustum(this), load_mesh, unload_mesh);
    rend.transparent_chunk_meshes.for_each_if_else(inside_frustum(this), load_mesh, unload_mesh);
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
    // TODO: perhaps prune the for_meshing uniqueQueue?
    rend.opaque_chunk_meshes.erase_if(outside_range(world.chunkMap,lo,hi));
    rend.transparent_chunk_meshes.erase_if(outside_range(world.chunkMap, lo,hi));


}

void Engine::update_scene() {
    profiler.bench_start("update");
    director.start_frame(player_cam.pos);
    ZoneScoped;


    if (!chunk_updates_paused){

        n_chunks_discovered = director.discover_candidates(max_gen_discovery_pf, GENERATION_DIST,RENDER_DIST);

        submit_gen_jobs(maxGenJobsPerFrame);

        upload_gen_results(maxGenUploadsPerFrame);


        evict_meshes_outside_radius(MESH_CULL_DIST());
        refresh_visible_chunks();


        submit_mesh_jobs(maxMeshJobsPerFrame);

        upload_mesh_results(maxMeshUploadsPerFrame);

        //NOTE: We must perform mesh sorting AFTER mesh upload this frame,
        // otherwise the size of sorted_keys diverges from the mesh_lists.
        director.handle_mesh_sorting(rend,player_cam.pos);
        count_states();
    }



    update_player_cam(player_cam);
    update_drone_cam(drone_cam, player_cam.pos);


    director.end_frame();
    profiler.bench_end("update");
}


void Engine::update_player_cam(Camera& player_cam){
    player_cam.vertical_fov = DebugOption::player_cam_vfov;
}
void Engine::update_drone_cam(Camera& drone_cam, WorldFloatPos target_pos, f32 fly_height){
    auto follow_pos = WorldFloatPos{player_cam.pos.raw()+glm::vec3{0,100,0}};
    drone_cam.set_pos_ori(follow_pos, -89.0, 0.0);
}


void Engine::submit_gen_jobs(i64 maxJobs){
    profiler.bench_start("enqueueGen");
    const auto candidates = director.find_gen_jobs(maxJobs);
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
                    auto* entry = world.make_chunk_entry(candidate_coord);
                    director.mark_gen_enqueue(*entry);
                }
            );
            count++;
        }
    }
    profiler.bench_end("enqueueGen");
    gen_jobs_this_frame = count;
}


void Engine::submit_mesh_jobs(i64 maxJobs){
    profiler.bench_start("enqueueMesh");
    auto candidates = director.find_mesh_jobs(maxJobs);

    i64 count = 0;
    for (const auto& coord: candidates){
//        if (!is_chunk_in_frustum(player_cam.getCullFrustum(),coord)){
//            continue;
//        }
        auto& meshQ = rend.meshers.meshJobQueue;
        auto* entry = world.chunkMap.entries.at(coord);

        assert(entry);
        if (director.qualifies_for_mesh_enqueue(*entry)){
            // BlockShape 
            static_assert(BlockShape::CUBE == static_cast<BlockShape>(0));
            static_assert(BlockShape::CROSS == static_cast<BlockShape>(1));
            bool success = meshQ.try_emplace(
                coord,
                rend.atlas_list,
                &world.chunkMap,
                entry
            );
            // MeshJob(std::size_t _meshRevisionID, WorldChunkCoord key, const TextureAtlas* _atlas, const ChunkEntry* entry, std::span<std::optional<ChunkSlice2D>> neighbourChunks):
            if (success){
                director.mark_mesh_enqueue(*entry);
                count++;
            }
        }
    }

    profiler.bench_end("enqueueMesh");
    if (count>0 || candidates.size()>0){
    //    std::println("found {} mesh-ready candidates. {} uploaded.", candidates.size(),count);
    }
    mesh_jobs_this_frame =count;

}


void Engine::upload_mesh_results(i64 maxUploads){
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
    for (const auto& [candidate_revision, chunk_coord, opaque, transparent, cutout] : candidate_results){
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
                log_to_chunk("mesh_uploads",chunk_coord, "Mesh upload success ({}->{})",entry->loaded_mesh_revision,candidate_revision);
                log_to_chunk("mesh_uploads",chunk_coord, "OPQ:{},TRN:{},CUT:{}",opaque.vertices.size(),transparent.vertices.size(),cutout.vertices.size());
                //log_to_chunk(chunk_coord,"opaque new: {}",opaque.vertices.size());
                //log_to_chunk(chunk_coord,"transp new: {}",transparent.vertices.size());
                if (rend.opaque_chunk_meshes.contains(chunk_coord)){
                //    log_to_chunk(chunk_coord,"opaque before: {}",rend.opaqueChunkMeshes.at(chunk_coord));
                }
                if (opaque.vertices.size()>0){
                    rend.uploadMesh(chunk_coord, std::move(opaque));
                } 
                if (transparent.vertices.size()>0){
                    rend.uploadMesh(chunk_coord, std::move(transparent));
                } 
                if (cutout.vertices.size()>0){
                    rend.uploadMesh(chunk_coord, std::move(cutout));
                } 
                //log_to_chunk(chunk_coord,"opaque after: {}",rend.opaqueChunkMeshes.at(chunk_coord));
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
    mesh_results_this_frame = count;
}


void Engine::upload_gen_results(i64 maxUploads){
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
    for (const auto& newGen : genResults){
        bool success = world.chunkMap.entries.if_contains_else(
            newGen.chunkCoord,
            [&](ChunkEntry& entry){
                if (entry.qualifies_for_gen_dequeue()){
                    entry.state_transition(gen_dequeue);
                    return true;
                }else{
                    return false;
                }
            },
            [](){
                return false;
            }
        );
        if (success){
            director.upload_generated_chunk(newGen);
        }

    }
    profiler.bench_end("drainGen");
    gen_res_this_frame = genResults.size();
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
    
    if(DebugOption::fill_all_boundaries || DebugOption::fill_neighbour_boundaries || DebugOption::outline_all_boundaries || DebugOption::outline_neighbour_boundaries){
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
    world.chunkMap.entries.for_each(
        [&](WorldChunkCoord coord, ChunkEntry& entry){
            director.mark_mesh_dirty(entry);
        }
    );
    rend.opaque_chunk_meshes.clear();
    rend.transparent_chunk_meshes.clear();
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
    for (auto& v: block_defs){
        std::println("{}",v);
    }
    cpptrace::register_terminate_handler(); // gives us stack traces in std::terminate
    set_debug_params();                 

    program_epoch_ns = get_current_ns();



    win.set_callbacks(static_cast<void*>(this));
    LOG_DEBUG("Finished setting window callbacks.");


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
    director.init(player_cam.pos);
    LOG_DEBUG("Finished Camera setup.");


    ui.init(win.ptr);
    LOG_DEBUG("Finished UI setup.");

    world.setup();
    LOG_DEBUG("Finished World setup.");

    // enqueue the starting chunks
    submit_gen_jobs(maxGenJobsPerFrame);
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
void Engine::handle_input(){
    if (input.just_pressed(KEY_ESCAPE)){
        if (paused){
            // unpause
            paused = false; 
        } else{
            win.scheduleClose();
            return;
        }
    }
    if(input.just_pressed(KEY_GRAVE_ACCENT)){
        ui.is_ui_expanded = !ui.is_ui_expanded;
    }
    if(input.just_pressed(KEY_C) && input.mods.shift && input.mods.super){
        const auto& pos=player_cam.pos;
        const auto& yaw=player_cam.yaw;
        const auto& pitch=player_cam.pitch;
        auto str = 
            std::format(
                "{{{:+2.3f},{:+2.3f},{:+2.3f}}}, {:+2.3f},{:+2.3f}",
                pos.x,pos.y,pos.z, pitch, yaw
            );
        input.set_clipboard(
            str
        );
    }
    if (input.scroll.y != input.prevscroll.y){
        static constexpr f32 base = 1.1f;
        const f32 exponent = -1 * (input.scroll.y * drone_cam.zoom_sens * profiler.dt_s);
        drone_cam.ortho_zoom *= pow(base,exponent);
    }
    if(input.just_pressed(KEY_P)){
        paused = !paused;
        if (!paused){
            win.captureCursor();
            player_cam.enableMousePanning();
        }else{ 
            win.freeCursor();
            player_cam.disableMousePanning();
        }
    }
    if(input.just_pressed(KEY_G)){
        chunk_updates_paused= !chunk_updates_paused;
    }
    if(input.just_pressed(KEY_L)){
        pause_logging = !pause_logging;
    }
    if(input.just_pressed(KEY_X)){
        dirty_current_chunk = !dirty_current_chunk;
    }

    if (paused) return; // WARNING: Anything below here is ignored during paused frames


    if (input.mousepos != input.prevmousepos) {
        const glm::vec2 diff = input.prevmousepos - input.mousepos;
        player_cam.rotateByMouse(diff, profiler.dt_s);
    }

    if(input.just_pressed(KEY_B)){
        dbg_modify_chunks = !dbg_modify_chunks;
    }
    if(input.just_pressed(KEY_T)){
        rend.debug.wireframe = !rend.debug.wireframe;
    }
    if(input.just_pressed(KEY_H)){
        DebugOption::show_debug_ui = !DebugOption::show_debug_ui;
    }
    if(input.just_pressed(KEY_C) && input.no_mods()){
        DebugOption::fill_neighbour_boundaries = !DebugOption::fill_neighbour_boundaries;
        DebugOption::outline_neighbour_boundaries = !DebugOption::outline_neighbour_boundaries;
    }

    if(input.is_down(KEY_LEFT_SHIFT)){
            player_cam.moveSpeed = Camera::SPRINT_MOVESPEED;
            player_cam.keyboard_sensitivity= Camera::SPRINT_KEYBOARD_SENSITVITY;
    }else if(input.is_down(KEY_LEFT_CONTROL)){
        player_cam.moveSpeed = Camera::WALK_MOVESPEED;
    }else{
        player_cam.moveSpeed = Camera::BASE_MOVESPEED;
        player_cam.keyboard_sensitivity= Camera::BASE_KEYBOARD_SENSITIVITY;
    }

    if(input.is_down(KEY_W)){
		player_cam.move(Direction::FORWARD, profiler.dt_s);
	}
    if(input.is_down(KEY_S)){
		player_cam.move(Direction::BACKWARD, profiler.dt_s);
	}
    if(input.is_down(KEY_A)){
		player_cam.move(Direction::LEFT, profiler.dt_s);
	}
    if(input.is_down(KEY_D)){
		player_cam.move(Direction::RIGHT, profiler.dt_s);
	}
    if(input.is_down(KEY_SPACE)){
		player_cam.move(Direction::UP, profiler.dt_s);
	    drone_cam.move(Direction::UP, profiler.dt_s);
	}
    if(input.is_down(KEY_E)){
		player_cam.move(Direction::UP, profiler.dt_s);
	}
    if(input.is_down(KEY_Q)){
		player_cam.move(Direction::DOWN, profiler.dt_s);
	}

    if(input.is_down(KEY_LEFT)){
		player_cam.rotate(Direction::LEFT, profiler.dt_s);
	}
    if(input.is_down(KEY_RIGHT)){
		player_cam.rotate(Direction::RIGHT, profiler.dt_s);
	}
    if(input.is_down(KEY_UP)){
		player_cam.rotate(Direction::UP, profiler.dt_s);
	}
    if(input.is_down(KEY_DOWN)){
		player_cam.rotate(Direction::DOWN, profiler.dt_s);
	}

    if (dbg_modify_chunks){
        dbg_modify_chunks = false;
        auto cur_chunk = toWorldChunkCoord(player_cam.pos);
        director.mark_neighbours_dirty(cur_chunk, "test");
        world.chunkMap.entries.if_contains(
            cur_chunk,
            [&](ChunkEntry& entry){
                director.mark_mesh_dirty(entry);
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
            [&](ChunkEntry& entry){
                director.mark_mesh_dirty(entry);
            }
        );
    }
}
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
    rb_genJobsAdded.write(gen_jobs_this_frame);
    rb_genResultsAdded.write(gen_res_this_frame);
    rb_meshJobsAdded.write(mesh_jobs_this_frame);
    rb_meshResultsAdded.write(mesh_results_this_frame);

    sizeof(long long int);
    sizeof(i64);
    n_generating = std::max(0ll,n_generating + gen_jobs_this_frame - gen_res_this_frame);
    rb_generating.write(n_generating);

    n_meshing = n_meshing + mesh_jobs_this_frame - mesh_results_this_frame;
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
