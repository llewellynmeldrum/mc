#pragma once
#include "Types.h"
struct GLFWwindow;
// src/Window.cpp
struct Window {
    Window() = default;
    ~Window() = default;
    void setup(void* ctx_ptr);
    // context is passes itself to setupWindow so that it can be assigned to the glfwUserPointer,
    // and thus accessed by glfw callback functions.

    void swapBuffers();
    bool shouldClose();
    void scheduleClose();
    void terminate();

    void captureCursor();
    void freeCursor();
    GLFWwindow* ptr = nullptr;
    i32         x = 0;
    i32         y = 0;

    bool enable_vsync = false;

    // use these for any screen space drawing or anything used for graphics (AR calculations)
    i32 px_w = 1600;  // rendered framebuffer width, px_w = 2*tw on retina displays
    i32 px_h = 900;   // rendered framebuffer height, px_h = 2*th on retina displays

    // use these for mouse input and whatnot
    i32        tw;  // `true` (logical) pixel width, px_w = 2*tw on retina displays
    i32        th;  // `true` (logical) pixel height, px_h = 2*th on retina displays
    inline f32 aspect() { return (f32)px_w / (f32)px_h; }
};
