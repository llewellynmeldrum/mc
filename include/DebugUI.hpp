#pragma once 
#include "cppslop.hpp"
FORWARD_DECL_STRUCT(Simulation)
FORWARD_DECL_STRUCT(GLFWwindow)
struct DebugUI{
     DebugUI() = default;
    ~DebugUI() = default;
    void setup(GLFWwindow* win_ptr);
    void drawDebugUI();
    void destroyDebugUI();
    void render();
    void updateUI();

private:
    void drawMainOverlay(Simulation* ctx);
    void drawChunkDebugger(Simulation* ctx);
    void drawFullscreenOverlay(Simulation* ctx) ;
    void drawUI();
    GLFWwindow* win_ptr;
};
