#include "ChunkStorage.hpp"
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

#include "Engine.hpp"

#include "LM.hpp"
#include "Line3D.hpp"
#include "Profiler.hpp"
#include "Logger.hpp"
#include "Assertion.hpp"

using namespace glm;
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
    if (g_StyleConfig::isEnabled()){
        std::println("Debugger detected, disabling ansi styling.");
    }
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
        "draw()",
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

void Engine::loop() {
    while (!should_close()) {
        profiler.start_frame();
        auto frame_bench = profiler.bench_scope("frame");

        handle_inputs(); 

        if (isPaused() == false){
            update_scene();
            if (rend.debug.showDebugUI){
                ui.update();
            }
        }

        draw_scene(); 
        if (rend.debug.showDebugUI){
            ui.draw(); 
        }

        {
            auto render_bench = profiler.bench_scope("render");
            win.swapBuffers();
        }

        profiler.end_frame();
        loop_count++;
    }
}

bool Engine::should_close() {
    return win.shouldClose();
}

i32 Engine::exit(i32 exit_code) {
    ui.destroy();
    win.terminate();
//    std::println("{}", ScopeTimer::summary());
    std::exit(exit_code);
    return exit_code;
}

void Engine::update_scene() {
    auto update_bench = profiler.bench_scope("update");

    auto center = toWorldChunkCoord(playerCam.pos);

    auto gen_candidates = scheduler.find_gen_ready_chunks(center, gen_extent(), maxGenJobsPerFrame);
    genJobsThisFrame = enqueueGenerationJobs(gen_candidates);
    genResultsThisFrame = drainAndUploadGenResults(maxGenUploadsPerFrame);

    // update meshes
    auto mesh_candidates = scheduler.find_mesh_ready_chunks(center, mesh_extent(), maxMeshJobsPerFrame);
    meshJobsThisFrame = enqueueMeshingJobs(mesh_candidates);
    meshResultsThisFrame = drainAndUploadMeshResults(maxMeshUploadsPerFrame);
    cullMeshes(true);

    update_debug_info();
}


void Engine::update_debug_info(){
    if (!rend.debug.showDebugUI){
        return;
    }
    rb_genJobsAdded.write(genJobsThisFrame);
    rb_genResultsAdded.write(genResultsThisFrame);
    rb_meshJobsAdded.write(meshJobsThisFrame);
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
//    makeCamFrustumLines(playerCam);
//    makeCamFrustumLines(droneCam);
    count_chunk_states();
}

void Engine::cullMeshes(bool enableFrustumCulling){
    // TODO: split this function, its perofmring two roles. (disable and removal)
    //
    // if a mesh is outside of render dist, but still being meshed, cull it.

    auto unload_mesh = [](IndexedMesh& mesh){
        mesh.unload();
    };
    auto load_mesh= [](IndexedMesh& mesh){
        mesh.load();
    };

    auto inside_frustum = [](Engine* sim){
        return [sim](IndexedMesh& mesh){
            return sim->inPlayerFrustum(mesh.chunkCoord);
        };
    };
    auto outside_nocull_range = [&](){
        auto lo = toWorldChunkCoord(playerCam.pos) + ChunkOffset{-mesh_nocull_extent()};
        auto hi = toWorldChunkCoord(playerCam.pos) + ChunkOffset{mesh_nocull_extent()};
        return [lo,hi](IndexedMesh& mesh){
            return !LM::isVecInBounds(mesh.chunkCoord, lo,hi);
        };
    };

    // load meshes inside frustum, unload meshes outside frustum
    auto [op_loaded, op_unloaded] = 
        rend.opaqueChunkMeshes.for_each_if_else(inside_frustum(this), load_mesh, unload_mesh);

    auto [tr_loaded, tr_unloaded] = 
    rend.transparentChunkMeshes.for_each_if_else(inside_frustum(this), load_mesh, unload_mesh);

    // erase all elements which are out of bounds:
    auto op_removed = rend.opaqueChunkMeshes.erase_if(outside_nocull_range);
    auto tr_removed =rend.transparentChunkMeshes.erase_if(outside_nocull_range);

    opaque_unloaded = op_unloaded;
    opaque_loaded = op_loaded;
    opaque_removed = op_removed;

}
void Engine::count_chunk_states(){
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
    n_mesh_done_clean                  ={};
    for (const auto& [key, val]: world.entries){
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
            case MeshState::done               :{
                n_mesh_done++;
                if (val.is_mesh_clean()){
                    n_mesh_done_clean++;
                }
            break;
            }
        }
    }

};







std::size_t Engine::enqueueMeshingJobs(std::span<WorldChunkCoord> candidates){
    profiler.bench_scope("enqueueMesh");

    std::size_t count = 0;
    for (const auto& job: candidates){

        auto& meshQ = rend.meshers.meshJobQueue;
        bool success = meshQ.try_emplace(job);
        if (success){
            world.persistent_mesh_candidates.erase(job.chunkCoord);
            world.entries.if_contains_else(
                job.chunkCoord, 
                [&](ChunkEntry& entry){
                    entry.inflight_mesh_revision = entry.target_mesh_revision;
                    entry.state_transition(mesh_enqueue);
                },
                [&](){
                    LOG_WARN("(mesh_enqueue): Homeless mesh not enqueued because it has no chunk entry");
                }
            );
            count++;
        }
    }

//    if (count>0 || candidates.size()>0){
//        LOG_DEBUG("found {} mesh-ready candidates. {} uploaded.", candidates.size(),count);
//    }
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
        auto* entry = world.entries.try_get(chunkCoord);

        if (entry && entry->is_mesh_on_queue()){
            if (entry->is_candidate_mesh_newer_than_loaded(candidate_revision)){
                if (opaque.vertices.size()>0){
                    rend.uploadMesh(chunkCoord, std::move(opaque));
                } else{
                    log_to_all("Skipped a mesh: O vertices=0!");
                }
                if (transparent.vertices.size()>0){
                    rend.uploadMesh(chunkCoord, std::move(transparent));
                } else{
                    log_to_all("Skipped a mesh: T vertices=0!");
                }
            }else{
                log_to_all("Skipped a mesh: Candidate older than loaded!");
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



void Engine::draw_scene() {
    auto draw_bench = profiler.bench_scope("draw()");
    chunkOutlines.clear();

    world.entries.for_each([&](WorldChunkCoord key, ChunkEntry& entry){
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
            world.entries.if_contains(
                neighbour,
                [&](ChunkEntry& entry){
                    const auto& state = entry.state;
                    auto color = ui.dbg_view.showGenState
                            ? GenDebugOutlineColor(state.gen)
                            : MeshDebugOutlineColor(state.mesh);
                    chunkOutlines.append_range(entry.bounding_box.getLines(color,true));
                });
        }
        world.entries.if_contains(
            cam_chunk,
            [&](ChunkEntry& entry){
                    const auto& state = entry.state;
                    auto color = ui.dbg_view.showGenState
                            ? GenDebugOutlineColor(state.gen)
                            : MeshDebugOutlineColor(state.mesh);
                chunkOutlines.append_range(entry.bounding_box.getLines(color,true));
            });
    }

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


void Engine::freeCursor(){
    win.freeCursor();
    playerCam.disableMousePanning();
}


// BUG: 
// These functions are kinda clarted. Im not clear on whether or not they properly interact with 
// all the queues and stuff. They generate so many edge cases I question their usefulness.
// For example:
// -> How do we inform meshes on the queue that their entries have been nuked?
//      What do we do with those homeless meshes? we must check for and discard them. 
void Engine::unGenerateAllChunks(){
    world.entries.clear();
}
void Engine::unMeshAllChunks(){
    world.entries.for_each([&](WorldChunkCoord coord, ChunkEntry& entry){
        world.mark_mesh_dirty(coord);
    });
    rend.opaqueChunkMeshes.clear();
    rend.transparentChunkMeshes.clear();
}




void Engine::captureCursor(){
    win.captureCursor();
    playerCam.enableMousePanning();
}

void Engine::pause(){
    freeCursor();
    paused=true;
}
void Engine::unpause(){
    captureCursor();
    paused=false;
}
bool Engine::isPaused()const{
    return paused;
}
void Engine::togglePause(){
    if (paused) unpause();
    else pause();
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
bool Engine::inPlayerFrustum(WorldChunkCoord coord){
    return playerCam.getFrustum().isAABBInside(world.getBoundingBox(coord));
}
