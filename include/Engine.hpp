#pragma once

#include "Camera.hpp"
#include "MirroredRingBuf.hpp"
#include "Window.hpp"
#include "Timer.hpp"
#include "Input.hpp"
#include "DebugUI.hpp"
#include "Renderer.hpp"
#include "cppslop.hpp"
#include "Profiler.hpp"
#include "ChunkDirector.hpp"

#include "World.hpp"

#include "Concurrency.hpp"
// src/Simulation.cpp
struct Engine {
  public:
    Engine() : 
        win(), 
        profiler(),
        input(win.ptr), 
        player_cam({+24.315,+154.162,+71.009}, -38.100,-49.203),
        drone_cam(),
        rend(),
        fixedCamTarget({0,0},{640,480}),
        ui(),
        world(),
        director(world.chunkMap)
    {}
    ~Engine() = default;

    void setup();
    void loop();
    i32 exit(i32 exit_code);


    Window   win;
    Profiler profiler;
    Input    input;
    Camera   player_cam;
    Camera   drone_cam;
    Renderer rend;
    TextureTarget fixedCamTarget;
    DebugUI  ui;
    World    world;
    ChunkDirector director;


    void set_debug_params();
    void handle_input();
    void refresh_visible_chunks();

    static constexpr i64 maxGenUploadsPerFrame= 16;
    static constexpr i64 max_gen_discovery_pf= 128;
    static constexpr i64 maxGenJobsPerFrame = 128;
    static constexpr i64 maxMeshUploadsPerFrame= 64;
    static constexpr i64 maxMeshEnqueueAttempts = 64;
    static constexpr i64 maxMeshDequeueAttempts = 64;
    static constexpr i64 maxMeshJobsPerFrame = 128;



    RenderTargetView screen_view();
    RenderTargetView secondaryView();
    auto construct_mesh_job(WorldChunkCoord candidateCoord);
    void count_states();
    bool is_chunk_in_frustum(const Frustum& frustum, WorldChunkCoord coord)const;
    void evict_meshes_outside_radius(i32 radius);
    void unMeshAllChunks();
    void unGenerateAllChunks();

    void update_player_cam(Camera& player_cam);
    void update_drone_cam(Camera& drone_cam, WorldFloatPos target_pos, f32 fly_height=100.0f);

    void update_scene();
    void draw_scene();
    void draw_chunk_boundaries(Camera& cam, RenderTargetView target );
    bool paused{false};
    bool chunk_updates_paused{false};
    bool dbg_modify_chunks{false};
    bool dirty_current_chunk{false};

    // =========
    // Generation
    // =========
    static constexpr i32 RENDER_DIST = 22;
    static constexpr i32 GENERATION_DIST = RENDER_DIST+2; //controls chunk gen
    
    // =========
    // Meshing 
    // =========
    static constexpr i32 MESH_CULL_DIST(){return RENDER_DIST+2;}
    // =========
    // telemetry
    // =========
    static constexpr i64 RB_SZ = 256;
    i64 chunksMeshed{0};
    MirroredRingBuf<f32, RB_SZ> rb_genJobsAdded;
    MirroredRingBuf<f32, RB_SZ> rb_genResultsAdded;

    MirroredRingBuf<f32, RB_SZ> rb_meshJobsAdded;
    MirroredRingBuf<f32, RB_SZ> rb_meshResultsAdded;

    i64 n_generating{};
    MirroredRingBuf<f32, RB_SZ> rb_generating;

    i64 n_meshing{};
    MirroredRingBuf<f32, RB_SZ> rb_meshing;

    i64 n_chunks_discovered = 0;
    i64 gen_jobs_this_frame = 0;
    i64 gen_res_this_frame = 0;

    i64 mesh_jobs_this_frame = 0;
    i64 mesh_results_this_frame = 0;
    i64 n_gen_on_queue               {};
    i64 n_gen_done                   {};

    i64 n_mesh_awaiting_generation   {};
    i64 n_mesh_ready_for_enqueue     {};
    i64 n_mesh_on_queue              {};
    i64 n_mesh_done                  {};
private:
    std::vector<WorldChunkCoord> findChunksForGeneration(i64 maxJobs);
    std::vector<MeshJob> find_mesh_jobs(i64 maxJobs);

    void submit_gen_jobs(i64 maxJobs);
    void submit_mesh_jobs(i64 maxJobs);


    void upload_gen_results(i64 maxUploads);
    void upload_mesh_results(i64 maxUploads);
};

// Input::Key definitions (based on glfw3)
