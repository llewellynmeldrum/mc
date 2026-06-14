
// WARNING: Always do glfw->glbinding
#include "GLFWWrapper.hpp"
#include "glbindingWrapper.hpp"
#include "glbinding-aux/Meta.h"
#include "glbinding-aux/logging.h"

#include "Window.hpp"
#include "GLFW/glfw3.h"
#include "stb_image.hpp"
#include "GLFWCallbacks.hpp"
#include <cassert>
#include <iostream>


#include "Logger.hpp"
#include "DEBUG.hpp"
#include "Assertion.hpp"

using namespace gl;

static void init_glFunctionLoader();
void        Window::swapBuffers() {
    glfwSwapBuffers(ptr);
}

void Window::set_callbacks(void* ctx_ptr){
    glfwSetWindowUserPointer(ptr, ctx_ptr);
    glfwSetCursorPosCallback(ptr, glfw_MouseMoveCallback);
    glfwSetScrollCallback(ptr, glfw_MouseScrollCallback);
    glfwSetFramebufferSizeCallback(ptr, glfw_ResizeCallback);
    glfw_ResizeCallback(ptr, px_w, px_h);
}
void Window::setup() {
    stbi_set_flip_vertically_on_load(true);

    glfwSetErrorCallback(glfw_ErrorCallback);

    // init glfw
    if (!glfwInit()) {
        LOG_ERROR("Failed to initialize GLFW.");
        LOG_EXIT(EXIT_FAILURE);
    }
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

//    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);  // required for opengl 3.2+
#endif

    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
    this->ptr = glfwCreateWindow(mode->width, mode->height, "My Title", nullptr, nullptr);
    if (!ptr) {
        LOG_ERROR("Failed to initialize GLFW.");
        glfwTerminate();
        LOG_EXIT(EXIT_FAILURE);
    }

    // WARNING: Doing any OpenGL calls before makeContextCurrent will fk shit up
    glfwMakeContextCurrent(ptr);
    // WARNING:  ^^^^^^^

    //    glfwSetWindowPosCallback(win,glfw_MoveCallback);
    glfwSetWindowPos(ptr, x, y);

    float xscale, yscale;
    glfwGetWindowContentScale(ptr, &xscale, &yscale);
    if (xscale != 1.0 || yscale != 1.0) {
        LOG_WARN("Retina mode detected, check scaling ({},{}) if any dimensions are weird", xscale,
                 yscale);
    }
    // ensure we pass the true pixel size to openGL
    glfwGetFramebufferSize(ptr, &px_w, &px_h);
    tw = px_w / xscale;
    th = px_h / yscale;
    glfwSetInputMode(ptr, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(ptr, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    init_glFunctionLoader();

    // WARNING: Only set/call these once glbinding has been setup, as they make gl calls.
    assert(ptr);
    glViewport(x, y, px_w, px_h);
    glfwSwapInterval(enable_vsync);

}

bool Window::shouldClose() {
    return glfwWindowShouldClose(ptr);
}

void Window::scheduleClose() {
    glfwSetWindowShouldClose(ptr, true);
}
void Window::terminate() {
    glfwTerminate();
}
void Window::captureCursor(){
    glfwSetInputMode(ptr, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}
void Window::freeCursor(){
    glfwSetInputMode(ptr, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

static void init_glFunctionLoader() {
    std::println(stderr, "testing\n");
    LOG_DEBUG("setup gl function loader");
    // at the moment, using glbinding, but i think this is a reasonable thing to swap out
    glbinding::initialize(glfwGetProcAddress);
    glbinding::setCallbackMaskExcept(glbinding::CallbackMask::After |
                                         glbinding::CallbackMask::ParametersAndReturnValue,
                                     { "glGetError" });
    glbinding::setAfterCallback([](const glbinding::FunctionCall& call) {
        const auto err = glGetError();
        if (err != GL_NO_ERROR) {
            std::cout << fmt::bold_red;
            std::print(">>>>>OPEN GL ERR [{}].  LAST CALL:", glbinding::aux::Meta::getString(err));
            if (!call.function->isResolved()) {
                std::cout << " (UNRESOLVED FUNCTION CALL!!):";
            }
            std::cout << fmt::reset << std::endl << "\t";

            std::cout << fmt::fg_cyan;
            std::cout << call.function->name();
            std::cout << fmt::reset << "(";
            for (const auto& param : call.parameters) {
                std::cout << param.get();
                if (param != call.parameters.back()) {
                    std::print(", ");
                }
            }
            std::cout << ")";

            if (call.returnValue) {
                std::cout << " -> " << call.returnValue.get();
            }
            std::println();
            DEBUG_BREAKPOINT();
        }
    });
}
