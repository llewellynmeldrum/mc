
#include <GLFW/glfw3.h>

// meh weird glbinding glfw3 issue
#include <glbinding/glbinding.h>
//#include <glbinding/gl/gl.h>
#include <glbinding-aux/Meta.h>
#include <glbinding/FunctionCall.h>
#include <glbinding/CallbackMask.h>

#include "Context.hpp"
void init_keybinds(){
    ctx.keyRepeatCooldown.insert({GLFW_KEY_T, 0});
}
void handleInputs(){
    if (glfwGetKey(ctx.win.ptr,GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(ctx.win.ptr, true);
    }
    f32 dt = ctx.time.dt; // TODO: implement 
    // dt = 1/60
    for (auto& [key,cd]: ctx.keyRepeatCooldown){
        if (cd>0.0f){
            cd-=dt;
        }
    }
    if (glfwGetKey(ctx.win.ptr, GLFW_KEY_T) == GLFW_PRESS){
        if (ctx.keyRepeatCooldown.at(GLFW_KEY_T)<=0.0f){
            ctx.wireframe = !ctx.wireframe;
            ctx.keyRepeatCooldown[GLFW_KEY_T] = 0.1f;
        }
    }
    dt*=60;
    if (glfwGetKey(ctx.win.ptr, GLFW_KEY_W) == GLFW_PRESS){
        ctx.cam.move(Direction::FORWARD,dt);
    }
    if (glfwGetKey(ctx.win.ptr, GLFW_KEY_S) == GLFW_PRESS){
        ctx.cam.move(Direction::BACKWARD,dt);
    }
    if (glfwGetKey(ctx.win.ptr, GLFW_KEY_A) == GLFW_PRESS){
        ctx.cam.move(Direction::LEFT,dt);
    }
    if (glfwGetKey(ctx.win.ptr, GLFW_KEY_D) == GLFW_PRESS){
        ctx.cam.move(Direction::RIGHT,dt);
    }
    if (glfwGetKey(ctx.win.ptr, GLFW_KEY_E) == GLFW_PRESS){
        ctx.cam.move(Direction::UP,dt);
    }
    if (glfwGetKey(ctx.win.ptr, GLFW_KEY_Q) == GLFW_PRESS){
        ctx.cam.move(Direction::DOWN,dt);
    }


    if (glfwGetKey(ctx.win.ptr, GLFW_KEY_LEFT) == GLFW_PRESS){
        ctx.cam.rotate(Direction::LEFT,dt);
    }
    if (glfwGetKey(ctx.win.ptr, GLFW_KEY_RIGHT) == GLFW_PRESS){
        ctx.cam.rotate(Direction::RIGHT,dt);
    }
    if (glfwGetKey(ctx.win.ptr, GLFW_KEY_UP) == GLFW_PRESS){
        ctx.cam.rotate(Direction::UP,dt);
    }
    if (glfwGetKey(ctx.win.ptr, GLFW_KEY_DOWN) == GLFW_PRESS){
        ctx.cam.rotate(Direction::DOWN,dt);
    }
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
void init_glFunctionLoader(){
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
        }
    });
}
void init_window(){
    program_epoch_ns = get_current_ns();
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
void init_opengl(){
    init_glFunctionLoader();
    
    // WARNING: Only set/call these once glbinding has been setup, as they make gl calls.
    assert(ctx.win.ptr);
    glfwSetFramebufferSizeCallback(ctx.win.ptr,glfw_ResizeCallback);
    glfw_ResizeCallback(ctx.win.ptr,ctx.win.w,ctx.win.h);
    glViewport(ctx.win.x, ctx.win.y, ctx.win.w, ctx.win.h);
    // perform depth testing, i.e refuse draw calls which would cause a vertex further away to overwrite a closer one
    glEnable(GL_DEPTH_TEST); 
}
