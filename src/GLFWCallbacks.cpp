
#include "GLFWCallbacks.hpp"
#include "Context.hpp"
#include "GLFW/glfw3.h"

void glfw_ResizeCallback(GLFWwindow* win_ptr, int width, int height){
    auto* ctx = (Context*)glfwGetWindowUserPointer(win_ptr);
    ctx->rend.updateViewport(0,0,width,height);
    ctx->win.w=width;
    ctx->win.h=height;
    ctx->cam.aspectRatio = ctx->win.aspect();
}
void glfw_MoveCallback(GLFWwindow* win_ptr, int xpos, int ypos){
    auto* ctx = (Context*)glfwGetWindowUserPointer(win_ptr);
    ctx->rend.updateViewport(xpos,ypos,ctx->win.w, ctx->win.h);
    ctx->win.x=xpos;
    ctx->win.y=ypos;
}
void glfw_ErrorCallback(int error, const char* description){
    LOG_ERROR("GLFW({}): {}",error, description);
}
