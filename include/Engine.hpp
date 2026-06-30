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
    Engine() = default;
    ~Engine() = default;
    void setup();
    void loop();
    i32 exit(i32 exit_code);


    Window   win;
    Profiler profiler;
    Input    input;
    Camera   playerCam;
    Camera   droneCam;
    TextureTarget fixedCamTarget{{0,0},{640,480}};
    Renderer rend;
    DebugUI  ui;
    World    world;
    ChunkDirector director;


    static constexpr i64 maxGenUploadsPerFrame= 16;
    static constexpr i64 maxGenJobsPerFrame = 16;
    static constexpr i64 maxMeshUploadsPerFrame= 64;
    static constexpr i64 maxMeshEnqueueAttempts = 64;
    static constexpr i64 maxMeshDequeueAttempts = 64;
    static constexpr i64 maxMeshJobsPerFrame = 128;



    RenderTargetView screenView();
    RenderTargetView secondaryView();
    auto construct_mesh_job(WorldChunkCoord candidateCoord);
    void count_states();
    bool is_chunk_in_frustum(const Frustum& frustum, WorldChunkCoord coord)const;
    void cullMeshes(bool enableFrustumCulling);
    void unMeshAllChunks();
    void unGenerateAllChunks();

    void update_scene();
    void draw_scene();
    void draw_chunk_boundaries(Camera& cam, RenderTargetView target );
    bool paused{false};

    // =========
    // Generation
    // =========
    static constexpr i32 RENDER_DIST = 6;
    static constexpr i32 SIMULATION_DIST = RENDER_DIST+2; //controls chunk gen
    static constexpr u64 WORLD_SEED = 1237;
    
    // =========
    // Meshing 
    // =========
    static constexpr glm::ivec3 RENDER_EXTENTS(){return  {RENDER_DIST,4, RENDER_DIST};}
    static constexpr i32 MESH_CULL_DIST = RENDER_DIST+2;
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

    i64 genJobsThisFrame = 0;
    i64 genResultsThisFrame = 0;

    i64 meshJobsThisFrame = 0;
    i64 meshResultsThisFrame = 0;
    i64 n_gen_on_queue               {};
    i64 n_gen_done                   {};

    i64 n_mesh_awaiting_generation   {};
    i64 n_mesh_ready_for_enqueue     {};
    i64 n_mesh_on_queue              {};
    i64 n_mesh_done                  {};
private:
    std::vector<WorldChunkCoord> findChunksForGeneration(i64 maxJobs);
    std::vector<MeshJob> findMeshJobs(i64 maxJobs);

    i64 enqueueGenerationJobs(i64 maxJobs);
    i64 enqueueMeshingJobs(i64 maxJobs);


    i64 drainAndUploadGenResults(i64 maxUploads);
    i64 drainAndUploadMeshResults(i64 maxUploads);
};

// Input::Key definitions (based on glfw3)
