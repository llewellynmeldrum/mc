#include "Window.hpp"
#include "Context.hpp"
#include "stb_image.hpp"
#include "GLFWCallbacks.hpp"
#include <cassert>
#include <iostream>

// WARNING: Always do glfw->glbinding
#include "GLFWWrapper.hpp"
#include "glbindingWrapper.hpp"
#include "glbinding-aux/Meta.h"
#include "glbinding-aux/logging.h"

#include "Logger.hpp"
#include "DEBUG.hpp"

using namespace gl;

static void init_glFunctionLoader();
void Window::swapBuffers(){
    glfwSwapBuffers(ptr);
}


void Window::setupWindow(void* ctx_ptr){
    LOG_DEBUG("setting up window",__PRETTY_FUNCTION__);
    LOG_EXPR(this);
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

    this->ptr = glfwCreateWindow(w, h, "Window Title", nullptr, nullptr);
    if (!ptr) {
        LOG_ERROR("Failed to initialize GLFW.");
        glfwTerminate();
        LOG_EXIT(EXIT_FAILURE);
    }

    glfwSetWindowUserPointer(ptr,ctx_ptr);
    // WARNING: Doing any OpenGL calls before makeContextCurrent will fk shit up
    glfwMakeContextCurrent(ptr);
    // WARNING:  ^^^^^^^

//    glfwSetWindowPosCallback(win,glfw_MoveCallback);
    glfwSetWindowPos(ptr, x,y);

    float xscale, yscale;
    glfwGetWindowContentScale(ptr, &xscale, &yscale);
    if (xscale !=1.0 || yscale != 1.0){
        LOG_WARN("Retina mode detected, check scaling if any dimensions are weird");
    }
    // ensure we pass the true pixel size to openGL
    glfwGetFramebufferSize(ptr,&w,&h);

    init_glFunctionLoader();
    
    // WARNING: Only set/call these once glbinding has been setup, as they make gl calls.
    assert(ptr);
    glfwSetFramebufferSizeCallback(ptr,glfw_ResizeCallback);
    glfw_ResizeCallback(ptr,w,h);
    glViewport(x, y, w, h);
    
    glEnable(GL_DEPTH_TEST); // perform depth testing, i.e refuse draw calls which would cause a vertex further away to overwrite a closer one
}

bool Window::shouldClose(){
    return glfwWindowShouldClose(ptr);
}

void Window::scheduleClose(){
    glfwSetWindowShouldClose(ptr, true);
}


static void init_glFunctionLoader(){
    LOG_DEBUG("setup gl function loader");
    // at the moment, using glbinding, but i think this is a reasonable thing to swap out
    glbinding::initialize(glfwGetProcAddress);
    glbinding::setCallbackMaskExcept(glbinding::CallbackMask::After | glbinding::CallbackMask::ParametersAndReturnValue, {"glGetError"});
    glbinding::setAfterCallback([](const glbinding::FunctionCall & call) {
        const auto err = glGetError();
        if (err != GL_NO_ERROR){
            std:: cout << fmt::bold_red;
            std::print(">>>>>OPEN GL ERR [{}].  LAST CALL:",glbinding::aux::Meta::getString(err));
            if (!call.function->isResolved()){
                std::cout << " (UNRESOLVED FUNCTION CALL!!):";
            }
            std:: cout << fmt::clear << std::endl << "\t";

            std:: cout << fmt::cyan;
            std::cout << call.function->name();
            std:: cout << fmt::clear << "(";
            for (const auto& param: call.parameters){
                std::cout << param.get();
                if (param != call.parameters.back()){
                  std::print(", ");
                }
            }
            std::cout << ")";

            if (call.returnValue){
              std::cout << " -> " << call.returnValue.get();
            }
            std::println();
            DEBUG_BREAKPOINT();
        }
    });
}
