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
struct Context {
  public:
    static constexpr std::size_t maxMeshesPerFrame = 128;
    Context() = default;
    ~Context() = default;

    std::size_t chunksMeshed{0};
    Window   win;
    Timer    time;
    Input    input;
    Camera   cam;
    Renderer rend;
    DebugUI  ui;
    World    world;

    void loop();
    static constexpr i32 RENDER_DIST = 8;
    static constexpr i32 SIMULATION_DIST = 16; //controls chunk 
    static constexpr u64 WORLD_SEED = 1337;

    void setupContext();
    void handleInputs();
    void update();
    void draw();
private:
    std::vector<glm::ivec3> findChunksForGeneration();
    std::vector<MeshJob> findMeshJobs();

    std::size_t enqueueGenerationJobs();
    std::size_t enqueueMeshingJobs();


    std::size_t drainAndUploadGenResults();
    std::size_t drainAndUploadMeshResults();
};

// Input::Key definitions (based on glfw3)
