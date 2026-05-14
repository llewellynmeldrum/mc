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
    Context() = default;
    ~Context() = default;

    Window   win;
    Timer    time;
    Input    input;
    Camera   cam;
    Renderer rend;
    DebugUI  ui;
    World    world;

    static constexpr i32 RENDER_DIST = 16;
    static constexpr i32 SIMULATION_DIST = 16;
    static constexpr i32 WORLD_SEED = 1337;

    void setupContext();
    void handleInputs();
    void update();
    void draw();
private:
    std::vector<ivec3> findChunksForGeneration();
    std::vector<ChunkView> findChunksForMeshing();

    std::size_t enqueueGenerationJobs();
    std::size_t enqueueMeshingJobs();


    std::size_t drainAndUploadGenResults();
    std::size_t drainAndUploadMeshResults();
};

// Input::Key definitions (based on glfw3)
