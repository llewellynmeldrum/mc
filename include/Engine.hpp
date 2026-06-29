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

#include "World.hpp"

#include "Concurrency.hpp"
// src/Simulation.cpp
struct Simulation {
  public:
    void setup();
    i32 exit(i32 exit_code);
    Simulation() = default;
    ~Simulation() = default;

    Window   win;
    Profiler profiler;
    Input    input;
    Camera   playerCam;
    Camera   droneCam;
    TextureTarget fixedCamTarget{{0,0},{640,480}};
    Renderer rend;
    DebugUI  ui;
    World    world;

    std::vector<Line3D> lines3d;
    std::vector<Line3D> chunkOutlines;

    static constexpr std::size_t maxGenUploadsPerFrame= 16;
    static constexpr std::size_t maxGenJobsPerFrame = 16;
    static constexpr std::size_t maxMeshUploadsPerFrame= 64;
    static constexpr std::size_t maxMeshEnqueueAttempts = 64;
    static constexpr std::size_t maxMeshDequeueAttempts = 64;
    static constexpr std::size_t maxMeshJobsPerFrame = 128;

    std::size_t chunksMeshed{0};

    static constexpr std::size_t RB_SZ = 256;
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
    
    // include/ChunkEntry.hpp

    std::size_t n_gen_on_queue               {};
    std::size_t n_gen_done                   {};

    std::size_t n_mesh_awaiting_generation   {};
    std::size_t n_mesh_ready_for_enqueue     {};
    std::size_t n_mesh_on_queue              {};
    std::size_t n_mesh_done                  {};

    RenderTargetView screenView();
    RenderTargetView secondaryView();
    auto construct_mesh_job(WorldChunkCoord candidateCoord);
    void count_states();
    bool inPlayerFrustum(WorldChunkCoord coord);
    void cullMeshes(bool enableFrustumCulling);
    void unMeshAllChunks();
    void unGenerateAllChunks();
    void captureCursor();
    void freeCursor();
    void loop();
    static constexpr i32 RENDER_DIST = 6;
    static constexpr glm::ivec3 RENDER_EXTENTS = {RENDER_DIST,4, RENDER_DIST};
    static constexpr i32 MESH_CULL_DIST = RENDER_DIST+2;
    static constexpr i32 SIMULATION_DIST = RENDER_DIST+2; //controls chunk gen
    static constexpr u64 WORLD_SEED = 1237;

    void handleInputs();
    void update();
    void draw();
    void pause();
    void unpause();
    bool isPaused();
    void togglePause();
private:
    bool paused;
    std::vector<WorldChunkCoord> findChunksForGeneration(std::size_t maxJobs);
    std::vector<MeshJob> findMeshJobs(std::size_t maxJobs);

    std::size_t enqueueGenerationJobs(std::size_t maxJobs);
    std::size_t enqueueMeshingJobs(std::size_t maxJobs);


    std::size_t drainAndUploadGenResults(std::size_t maxUploads);
    std::size_t drainAndUploadMeshResults(std::size_t maxUploads);
};

// Input::Key definitions (based on glfw3)
