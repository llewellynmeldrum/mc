#include <utility>

#include "Camera.hpp"
#include "GLFW/glfw3.h"
#include "glm/fwd.hpp"

struct Window{
    GLFWwindow* ptr = nullptr;
    i32 x = 0;
    i32 y = 0;
    i32 w = 900;
    i32 h = 1169;
    f32 aspect(){
        return (f32)w/(f32)h;
    }
};
struct Timer{
    u64 framecount = 0;
    f64 dt= 0.0f;
    f64 elapsed_s = 0.0;
    void init() { 
        glfwSetTime(0.0);
    }
    void update() {
        f64 prev_elapsed_s = elapsed_s;
        elapsed_s = glfwGetTime();
        dt= elapsed_s-prev_elapsed_s;
    }
};

struct Context {
    Camera cam;
    Timer time;
    Window win;
    Context(Camera _cam): cam(std::move(_cam)){}
    
    bool wireframe = false;
};
extern Context ctx;
