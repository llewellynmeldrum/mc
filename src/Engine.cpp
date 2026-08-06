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
#include "SkyboxState.hpp"
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


void Engine::per_tick_update(){
    update_player_cam(player_cam);
    update_drone_cam(drone_cam, player_cam.pos);
    world.tick();
    tick_count++;
}
void Engine::loop(){
    auto dt = timer::milliseconds(16);
    while (!win.shouldClose()) {
        t_frame_start = timer::now();
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
            if (!tick_updates_paused){
                perform_tick_updates(dt);
            }
        }
        rend.dbg_rend.update(player_cam,this);
    
        if (DebugOption::show_debug_ui){
            rend.update_player_cam_frustum_lines(this);
        }
        ui.update();


        auto const& sky_cfg = get_skybox_cfg();
        rend.per_frame_update(player_cam, sky_cfg.make_skybox());
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
        dt = timer::now() - t_frame_start;
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





void Engine::handle_chunk_scheduling() {
    profiler.bench_start("update");
    director.start_frame(player_cam.pos);


    if (director.player_crossed_chunk_boundary()){
        director.discover_candidates();
    }

    enqueue_jobs<JobType::Gen>();
    upload_results<JobType::Gen>();


    enqueue_jobs<JobType::Light>();
    upload_results<JobType::Light>();

    evict_meshes_outside_radius(director.MESH_CULL_DIST()*1.5f);
    refresh_visible_chunks();
    classify_visible_chunks();

    enqueue_jobs<JobType::Mesh>();
    upload_results<JobType::Mesh>();

    //NOTE: We must perform mesh sorting AFTER mesh upload this frame,
    // otherwise the size of sorted_keys diverges from the mesh_lists.
    director.handle_mesh_sorting(rend,player_cam.pos);
    count_states();
    director.end_frame();
    profiler.bench_end("update");
}


void Engine::update_player_cam(Camera& player_cam){
}

void Engine::update_drone_cam(Camera& drone_cam, WorldFloatPos target_pos, f32 fly_height){
    auto follow_pos = WorldFloatPos{player_cam.pos.raw()+glm::vec3{0,100,0}};
    drone_cam.set_pos_ori(follow_pos, -89.0, 0.0);
}
LightingJob Engine::make_light_job(WorldChunkCoord coord){
    auto* entry = world.chunkMap.entries.try_get(coord);
    assert(entry);
    return {
        make_bench_ctx<JobType::Light>(),
        coord,
        &director.chunk_map,
        entry
    };
}

GenJob Engine::make_gen_job(WorldChunkCoord coord){
    return {
        make_bench_ctx<JobType::Gen>(),
        coord, 
        world.worldgen_epoch,
        world.active_cfg
    };
}
MeshJob Engine::make_mesh_job(WorldChunkCoord coord){
    auto* entry = world.chunkMap.entries.try_get(coord);
    assert(entry);

    return {
        make_bench_ctx<JobType::Mesh>(),
        coord,
        rend.atlas_list,
        &world.chunkMap,
        entry
    };
}



bool Engine::upload_light(LightingResult&& res){
    auto [coord, rev, lights] = res;
    auto log_fail_upload = [&](std::string_view str){
        log_to_chunk(coord, "lighting upload rejected: {}.",str);
    };
    auto* entry = world.chunkMap.entries.try_get(coord);
    if (!entry){
        // NOTE: this should probably be an assert.
        log_fail_upload("Entry does not exist????");
        return false;
    }

    if (try_upload_candidate_revision(entry->lighting, rev)){
        log_to_chunk(LogType::lighting_state,coord, "lighting upload success ({}->{})",entry->lighting.loaded,rev);
        director.upload_light_result(entry, std::move(res));
    }else{
        entry->lighting.drop_inflight();
        log_fail_upload(std::format("Candidate rev ({}) is older than loaded ({}).",
                        rev,entry->lighting.loaded));
        return false;
    }
    this->chunksLit++;
    return true;
}

bool Engine::upload_mesh(MeshResult&& res){
    const auto& [candidate_revision, chunk_coord, opaque, blended, cutout] = res;
    auto log_fail_upload = [&](std::string_view str){
        log_to_chunk(chunk_coord, "Mesh upload rejected: {}.",str);
    };
    auto* entry = world.chunkMap.entries.try_get(chunk_coord);
    if (!entry){
        // NOTE: this should probably be an assert.
        log_fail_upload("Entry does not exist????");
        return false;
    }

    if (entry->mesh.candidate_is_acceptable(candidate_revision)){
        log_to_chunk(LogType::mesh_uploads,chunk_coord, "Mesh upload success ({}->{})",entry->mesh.loaded,candidate_revision);
        director.upload_mesh_result(rend, entry, std::move(res));
        if (candidate_revision == RevisionState::FIRST_JOB){
            entry->meta.m_upload_time = timer::now();
        }
        entry->mesh.complete_inflight(candidate_revision);
    }else{
        entry->mesh.drop_inflight();
        log_fail_upload(std::format("Candidate rev ({}) is older than loaded ({}).",
                        candidate_revision,entry->mesh.loaded));
        return false;
    }
    this->chunksMeshed++;
//    auto ttm = mesh_rtt_bencher.bench_end(chunk_coord,candidate_revision);
    mesh_res_queue_idle_bencher.bench_end(chunk_coord, candidate_revision);
   // log_to_chunk(chunk_coord,"time to mesh: {:2.4f}ms",ttm);
    return true;
}

bool Engine::upload_gen(GenResult&& res){
    const auto& chunk_coord = res.coord;
    const auto& candidate_revision = res.rev;

    auto log_fail_upload = [&](std::string_view str){
        log_to_chunk(chunk_coord, "Gen upload rejected: {}.",str);
    };
    auto* entry = world.chunkMap.entries.try_get(chunk_coord);
    if (!entry){
        log_fail_upload("Entry does not exist????");
        return false;
    }

    if (try_upload_candidate_revision(entry->gen, candidate_revision)){
        log_to_chunk(LogType::gen_uploads,chunk_coord, "gen upload success ({}->{})",entry->gen.loaded,candidate_revision);
        director.upload_gen_result(entry, std::move(res));
        entry->gen.loaded = candidate_revision;
    }else{
        entry->gen.drop_inflight();
        log_fail_upload(std::format("Candidate rev ({}) is older than loaded ({}).",
                        candidate_revision,entry->gen.loaded));
        return false;
    }
//    auto ttg = gen_rtt_bencher.bench_end(chunk_coord, candidate_revision);
    gen_res_queue_idle_bencher.bench_end(chunk_coord, candidate_revision);
//    log_to_chunk(chunk_coord,"time to gen: {:2.4f}ms",ttg);
    return true;
}

void Engine::draw_chunk_boundaries(Camera& cam, RenderTargetView target ){
    rend.draw_debugChunks_to(player_cam,this,screen_view());
    rend.draw_3DLines_to(cam,rend.dbg_rend.chunk_outlines,target);
}
void Engine::draw_scene() {
    profiler.bench_start("01_draw");
    player_cam.aspectRatio = win.aspect(); // this probably has a better home
    rend.debug.reset_per_frame();


    rend.draw_skybox(screen_view());
    rend.draw_to(player_cam, screen_view(), &profiler);


    if( DebugOption::enable_drone_cam){
        rend.draw_skybox(secondaryView());
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

void Engine::perform_tick_updates(timer::duration dt){
    tick_gap_accumulator += std::min(dt, maxGapContributionPerFrame);
//    std::println("tick_gap_accum: {}",tick_gap_accumulator);
//    std::println("dt: {}",dt);

    auto ticks_this_frame {0uz};
    while (tick_gap_accumulator > msPerTick() && ticks_this_frame < max_ticks_per_frame ){
        per_tick_update();
        tick_gap_accumulator-=msPerTick();
        ticks_this_frame++;
    }

}

void Engine::setup(bool setup_logging) {
    auto log_stage = [setup_logging](auto stage_msg){
        if (setup_logging){
            LOG_DEBUG("Finished {}", stage_msg);
        }
    };
    if (setup_logging){
        for (auto& v: block_defs){
            std::println("{}",v);
        }
    }
#ifdef ENABLE_CPPTRACE
    cpptrace::register_terminate_handler(); // gives us stack traces in std::terminate handler
#endif
    set_debug_params();                 

    program_epoch_ns = get_current_ns();



    win.set_callbacks(static_cast<void*>(this));
    log_stage("Finished setting window callbacks.");


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
        "findJobsGen",

        "enqueueLight",
        "findJobsLight",
        "drainLight",

        "enqueueMesh",
        "findJobsMesh",
        "drainMesh",

        "render",
    });
    log_stage("Finished Profiler setup.");

    player_cam.is_main_camera=true;
    drone_cam.vertical_fov = 50.0f;
    director.setup(player_cam.pos);
    log_stage("Finished Camera setup.");


    ui.init(win.ptr);
    log_stage("Finished UI setup.");


    rend.update_debug_uniforms();
    auto const& sky_cfg = get_skybox_cfg();
    rend.per_frame_update(player_cam, sky_cfg.make_skybox());
    // TODO: fog distance should be the average of the chunk distances?
    auto fog_dist = director.RENDER_DIST * ChunkInfo::HOZ_EXTENT;
    rend.set_fog_color(glm::vec3(0.0f));
    rend.set_fog_start(fog_dist*0.85);
    rend.set_fog_end(fog_dist);
    // enqueue the starting chunks
    world.worldgen_epoch++;
    director.discover_candidates();

    global_logger.epoch = Logger::clock::now();
//    constexpr auto sq= [](auto n){
//        return n*n;
//    };
    [[maybe_unused]] constexpr auto n_chunks = 100;
    //auto n_chunks = sq((director.GENERATION_DIST+1)*2);
    //force_load_chunks(n_chunks);


    const auto start_pos = player_cam.pos;
    const auto start_pitch = player_cam.pitch;
    const auto start_yaw = player_cam.yaw;

//    bake_n_chunks(n_chunks);
    player_cam.set_pos_ori(start_pos,start_pitch,start_yaw); //{-0.509,+383.622,+12.423}, -89.000,+171.000
//    for (int i = 0; i<1; i++){
//        force_load_chunks(n_chunks);
//        regenerate_world();
//    }
    per_tick_update();


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
SkyboxConfig Engine::get_skybox_cfg(){
    if (DebugOption::skybox_ui_override){
        return ui.skybox_cfg;
    }
    return default_skybox_cfg(tick_count);
}
void Engine::handle_input(){
    if (input.just_pressed(KEY_ESCAPE)){
        if (baking_starting_chunks){
            std::println("\n\nCancelled baking of starting chunks!\n\n");
            baking_starting_chunks = false;
            return;
        }
        if (paused){
            // unpause
            paused = false; 
            return;
        } 
        if(chunk_updates_paused){
            chunk_updates_paused= false;
            return;
        }
        if(mouse_mode){
            mouse_mode = false;
            return;
        } 

        win.scheduleClose();
        return;
    }

    if(input.just_pressed(KEY_GRAVE_ACCENT)){
        ui.is_ui_expanded = !ui.is_ui_expanded;
    }
    if (baking_starting_chunks){
		player_cam.rotate(Direction::LEFT, profiler.dt_s * 3.0f);
        return;
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
    if(input.is_down( KEY_C)){
        player_cam.vertical_fov = DebugOption::player_cam_vfov_zoom;
        player_cam.cached_frustum.invalidate();
        player_cam.cached_viewMatrix.invalidate();
        log_to_ui("C is down");
    }else{
        log_to_ui("C is up");
        player_cam.vertical_fov = DebugOption::player_cam_vfov;
        player_cam.cached_frustum.invalidate();
        player_cam.cached_viewMatrix.invalidate();
    }
    if (input.scroll.y != input.prevscroll.y){
        static constexpr f32 base = 1.1f;
        const f32 exponent = -1 * (input.scroll.y * drone_cam.zoom_sens * profiler.dt_s);
        f32 zoom = pow(base,exponent);
        drone_cam.ortho_zoom *= zoom;
        DebugOption::player_cam_vfov_zoom *= zoom;
        std::println("{:v}",DebugOption::player_cam_vfov_zoom);
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

    if(input.just_pressed(KeyModifiers{.shift=true},KEY_T)){
        tick_updates_paused = !tick_updates_paused;
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
        DebugOption::enable_3d_debug_visuals = !DebugOption::enable_3d_debug_visuals;
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
    director.ready_for_light.clear();
    rend.opaque_chunk_meshes.clear();
    rend.blended_chunk_meshes.clear();
    rend.cutout_chunk_meshes.clear();
    for (auto& [key, entry]: world.chunkMap.entries){
        director.mark_mesh_dirty(&entry, "remeshed world");
        director.mark_lighting_dirty(&entry, "remeshed world");
    }
}
void Engine::regenerate_world(){
    

    discard_results<JobType::Gen>();
    discard_results<JobType::Light>();
    discard_results<JobType::Mesh>();
    director.ready_for_enqueue<JobType::Gen>().clear();
    director.ready_for_enqueue<JobType::Light>().clear();
    director.ready_for_enqueue<JobType::Mesh>().clear();

    {
        auto lock = per_chunk_log.lock_guard();
        per_chunk_log.clear();
    }


    

    get_pf_accums<JobType::Gen>().reset();
    get_pf_accums<JobType::Light>().reset();
    get_pf_accums<JobType::Mesh>().reset();

    // _. clear the mesh lists
    rend.opaque_chunk_meshes.clear();
    rend.blended_chunk_meshes.clear();
    rend.cutout_chunk_meshes.clear();
    rend.sorted_opaque_coords.clear();
    rend.sorted_blended_coords.clear();
    rend.sorted_cutout_coords.clear();

    // _. clear the chunk stores
//    world.regenerate();
    world.chunkMap.clear();
//        worldgen_epoch++; // all new genjobs will have targetRevision incremented 
//        LOG_DEBUG("{}->{}",active_cfg.cont_cfg.seed_offset, editable_cfg.world_seed);
//        GenConfig::copy(active_cfg,editable_cfg);
}
void Engine::count_states(){
    update_state_counters<JobType::Gen>();
    update_state_counters<JobType::Light>();
    update_state_counters<JobType::Mesh>();
};
void Engine::bake_n_chunks(i32 count){
    std::println("===============================================================================");
    std::println("Baking {} starting chunks...",count);
    baking_starting_chunks = true;
//    assert(count > 32, "Small value can cause the queue to be jammed.");

    auto t0 = timer::now();
    timer::duration discovery_duration;
    timer::duration gen_duration;
    timer::duration light_duration;
    timer::duration mesh_duration;
    i32 gen_count = count;
    i32 light_count = gen_count * .75; // not all chunks which are generated can be lit (edges)
    i32 mesh_count = gen_count * .75; // not all chunks which are generated can be lit (edges)
    // sleeps but still updates the ui.
    
    gen_bake_progress.set_total(count);
    light_bake_progress.set_total(light_count);
    mesh_bake_progress.set_total(mesh_count);
    auto discovery_t0 = timer::now();
    while(director.ready_for_enqueue<JobType::Gen>().size() < count){
        std::println(stderr, "size:{} (count:{})",director.ready_for_enqueue<JobType::Gen>().size(),count);
        director.discover_candidates();
    }
    discovery_duration = timer::now() - discovery_t0;


    auto gen_t0 = timer::now();
    gen_bake_progress.print_name();
    render_busy_wait_until<JobType::Gen>(gen_count, [&](){
        auto n_done = get_state_counters<JobType::Gen>().n_done;
        gen_bake_progress.update(n_done);
        return  n_done >= gen_count;
    });
    if (!baking_starting_chunks) return;
    gen_duration = timer::now() - gen_t0;



    auto light_t0 = timer::now();
    light_bake_progress.print_name();
    render_busy_wait_until<JobType::Light>(light_count,[&]{
        auto n_done = get_state_counters<JobType::Light>().n_done;
        light_bake_progress.update(n_done);
        return  n_done >= light_count;
    });
    if (!baking_starting_chunks) return;
    light_duration = timer::now() - light_t0;


    auto mesh_t0= timer::now();
    mesh_bake_progress.print_name();
    render_busy_wait_until<JobType::Mesh>(mesh_count,[&]{
        auto n_done = get_state_counters<JobType::Mesh>().n_done;
        mesh_bake_progress.update(n_done);
        return  n_done >= mesh_count;
    });
    if (!baking_starting_chunks) return;
    mesh_duration = timer::now() - mesh_t0;

    auto duration = timer::now() - t0;
    auto fmt_var = [](auto v){
        if (v<1.0f){
            return std::format("{:>18}",std::format("{:4.2f}ms ({:4.2f}us)",v,v*1000.0f));
        }else{
            return std::format("{:>9}",std::format("{:4.2f}ms",v));
        }
    };
    auto ms = [&](auto dur){ return fmt_var(timer::to_milliseconds(dur)); };
    auto ms_per = [&](auto dur, auto n){ return fmt_var(timer::to_milliseconds(dur)/n); };

    auto gen_pct = 100.0f * timer::to_milliseconds(gen_duration) / timer::to_milliseconds(duration);
    auto lit_pct = 100.0f * timer::to_milliseconds(light_duration) / timer::to_milliseconds(duration);
    auto mes_pct = 100.0f * timer::to_milliseconds(mesh_duration) / timer::to_milliseconds(duration);
    std::println("===============================================================================");
    std::println("Finished baking {} starting chunks, took {} ,{}/chunk",
    count, ms(duration),ms_per(duration,count));
    std::println("Generated {}, took {}({:4.1f}%) ,{}/chunk", gen_count,   ms(gen_duration),  gen_pct,  ms_per(gen_duration,count));
    std::println("Lit       {}, took {}({:4.1f}%) ,{}/chunk", light_count, ms(light_duration),lit_pct,  ms_per(light_duration,count));
    std::println("Meshed    {}, took {}({:4.1f}%) ,{}/chunk", mesh_count,  ms(mesh_duration), mes_pct,  ms_per(mesh_duration,count));
    baking_starting_chunks= false;
}


