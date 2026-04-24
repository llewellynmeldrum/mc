#pragma once
#include "Types.h"
struct GLFWwindow;
// src/Window.cpp
struct Window {
    Window() = default;
    ~Window() = default;
    void setupWindow(void* ctx_ptr);
    // context is passes itself to setupWindow so that it can be assigned to the glfwUserPointer,
    // and thus accessed by glfw callback functions.

    void swapBuffers();
    bool shouldClose();
    void scheduleClose();

    GLFWwindow* ptr = nullptr;
    i32         x = 0;
    i32         y = 0;
    i32         w = 1600;
    i32         h = 900;
    inline f32  aspect() { return (f32)w / (f32)h; }
};
