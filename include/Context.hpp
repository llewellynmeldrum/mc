#pragma once 

#include "Camera.hpp"
#include "Window.hpp"
#include "Timer.hpp"
#include "Input.hpp"
#include "DebugUI.hpp"
#include "Renderer.hpp"

#include "World.hpp"

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
    DebugUI ui;
    World world;

    void setupContext();
    void handleInputs();
    void drawScene();
};

// Input::Key definitions (based on glfw3)

