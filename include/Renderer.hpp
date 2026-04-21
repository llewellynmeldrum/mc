#pragma once 
#include "Shaders.hpp"

struct Renderer{
    void setup();
    void draw();
    void swap();
    void present();

    ShaderProgram prog;

    struct {
        bool wireframe{false};
    }debug;
};
