#include "Camera.hpp"
#include "GLFW/glfw3.h"
#include "glm/fwd.hpp"

struct Window{
    GLFWwindow* ptr;
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
    f64 dt_ms= 0.0f;
    f64 elapsed_ms = 0.0;
    void init() { 
        glfwSetTime(0.0);
    }
    void update() {
        f64 prev_elapsed_ms = elapsed_ms;
        elapsed_ms = glfwGetTime()/1000.0;
        dt_ms = elapsed_ms-prev_elapsed_ms;
    }
};

struct Context {
    Camera cam;
    Timer time;
    Window win;
    Context(Camera _cam): cam(_cam){}
    
    bool wireframe = false;
};
extern Context ctx;
