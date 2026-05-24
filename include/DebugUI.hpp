#pragma once 
#include "cppslop.hpp"
FORWARD_DECL_STRUCT(Simulation)
FORWARD_DECL_STRUCT(GLFWwindow)
struct DebugUI{
     DebugUI() = default;
    ~DebugUI() = default;
    void setupDebugUI(GLFWwindow* win_ptr);
    void drawDebugUI();
    void destroyDebugUI();
    void render();
    void updateUI();
private:
    void ShowMainOverlay(Simulation* ctx);
    void ShowChunkDebugger(Simulation* ctx);
void ShowFullscreenOverlay(Simulation* ctx) ;
    void drawUI();
    GLFWwindow* win_ptr;
};
