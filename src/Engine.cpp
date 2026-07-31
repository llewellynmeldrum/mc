#include <algorithm>
#include <optional>
#include <queue>
#include <chrono>
#include <print>
#include <ratio>
#include <flat_map>
#include <thread>
#include <unordered_map>
#include <utility>


#include "ChunkConstants.hpp"
#include "Lighting.hpp"
#include "ChunkStorage.hpp"
#include "DebugChunkLog.hpp"
#include "DebugOptions.hpp"
#include "Direction.hpp"
#include "GlobalDebugLog.hpp"
#include "PendingBlockWrites.hpp"
#include "WorldGen_BiomeFeatureSets.hpp"
#include "glm/ext/matrix_float4x4.hpp"

#include "Chunk.hpp"
#include "cpp23_ranges.hpp"
#include "ChunkConcurrency.hpp"
#include "ChunkEntry.hpp"
#include "ChunkHelpers.hpp"
#include "CoordTypes.hpp"
#include "DebugFormat.hpp"
#include "CoordIteration.hpp"
#include "Renderer.hpp"
#include "TextureAtlas.hpp"

#include "Engine.hpp"
#include "Timer.hpp"

#include "LM.hpp"
#include "Line3D.hpp"
#include "Logger.hpp"
#include "Assertion.hpp"
#include "FmtStyle.hpp"
#include "UnixHelpers.hpp"
#include "FormatSpecs.hpp"


void Engine::per_tick_update(){
    update_player_cam(player_cam);
    update_drone_cam(drone_cam, player_cam.pos);
    world.tick();
    tick_counter++;
}
void Engine::loop(){
    while (!win.shouldClose()) {
        auto dt = timer::now() - t_last_frame_ended;
        profiler.start_frame();
        profiler.bench_start("frame");

        profiler.bench_start("input");
            input.poll(); 
            handle_input();
        profiler.bench_end("input");

        if (!paused){
            if (!chunk_updates_paused){
                handle_chunk_scheduling(); 
            }
            update(dt);
        }
        rend.dbg_rend.update(player_cam,this);
    
        if (DebugOption::show_debug_ui){
            rend.update_player_cam_frustum_lines(this);
        }
        ui.update();

        draw_scene(); 
        if (DebugOption::show_debug_ui){ ui.draw(); }

        {
            profiler.bench_start("render");
            win.swapBuffers();
            profiler.bench_end("render");
        }

        input.end_frame();
        profiler.bench_end("frame");
        profiler.end_frame();
        t_last_frame_ended = timer::now();
    }
}





void Engine::classify_visible_chunks(){
    auto cur_chunk_pos_raw = director.cur_chunk_pos;
    auto assign_dist = [cur_chunk_pos_raw](IndexedMesh& mesh){
        mesh.chunk_dist_to_cam = LM::dist(mesh.chunkCoord, cur_chunk_pos_raw);
    };
    rend.opaque_chunk_meshes.for_each(assign_dist);
    rend.blended_chunk_meshes.for_each(assign_dist);
    rend.cutout_chunk_meshes.for_each(assign_dist);
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
    rend.blended_chunk_meshes.for_each_if_else(inside_frustum(this), load_mesh, unload_mesh);
    rend.cutout_chunk_meshes.for_each_if_else(inside_frustum(this), load_mesh, unload_mesh);
}

void Engine::evict_meshes_outside_radius(i32 radius){
    // if a mesh is outside of render dist, but still being meshed, cull it.
    auto lo = toWorldChunkCoord(player_cam.pos) + ChunkOffset{-radius};
    auto hi = toWorldChunkCoord(player_cam.pos) + ChunkOffset{radius};


    // Prior to a meshes' erasure, update its chunk entry to reflect its erasure
    auto outside_range = [&](ChunkMap& map, auto lo, auto hi){
        return [&, lo, hi](IndexedMesh& mesh){
            bool out_of_bounds = !LM::isVecInBounds(mesh.chunkCoord, lo,hi);
            if (out_of_bounds){
                auto coord = mesh.chunkCoord;
                auto* entry = map.entries.try_get(coord);
                if (entry){
                    director.mark_mesh_deleted(entry);
                }
            }
            return out_of_bounds;
        };
    };

    // erase all elements which are out of bounds:
    // TODO: perhaps prune the for_meshing uniqueQueue?
    rend.opaque_chunk_meshes.erase_if(outside_range(world.chunkMap,lo,hi));
    rend.blended_chunk_meshes.erase_if(outside_range(world.chunkMap, lo,hi));
    rend.cutout_chunk_meshes.erase_if(outside_range(world.chunkMap, lo,hi));
}




void Engine::process_lighting_updates(){
    constexpr static auto N_LIGHTING_JOBS = 128uz;
    for (const auto& coord: director.find_lighting_jobs(128)){
        auto* entry = director.chunk_map.entries.try_get(coord);
        if (director.qualifies_for_light_work(entry)){
            auto candidate_rev = entry->lighting.get_candidate_rev();
            if (candidate_rev == RevisionState::FIRST_JOB){
                entry->light_data.reset();
            }
            assert(entry->light_data.buf.size() == ChunkInfo::SIZE);

            auto res = process_lighting(LightingJob(coord,&director.chunk_map,entry));
            director.upload_light_result(entry,std::move(res));
        }
    }
}

void Engine::handle_chunk_scheduling() {
    profiler.bench_start("update");
    director.start_frame(player_cam.pos);


//    for_each_xz_in_chunk([&](i32 cx, i32 cz){
        if (director.block_at({3,5,3}) != BlockType::TORCH){
            bool success = director.place_block(WorldBlockPos{3,5,3},BlockType::TORCH);
            std::println("Placing torch: {}", success ? "success" : "failed");
            for_each_xz_exclusive(glm::ivec2{2,2},glm::ivec2{10,10},[&](auto wx, auto wz){
                director.place_block(WorldBlockPos{wx,10,wz},BlockType::COBBLESTONE);
            });
        }
//    });

    if (director.player_crossed_chunk_boundary()){
        director.discover_candidates(mesh_enqueue_delay_bench, gen_enqueue_delay_bencher);
    }

    submit_gen_jobs(maxGenJobsPerFrame);
    upload_gen_results(maxGenUploadsPerFrame);


    evict_meshes_outside_radius(director.MESH_CULL_DIST());
    refresh_visible_chunks();
    classify_visible_chunks();

    
    process_lighting_updates();

    submit_mesh_jobs(maxMeshJobsPerFrame);
    upload_mesh_results(maxMeshUploadsPerFrame);

    //NOTE: We must perform mesh sorting AFTER mesh upload this frame,
    // otherwise the size of sorted_keys diverges from the mesh_lists.
    director.handle_mesh_sorting(rend,player_cam.pos);
    count_states();
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

bool Engine::enqueue_gen(WorldChunkCoord candidate_coord){
    auto& genQ = world.generators.job_queue;
    bool success = genQ.try_emplace(
        ChunkBenchContext{gen_work_bencher,gen_job_queue_idle_bencher, gen_res_queue_idle_bencher},
        world.worldgen_epoch,
        candidate_coord, 
        world.active_cfg
    );
    if (success){
        gen_enqueue_delay_bencher.bench_end(candidate_coord);
        gen_rtt_bencher.bench_start(candidate_coord,world.worldgen_epoch);
        gen_job_queue_idle_bencher.bench_start(candidate_coord,world.worldgen_epoch);
        auto* entry = world.chunkMap.entries.try_get(candidate_coord);
        if (!entry){
            entry = world.make_chunk_entry(candidate_coord);
        }
        director.mark_gen_enqueue(entry);
        gen_jobs_this_frame++;
    }else{
        // did not upload (mutex contention)
    }
    return success;
}

void Engine::submit_gen_jobs(i64 maxJobs){
    profiler.bench_start("enqueueGen");
    for (const auto& candidate_coord: director.find_gen_jobs(maxJobs)){
        enqueue_gen(candidate_coord);
    }
    profiler.bench_end("enqueueGen");
}


void Engine::submit_mesh_jobs(i64 maxJobs){
    profiler.bench_start("enqueueMesh");
    auto candidates = director.find_mesh_jobs(maxJobs);

    i64 count = 0;
    for (const auto& candidate_coord: candidates){
//        if (!is_chunk_in_frustum(player_cam.getCullFrustum(),coord)){
//            continue;
//        }
        auto& meshQ = rend.meshers.job_queue;
        auto* entry = world.chunkMap.entries.at(candidate_coord);

        assert(entry);
        if (director.qualifies_for_mesh_enqueue(entry)){
            // BlockShape 
            static_assert(BlockShape::CUBE == static_cast<BlockShape>(0));
            static_assert(BlockShape::CROSS == static_cast<BlockShape>(1));
            bool success = meshQ.try_emplace(
                ChunkBenchContext{mesh_work_bencher,mesh_job_queue_idle_bencher, mesh_res_queue_idle_bencher},
                candidate_coord,
                rend.atlas_list,
                &world.chunkMap,
                entry
            );
            if (success){
                mesh_rtt_bencher.bench_start(candidate_coord,entry->mesh.target);
                mesh_job_queue_idle_bencher.bench_start(candidate_coord,entry->mesh.target);
                director.mark_mesh_enqueue(entry);
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
    auto candidate_results = drain_mesh_results(rend.meshers.res_queue,maxUploads);
    for (const auto& [candidate_revision, chunk_coord, opaque, blended, cutout] : candidate_results){
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

        if (try_upload_candidate(entry->mesh, candidate_revision)){
            log_to_chunk(LogType::mesh_uploads,chunk_coord, "Mesh upload success ({}->{})",entry->mesh.loaded,candidate_revision);
            log_to_chunk(LogType::mesh_uploads,chunk_coord, "OPQ:{},TRN:{},CUT:{}",opaque.vertices.size(),blended.vertices.size(),cutout.vertices.size());
            //log_to_chunk(chunk_coord,"opaque new: {}",opaque.vertices.size());
            //log_to_chunk(chunk_coord,"transp new: {}",blended.vertices.size());
            if (rend.opaque_chunk_meshes.contains(chunk_coord)){
            //    log_to_chunk(chunk_coord,"opaque before: {}",rend.opaqueChunkMeshes.at(chunk_coord));
            }
            opaque.vertices.size() > 0   ? rend.uploadMesh(chunk_coord, std::move(opaque)) : void();
            blended.vertices.size() > 0  ? rend.uploadMesh(chunk_coord, std::move(blended)) : void();
            cutout.vertices.size() > 0   ? rend.uploadMesh(chunk_coord, std::move(cutout)) : void();
        }else{
                log_fail_upload(std::format("Candidate rev ({}) is older than loaded ({}).",
                                candidate_revision,entry->mesh.loaded));
                continue;
        }
        this->chunksMeshed++;
        auto ttm = mesh_rtt_bencher.bench_end(chunk_coord,candidate_revision);
        mesh_res_queue_idle_bencher.bench_end(chunk_coord, candidate_revision);
       // log_to_chunk(chunk_coord,"time to mesh: {:2.4f}ms",ttm);
        count++;
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
    auto genResults = drain_gen_results(world.generators.res_queue,maxUploads);
    for (auto& newGen : genResults){
        const auto& chunk_coord = newGen.chunk_coord;
        const auto& candidate_revision = newGen.genRevisionID;
        auto log_fail_upload = [&](std::string_view str){
            log_to_chunk(chunk_coord, "Gen upload rejected: {}.",str);
        };
        auto* entry = world.chunkMap.entries.try_get(chunk_coord);
        if (!entry){
            log_fail_upload("Entry does not exist????");
            continue;
        }

        if (try_upload_candidate(entry->gen, candidate_revision)){
            log_to_chunk(LogType::gen_uploads,chunk_coord, "gen upload success ({}->{})",entry->gen.loaded,candidate_revision);
            director.upload_generated_chunk(entry, std::move(newGen));
            entry->gen.loaded = candidate_revision;
        }else{
            log_fail_upload(std::format("Candidate rev ({}) is older than loaded ({}).",
                            candidate_revision,entry->gen.loaded));
        }
        auto ttg = gen_rtt_bencher.bench_end(chunk_coord, candidate_revision);
        gen_res_queue_idle_bencher.bench_end(chunk_coord, candidate_revision);
//        log_to_chunk(chunk_coord,"time to gen: {:2.4f}ms",ttg);
    }
    profiler.bench_end("drainGen");
    gen_res_this_frame = genResults.size();
}

void Engine::draw_chunk_boundaries(Camera& cam, RenderTargetView target ){
    rend.draw_debugChunks_to(player_cam,this,screen_view());
    rend.draw_3DLines_to(cam,rend.dbg_rend.chunk_outlines,target);
}
void Engine::draw_scene() {
    profiler.bench_start("01_draw");
    player_cam.aspectRatio = win.aspect();
    rend.debug.reset_per_frame();


    rend.clear_to(screen_view());
    rend.draw_to(player_cam, screen_view(), &profiler);


    if( DebugOption::enable_drone_cam){
        rend.clear_to(secondaryView());
        rend.draw_to(drone_cam, secondaryView(), &profiler);
        if (DebugOption::enable_3d_debug_visuals){
            // Drone cam sees players' frustum lines 
            rend.draw_3DLines_to(drone_cam, rend.player_cam_frustum_lines, secondaryView());
        }
    }
    
    if( DebugOption::enable_3d_debug_visuals    &&
        (DebugOption::fill_all_boundaries           ||
        DebugOption::fill_neighbour_boundaries      || 
        DebugOption::outline_all_boundaries         || 
        DebugOption::outline_neighbour_boundaries)
    ){
        draw_chunk_boundaries(player_cam, screen_view());
        draw_chunk_boundaries(drone_cam, secondaryView());
   }



    static bool first_draw = true;
    if (first_draw) {
        LOG_DEBUG("Finished first draw");
        first_draw = false;
    }
    profiler.bench_end("01_draw");
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
            director.mark_mesh_dirty(&entry, "unmeshed all chunks");
        }
    );
    rend.opaque_chunk_meshes.clear();
    rend.blended_chunk_meshes.clear();
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

void Engine::update(timer::duration dt){
    tick_gap_accumulator += std::min(dt, maxGapContributionPerFrame);

    auto ticks_this_frame {0uz};
    while (tick_gap_accumulator > msPerTick && ticks_this_frame < max_ticks_per_frame ){
        per_tick_update();
        tick_gap_accumulator-=msPerTick;
        ticks_this_frame++;
    }

    //std::println("{} ticks this frame.",ticks_this_frame);
}
void Engine::setup() {
    for (auto& v: block_defs){
        std::println("{}",v);
    }
#ifdef ENABLE_CPPTRACE
    cpptrace::register_terminate_handler(); // gives us stack traces in std::terminate handler
#endif
    set_debug_params();                 

    program_epoch_ns = get_current_ns();



    win.set_callbacks(static_cast<void*>(this));
    LOG_DEBUG("Finished setting window callbacks.");


    profiler.init<std::string_view>({
        "01_draw",
        "02_rendinit",
        "03_opaque",
        "04_cutout",
        "05_blended",
        "frame",
        "input",
        "update",
        "enqueueGen",
        "drainGen",
        "enqueueMesh",
        "drainMesh",
        "render",
    });
    LOG_DEBUG("Finished Profiler setup.");

    player_cam.is_main_camera=true;
    drone_cam.vertical_fov = 50.0f;
    director.init(player_cam.pos);
    LOG_DEBUG("Finished Camera setup.");


    ui.init(win.ptr);
    LOG_DEBUG("Finished UI setup.");

    world.setup();
    LOG_DEBUG("Finished World setup.");

    rend.update_debug_uniforms();
    // enqueue the starting chunks
    director.discover_candidates(mesh_enqueue_delay_bench, gen_enqueue_delay_bencher);

    world.worldgen_epoch++;
    global_logger.epoch = Logger::clock::now();



    //submit_gen_jobs(maxGenJobsPerFrame);
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
        } else if(chunk_updates_paused){
            chunk_updates_paused= false;
        }else if(mouse_mode){
            mouse_mode = false;
        } else{
            win.scheduleClose();
            return;
        }
    }

    if(input.just_pressed(KEY_GRAVE_ACCENT)){
        ui.is_ui_expanded = !ui.is_ui_expanded;
    }
    if(input.just_pressed({.shift=true, .super=true }, KEY_C)){
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
    if(input.just_pressed(KEY_M)){
        mouse_mode = !mouse_mode;
        if (!mouse_mode){
            win.captureCursor();
            player_cam.enableMousePanning();
        }else{ 
            win.freeCursor();
            player_cam.disableMousePanning();
        }
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
    if (paused) return; // WARNING: Anything below here is ignored during paused frames
    if(input.just_pressed(KEY_L)){
        DebugOption::show_lighting_system = !DebugOption::show_lighting_system;
        rend.update_debug_uniforms();
    }


    if (input.mousepos != input.prevmousepos) {
        const glm::vec2 diff = input.prevmousepos - input.mousepos;
        player_cam.rotateByMouse(diff, profiler.dt_s);
    }

    // NOTE:  DEBUG TOGGLES
    if(input.just_pressed(KEY_T)){
        auto coord = director.cur_chunk_pos;
        auto* entry = world.chunkMap.entries.try_get(coord);
        if (entry){
            auto& pwq = *(world.chunkMap.get_or_emplace_pwq(coord));
            auto cur_block_pos = director.cur_block_pos;
            auto chunk_local = toChunkBlockPos(cur_block_pos);
            while (entry->block_data.at(chunk_local) == BlockType::AIR){
                cur_block_pos.y--;
                chunk_local = toChunkBlockPos(cur_block_pos);
            }
            auto writer = BlockWriter{
                entry->block_data.view(),
                pwq,
                coord
            };
            writer.try_place(
                OverwritePolicy::OnlyAir,
                cur_block_pos,
                BlockType::DBG_OUTLINE
            );
            cur_block_pos.y++;
            features::regular_oak_tree.place(
                cur_block_pos,
                1.0f,
                writer
            );
            director.mark_mesh_dirty(entry, "Placed tree");
            director.mark_lighting_dirty(entry, "Placed tree");
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
    if(input.just_pressed(KEY_B)){
        dbg_modify_chunks = !dbg_modify_chunks;
    }
    if(input.just_pressed({.alt=true},KEY_W) ){
        rend.debug.wireframe = !rend.debug.wireframe;
    }
    if(input.just_pressed(KEY_H)){
        DebugOption::show_debug_ui = !DebugOption::show_debug_ui;
    }
    if(input.just_pressed(KEY_C)){
        DebugOption::fill_neighbour_boundaries = !DebugOption::fill_neighbour_boundaries;
        DebugOption::outline_neighbour_boundaries = !DebugOption::outline_neighbour_boundaries;
    }

    if(input.just_pressed(KEY_R)){
        if (input.mods.shift){
            LOG_DEBUG("regenerating world...");
            regenerate_world();
            return;
        }else{
            remesh_world();
            return;
        }
	}

    // NOTE: MOVEMENT
    if(input.is_down(KeyModifiers{.shift=true})){
        std::println("DOWN");
        player_cam.moveSpeed = Camera::SPRINT_MOVESPEED;
        player_cam.keyboard_sensitivity= Camera::SPRINT_KEYBOARD_SENSITVITY;
    }else if(input.is_down(KEY_LEFT_CONTROL)){
        player_cam.moveSpeed = Camera::WALK_MOVESPEED;
    }else{
        player_cam.moveSpeed = Camera::BASE_MOVESPEED;
        player_cam.keyboard_sensitivity= Camera::BASE_KEYBOARD_SENSITIVITY;
    }

    if(input.is_down(KeyModifiers::any(), KEY_W)){
		player_cam.move(Direction::FORWARD, profiler.dt_s);
	}
    if(input.is_down(KeyModifiers::any(), KEY_S)){
		player_cam.move(Direction::BACKWARD, profiler.dt_s);
	}
    if(input.is_down(KeyModifiers::any(), KEY_A)){
		player_cam.move(Direction::LEFT, profiler.dt_s);
	}
    if(input.is_down(KeyModifiers::any(), KEY_D)){
		player_cam.move(Direction::RIGHT, profiler.dt_s);
	}
    if(input.is_down(KeyModifiers::any(), KEY_SPACE)){
		player_cam.move(Direction::UP, profiler.dt_s);
	    drone_cam.move(Direction::UP, profiler.dt_s);
	}
    if(input.is_down(KeyModifiers::any(), KEY_E)){
		player_cam.move(Direction::UP, profiler.dt_s);
	}
    if(input.is_down(KeyModifiers::any(), KEY_Q)){
		player_cam.move(Direction::DOWN, profiler.dt_s);
	}

    if(input.is_down(KeyModifiers::any(), KEY_LEFT)){
		player_cam.rotate(Direction::LEFT, profiler.dt_s);
	}
    if(input.is_down(KeyModifiers::any(), KEY_RIGHT)){
		player_cam.rotate(Direction::RIGHT, profiler.dt_s);
	}
    if(input.is_down(KeyModifiers::any(), KEY_UP)){
		player_cam.rotate(Direction::UP, profiler.dt_s);
	}
    if(input.is_down(KeyModifiers::any(), KEY_DOWN)){
		player_cam.rotate(Direction::DOWN, profiler.dt_s);
	}

    if (dbg_modify_chunks){
        dbg_modify_chunks = false;
        auto cur_chunk = toWorldChunkCoord(player_cam.pos);
        director.mark_neighbour_meshes_dirty(cur_chunk, "test");
        world.chunkMap.entries.if_contains(
            cur_chunk,
            [&](ChunkEntry& entry){
                director.mark_mesh_dirty(&entry, "dbg modified chunks");
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
                director.mark_mesh_dirty(&entry, "forced dirty");
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
void Engine::remesh_world(){
    director.ready_for_mesh.clear();
    director.ready_for_lighting.clear();
    rend.opaque_chunk_meshes.clear();
    rend.blended_chunk_meshes.clear();
    rend.cutout_chunk_meshes.clear();
    for (auto& [key, entry]: world.chunkMap.entries){
        director.mark_mesh_dirty(&entry, "remeshed world");
        director.mark_lighting_dirty(&entry, "remeshed world");
    }
}
void Engine::regenerate_world(){
    // 1. clear job queues: (no more inputs to the threads)
    rend.meshers.job_queue.clear();
    world.generators.job_queue.clear();
    {
        auto lock = per_chunk_log.lock_guard();
        per_chunk_log.clear();
    }

    director.ready_for_gen.clear();
    director.ready_for_mesh.clear();
    
    // now, no more inputs into the threads are possible: 
    // they only have the work which they have accepted.
    // From here, we continuously pop off the queue until all the threads finish.
    // But how do we know that all the threads are done?
    
    // ...

    // _. clear the mesh lists
    rend.opaque_chunk_meshes.clear();
    rend.blended_chunk_meshes.clear();
    rend.cutout_chunk_meshes.clear();

    // _. clear the chunk stores
    world.regenerate();
}
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

    n_gen_pending ={};
    n_gen_ready_for_enqueue={};
    n_gen_on_queue               ={};
    n_gen_done                   ={};

    n_mesh_pending   ={};
    n_mesh_ready_for_enqueue     ={};
    n_mesh_on_queue              ={};
    n_mesh_done                  ={};
    n_light_pending   ={};
    n_light_ready_for_enqueue     ={};
    n_light_on_queue              ={};
    n_light_done                  ={};
    for (const auto& [key, val]: world.chunkMap.entries){
        switch(val.lighting_pipeline_state()){
            // TODO: add
            case PipelineState::pending: n_light_pending++; break;
            case PipelineState::on_queue: n_light_on_queue++; break;
            case PipelineState::done: n_light_done++; break;
            case PipelineState::ready_for_enqueue: n_light_ready_for_enqueue++; break;
        }
        switch(val.gen_pipeline_state()){
            // TODO: add
            case PipelineState::pending: n_gen_pending++; break;
            case PipelineState::on_queue: n_gen_on_queue++; break;
            case PipelineState::done: n_gen_done++; break;
            case PipelineState::ready_for_enqueue: n_gen_ready_for_enqueue++; break;
        }

        switch(val.mesh_pipeline_state()){
            case PipelineState::pending : n_mesh_pending++; break;
            case PipelineState::ready_for_enqueue  : n_mesh_ready_for_enqueue  ++; break;
            case PipelineState::on_queue           : n_mesh_on_queue           ++; break;
            case PipelineState::done               : n_mesh_done               ++; break;
        }
    }

};
