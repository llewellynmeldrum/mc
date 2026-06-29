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


    static constexpr std::size_t maxGenUploadsPerFrame= 16;
    static constexpr std::size_t maxGenJobsPerFrame = 16;
    static constexpr std::size_t maxMeshUploadsPerFrame= 64;
    static constexpr std::size_t maxMeshEnqueueAttempts = 64;
    static constexpr std::size_t maxMeshDequeueAttempts = 64;
    static constexpr std::size_t maxMeshJobsPerFrame = 128;



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
    static constexpr std::size_t RB_SZ = 256;
    std::size_t chunksMeshed{0};
    MirroredRingBuf<f32, RB_SZ> rb_genJobsAdded;
    MirroredRingBuf<f32, RB_SZ> rb_genResultsAdded;

    MirroredRingBuf<f32, RB_SZ> rb_meshJobsAdded;
    MirroredRingBuf<f32, RB_SZ> rb_meshResultsAdded;

    std::size_t n_generating{};
    MirroredRingBuf<f32, RB_SZ> rb_generating;

    std::size_t n_meshing{};
    MirroredRingBuf<f32, RB_SZ> rb_meshing;

    std::size_t genJobsThisFrame = 0;
    std::size_t genResultsThisFrame = 0;

    std::size_t meshJobsThisFrame = 0;
    std::size_t meshResultsThisFrame = 0;
    std::size_t n_gen_on_queue               {};
    std::size_t n_gen_done                   {};

    std::size_t n_mesh_awaiting_generation   {};
    std::size_t n_mesh_ready_for_enqueue     {};
    std::size_t n_mesh_on_queue              {};
    std::size_t n_mesh_done                  {};
private:
    std::vector<WorldChunkCoord> findChunksForGeneration(std::size_t maxJobs);
    std::vector<MeshJob> findMeshJobs(std::size_t maxJobs);

    std::size_t enqueueGenerationJobs(std::size_t maxJobs);
    std::size_t enqueueMeshingJobs(std::size_t maxJobs);


    std::size_t drainAndUploadGenResults(std::size_t maxUploads);
    std::size_t drainAndUploadMeshResults(std::size_t maxUploads);
};

// Input::Key definitions (based on glfw3)
