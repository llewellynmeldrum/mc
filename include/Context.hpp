#pragma once 

#include <utility>
#include <iostream>

#include "Camera.hpp"
#include "TextureAtlas.hpp"


void init_glFunctionLoader();

struct GLFWwindow;
struct Window{
    GLFWwindow* ptr = nullptr;
    i32 x = 0;
    i32 y = 0;
    i32 w = 900;
    i32 h = 1169;
    inline f32 aspect(){
        return (f32)w/(f32)h;
    }
};

struct Timer{
    u64 framecount = 0;
    f64 dt= 0.0f;
    f64 elapsed_s = 0.0;
    void init();
    void update();
};

void init_window();
void init_opengl();
void init_keybinds();
struct Context {
    Camera cam;
    Timer time;
    Window win;
    TextureAtlas atlas;
    std::unordered_map<u32, f32> keyRepeatCooldown;
    Context(Camera _cam);

    bool wireframe = false;
};
extern Context ctx;

void handleInputs(Context& ctx);
