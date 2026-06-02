#pragma once 
#include "cppslop.hpp"
FORWARD_DECL_STRUCT(Simulation)
FORWARD_DECL_STRUCT(GLFWwindow)
struct DebugUI{
     DebugUI();
    ~DebugUI();
    void init(GLFWwindow* win_ptr);
    void destroy();

    void draw();
    void update();

    f32 UI_SCALE = 1.25;
private:
    GLFWwindow* win_ptr;
    static void StartFrame();
};
