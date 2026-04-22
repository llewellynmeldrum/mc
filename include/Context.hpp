#pragma once 

#include "Camera.hpp"
#include "Window.hpp"
#include "Timer.hpp"
#include "Input.hpp"
#include "Renderer.hpp"


// src/Context.cpp
struct Context {
public:
    Context() = default;
    ~Context() = default;

    Window win;
    Timer time;
    Input input;
    Camera cam;
    Renderer rend;

    void setupContext();
    void handleInputs();
    void draw();
};

// Input::Key definitions (based on glfw3)

