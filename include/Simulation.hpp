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
    Simulation() =default;
    ~Simulation() = default;
    Window   win;
    Timer    time;
    Input    input;
    Camera   cam;
    Renderer rend;
    DebugUI  ui;
    World    world;
    static constexpr std::size_t maxGenUploadsPerFrame= 32;
    static constexpr std::size_t maxGenJobsPerFrame = 32;
    static constexpr std::size_t maxMeshUploadsPerFrame= 16;
    static constexpr std::size_t maxMeshJobsPerFrame = 128;

    std::size_t chunksMeshed{0};

    static constexpr std::size_t RB_SZ = 256;
    MirroredRingBuf<f32, RB_SZ> rb_genJobsAdded;
    MirroredRingBuf<f32, RB_SZ> rb_genResultsAdded;

    MirroredRingBuf<f32, RB_SZ> rb_meshJobsAdded;
    MirroredRingBuf<f32, RB_SZ> rb_meshResultsAdded;

    std::size_t genJobsThisFrame = 0;
    std::size_t genResultsThisFrame = 0;

    std::size_t meshJobsThisFrame = 0;
    std::size_t meshResultsThisFrame = 0;
    

    void unMeshAllChunks();
void unGenerateAllChunks();
    void captureCursor();
    void freeCursor();
    void loop();
    static constexpr i32 RENDER_DIST = 6;
    static constexpr i32 SIMULATION_DIST = 8; //controls chunk gen
    static constexpr u64 WORLD_SEED = 1237;

    void setupContext();
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
