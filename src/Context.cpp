#include "Context.hpp"

Context::Context(Camera _cam) : cam(std::move(_cam)) {}
void Context::setup(){
    program_epoch_ns = get_current_ns();
    win.setup();
    input.setup(win.ptr);
    time.setup();
    cam.setup();
}
void Context::handleInputs(){
    if (input.getKey(KEY_ESCAPE) == KeyState::Held){
        win.scheduleClose();
        return;
    }
    f32 dt = ctx.time.dt; // TODO: implement 
    // dt = 1/60
    for (auto& [key,cd]: input.keyRepeatCooldown){
        if (cd>0.0f){
            cd-=dt;
        }
    }
    if (input.getKey(KEY_T) == KeyState::Held){
        if (ctx.keyRepeatCooldown.at(GLFW_KEY_T)<=0.0f){
            ctx.wireframe = !ctx.wireframe;
            ctx.keyRepeatCooldown[GLFW_KEY_T] = 0.1f;
        }
    }
    dt*=60;
    if (input.getKey(KEY_W) == KeyState::Held){
        ctx.cam.move(Direction::FORWARD,dt);
    }
    if (input.getKey(KEY_S) == KeyState::Held){
        ctx.cam.move(Direction::BACKWARD,dt);
    }
    if (input.getKey(KEY_A) == KeyState::Held){
        ctx.cam.move(Direction::LEFT,dt);
    }
    if (input.getKey(KEY_D) == KeyState::Held){
        ctx.cam.move(Direction::RIGHT,dt);
    }
    if (input.getKey(KEY_E) == KeyState::Held){
        ctx.cam.move(Direction::UP,dt);
    }
    if (input.getKey(KEY_Q) == KeyState::Held){
        ctx.cam.move(Direction::DOWN,dt);
    }


    if (input.getKey(KEY_LEFT) == KeyState::Held){
        ctx.cam.rotate(Direction::LEFT,dt);
    }
    if (input.getKey(KEY_RIGHT) == KeyState::Held){
        ctx.cam.rotate(Direction::RIGHT,dt);
    }
    if (input.getKey(KEY_UP) == KeyState::Held){
        ctx.cam.rotate(Direction::UP,dt);
    }
    if (input.getKey(KEY_DOWN) == KeyState::Held){
        ctx.cam.rotate(Direction::DOWN,dt);
    }
}


[[deprecated]]void init_glFunctionLoader(){
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
[[deprecated]]void init_window(){
}
[[deprecated]]void init_opengl(){
    init_glFunctionLoader();
    
    // WARNING: Only set/call these once glbinding has been setup, as they make gl calls.
    assert(ctx.win.ptr);
    glfwSetFramebufferSizeCallback(ctx.win.ptr,glfw_ResizeCallback);
    glfw_ResizeCallback(ctx.win.ptr,ctx.win.w,ctx.win.h);
    glViewport(ctx.win.x, ctx.win.y, ctx.win.w, ctx.win.h);
    // perform depth testing, i.e refuse draw calls which would cause a vertex further away to overwrite a closer one
    glEnable(GL_DEPTH_TEST); 
}
