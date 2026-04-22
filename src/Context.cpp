#include "Context.hpp"

void Context::setupContext(){
    program_epoch_ns = get_current_ns();
    LOG_EXPR(this);
    win.setupWindow(static_cast<void*>(this));
    input.setupInput(win.ptr);
    time.setupTimer();
    cam.setupCamera();
}

void Context::handleInputs(){
    if (input.getKey(KEY_ESCAPE) == KeyState::Held){
        win.scheduleClose();
        return;
    }
    f32 dt = time.dt; // TODO: implement 
    // dt = 1/60
    for (auto& [key,cd]: input.keyRepeatCooldown){
        if (cd>0.0f){
            cd-=dt;
        }
    }
    if (input.getKey(KEY_T) == KeyState::Held){
        if (input.keyRepeatCooldown.at(KEY_T)<=0.0f){
            rend.debug.wireframe = !rend.debug.wireframe;
            input.keyRepeatCooldown[KEY_T] = 0.1f;
        }
    }
    dt*=60;
    if (input.getKey(KEY_W) == KeyState::Held){
        cam.move(Direction::FORWARD,dt);
    }
    if (input.getKey(KEY_S) == KeyState::Held){
        cam.move(Direction::BACKWARD,dt);
    }
    if (input.getKey(KEY_A) == KeyState::Held){
        cam.move(Direction::LEFT,dt);
    }
    if (input.getKey(KEY_D) == KeyState::Held){
        cam.move(Direction::RIGHT,dt);
    }
    if (input.getKey(KEY_E) == KeyState::Held){
        cam.move(Direction::UP,dt);
    }
    if (input.getKey(KEY_Q) == KeyState::Held){
        cam.move(Direction::DOWN,dt);
    }


    if (input.getKey(KEY_LEFT) == KeyState::Held){
        cam.rotate(Direction::LEFT,dt);
    }
    if (input.getKey(KEY_RIGHT) == KeyState::Held){
        cam.rotate(Direction::RIGHT,dt);
    }
    if (input.getKey(KEY_UP) == KeyState::Held){
        cam.rotate(Direction::UP,dt);
    }
    if (input.getKey(KEY_DOWN) == KeyState::Held){
        cam.rotate(Direction::DOWN,dt);
    }
}

