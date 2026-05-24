#pragma once

#include "Camera.hpp"
#include "Window.hpp"
#include "Timer.hpp"
#include "Input.hpp"
#include "DebugUI.hpp"
#include "Renderer.hpp"

#include "World.hpp"

#include "Concurrency.hpp"
// src/Context.cpp
struct Simulation {
  public:
    static constexpr std::size_t maxMeshesPerFrame = 128;
    Simulation() = default;
    ~Simulation() = default;

    std::size_t chunksMeshed{0};
    Window   win;
    Timer    time;
    Input    input;
    Camera   cam;
    Renderer rend;
    DebugUI  ui;
    World    world;

    void captureCursor();
    void freeCursor();
    void loop();
    static constexpr i32 RENDER_DIST = 6;
    static constexpr i32 SIMULATION_DIST = 8; //controls chunk gen
    static constexpr u64 WORLD_SEED = 1337;

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
    std::vector<glm::ivec3> findChunksForGeneration();
    std::vector<MeshJob> findMeshJobs();

    std::size_t enqueueGenerationJobs();
    std::size_t enqueueMeshingJobs();


    std::size_t drainAndUploadGenResults();
    std::size_t drainAndUploadMeshResults();
};

// Input::Key definitions (based on glfw3)
