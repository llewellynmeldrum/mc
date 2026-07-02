#pragma once 
#include "ChunkEntry.hpp"
#include "CoordTypes.hpp"
#include "DebugChunkLog.hpp"
#include "DebugChunkRenderer.hpp"
#include "cppslop.hpp"
#include <chrono>
#include <format>
FORWARD_DECL_STRUCT(Engine)
FORWARD_DECL_STRUCT(GLFWwindow)


struct DebugUI{
     DebugUI();
    ~DebugUI();
    void init(GLFWwindow* win_ptr);
    void destroy();
    bool is_ui_expanded{false};

    void draw();
    void update();

    f32 UI_SCALE = 1.25;
private:
    GLFWwindow* win_ptr;
    static void StartFrame();
};
