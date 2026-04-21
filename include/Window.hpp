#pragma once 
#include "Types.h"
struct GLFWwindow;
// src/Window.cpp
struct Window{
    void setup();

    void draw();
    bool shouldClose();
    void scheduleClose();
    
    GLFWwindow* ptr = nullptr;
    i32 x = 0;
    i32 y = 0;
    i32 w = 900;
    i32 h = 1169;
    inline f32 aspect(){
        return (f32)w/(f32)h;
    }
};
