#include "Window.hpp"
#include "GLFW/glfw3.h"
#include "stb_image.hpp"
void Window::setup(){
    stbi_set_flip_vertically_on_load(true);  

    glfwSetErrorCallback(glfw_ErrorCallback);

    // init glfw
    if (!glfwInit()) {
        LOG_ERROR("Failed to initialize GLFW.");
        LOG_EXIT(EXIT_FAILURE);
    } 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true); // required for opengl 3.2+
    #endif

    ctx.win.ptr = glfwCreateWindow(ctx.win.w, ctx.win.h, "Window Title", nullptr, nullptr);
    if (!ctx.win.ptr) {
        LOG_ERROR("Failed to initialize GLFW.");
        glfwTerminate();
        LOG_EXIT(EXIT_FAILURE);
    }

    // WARNING: Doing any OpenGL calls before makeContextCurrent will fk shit up
    glfwMakeContextCurrent(ctx.win.ptr);
    // WARNING:  ^^^^^^^

    glfwSetWindowUserPointer(ctx.win.ptr,&ctx);
//    glfwSetWindowPosCallback(win,glfw_MoveCallback);
    glfwSetWindowPos(ctx.win.ptr, ctx.win.x,ctx.win.y);

    float xscale, yscale;
    glfwGetWindowContentScale(ctx.win.ptr, &xscale, &yscale);
    if (xscale !=1.0 || yscale != 1.0){
        LOG_WARN("Retina mode detected, check scaling if any dimensions are weird");
    }
    // ensure we pass the true pixel size to openGL
    glfwGetFramebufferSize(ctx.win.ptr,&ctx.win.w,&ctx.win.h);

}

bool Window::shouldClose(){
    return glfwWindowShouldClose(ptr);
}

void Window::scheduleClose(){
    glfwSetWindowShouldClose(ptr, true);
}

static void glfw_ResizeCallback(GLFWwindow* win_ptr, int width, int height){
    auto* ctx = (Context*)glfwGetWindowUserPointer(win_ptr);
    glViewport(0,0,width,height);
    ctx->win.w=width;
    ctx->win.h=height;
    ctx->cam.aspectRatio = ctx->win.aspect();
}
static void glfw_MoveCallback(GLFWwindow* win_ptr, int xpos, int ypos){
    auto* ctx = (Context*)glfwGetWindowUserPointer(win_ptr);
    glViewport(xpos,ypos,ctx->win.w,ctx->win.h);
    ctx->win.x=xpos;
    ctx->win.y=ypos;
}
static void glfw_ErrorCallback(int error, const char* description){
    LOG_ERROR("GLFW({}): {}",error, description);
}
