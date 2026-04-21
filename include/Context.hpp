#pragma once 

#include "Camera.hpp"
#include "Window.hpp"
#include "Timer.hpp"
#include "Input.hpp"
#include "Renderer.hpp"


// src/Context.cpp
struct Context {
public:
    Context(Camera _cam);
    ~Context() = default;

    Window win;
    Timer time;
    Input input;
    Camera cam;
    Renderer rend;

    void setup();
    void handleInputs();
    void draw();

    bool wireframe = false;
};
extern Context ctx;

// Input::Key definitions (based on glfw3)

