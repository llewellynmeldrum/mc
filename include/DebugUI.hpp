#pragma once 
#include "ChunkEntry.hpp"
#include "CoordTypes.hpp"
#include "DebugChunkLog.hpp"
#include "DebugChunkRenderer.hpp"
#include "SkyboxState.hpp"
#include "cppslop.hpp"
#include <chrono>
#include <format>
FORWARD_DECL_STRUCT(Engine)
FORWARD_DECL_STRUCT(GLFWwindow)


struct DebugUI{
     DebugUI(SkyboxConfig initial_cfg);
    ~DebugUI();
    void init(GLFWwindow* win_ptr);
    void destroy();
    bool is_ui_expanded{false};
    // default one in Engine.hpp
    SkyboxConfig skybox_cfg;


    void draw();
    void update();

    f32 UI_SCALE = 1.25;
private:
    GLFWwindow* win_ptr;
    static void StartFrame();
};
