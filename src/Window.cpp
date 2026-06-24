
#include <cassert>
#include <sstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <optional>
#include <regex>
#include <algorithm>

#include <curl/curl.h>      
#include <libxml/HTMLparser.h>
#include <libxml/xpath.h>

// WARNING: Always do glfw->glbinding
#include "FmtStyle.hpp"
#include "GLFWWrapper.hpp"
#include "glbindingWrapper.hpp"
#include "glbinding-aux/Meta.h"
#include "glbinding-aux/logging.h"
#include "glbinding-aux/types_to_string.h"

#include "glHelpers.hpp"
#include "Window.hpp"
#include "GLFW/glfw3.h"
#include "stb_image.hpp"
#include "GLFWCallbacks.hpp"


#include "Logger.hpp"
#include "Breakpoints.hpp"
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

std::string lookup_gl_error_description( const std::string& function_name, const std::string& error);
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
            std::ostringstream fn_params_oss{};
            fn_params_oss << fmt::reset();
            for (const auto& param : call.parameters) {
                fn_params_oss << param;
                if (param != call.parameters.back()) {
                    fn_params_oss <<  ", ";
                }
            }

            std::string fn_sig{};
            if (!call.function->isResolved()) {
                fn_sig=" (UNRESOLVED FUNCTION CALL!!):";
            }

            std::ostringstream fn_return_oss;
            if (call.returnValue.get()) {
                fn_return_oss << call.returnValue;
            }else{
                fn_return_oss << "void";
            }

            fn_sig+= std::format("{}\n\t{}({}) -> {}",
                fmt::reset(),
                fmt::styled(fmt::fg_cyan(), call.function->name()),
                fmt::styled(fmt::reset(), fn_params_oss.str()),
                fmt::styled(fmt::fg_cyan(), fn_return_oss.str())
            );

            std::string error_type_str = glbinding::aux::Meta::getString(err);

             std::string error_str = std::format("{} >>>> OpenGL Error: [{}]. Last function call: {}",
                fmt::bold_red(),
                fmt::styled_bg(fmt::italic(),error_type_str),
                fn_sig
            );

            std::println(stderr,"{}",error_str);
            std::println(stderr,"{}", lookup_gl_error_description(call.function->name(), error_type_str));
            BREAKPOINT_QUIET();
        }
    });
}

