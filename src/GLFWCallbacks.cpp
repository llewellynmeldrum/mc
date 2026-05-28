
#include "DebugFormatSpecializations.hpp"
#include "GLFWCallbacks.hpp"
#include "Simulation.hpp"
#include "GLFW/glfw3.h"

void glfw_ResizeCallback(GLFWwindow* win_ptr, int width, int height) {
    auto* ctx = (Simulation*)glfwGetWindowUserPointer(win_ptr);
    ctx->rend.updateViewport(0, 0, width, height);
    ctx->win.px_w = width;
    ctx->win.px_h = height;
    ctx->cam.aspectRatio = ctx->win.aspect();
}
void glfw_MoveCallback(GLFWwindow* win_ptr, int xpos, int ypos) {
    auto* ctx = (Simulation*)glfwGetWindowUserPointer(win_ptr);
    ctx->rend.updateViewport(xpos, ypos, ctx->win.px_w, ctx->win.px_h);
    ctx->win.x = xpos;
    ctx->win.y = ypos;
}
void glfw_MouseMoveCallback(GLFWwindow* win_ptr, double x, double y) {
    auto* ctx = (Simulation*)glfwGetWindowUserPointer(win_ptr);
    ctx->input.mousepos.x = (x / ctx->win.tw) * 2.0 - 1;
    ctx->input.mousepos.y = (y / ctx->win.th) * 2.0 - 1;
}
void glfw_ErrorCallback(int error, const char* description) {
    LOG_ERROR("GLFW({}): {}", error, description);
}
