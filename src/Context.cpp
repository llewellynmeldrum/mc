#include "Context.hpp"

void Context::setupContext() {
    program_epoch_ns = get_current_ns();
    LOG_EXPR(this);
    win.setupWindow(static_cast<void*>(this));
    input.setupInput(win.ptr);
    time.setupTimer();
    cam.setupCamera();
    rend.setupRenderer();

    ui.setupDebugUI(win.ptr);
}

void Context::handleInputs() {
    if (input.getKey(KEY_ESCAPE) == KeyState::Held) {
        win.scheduleClose();
        return;
    }
    f32 dt = time.dt_s;
    // dt = 1/60
    for (auto& [key, cd] : input.keyRepeatCooldown) {
        if (cd > 0.0f) {
            cd -= dt;
        }
    }
    f32 scaled_dt = dt * 60;

    if (input.mousepos != input.prevmousepos) {
        const vec2 diff = input.prevmousepos - input.mousepos;
        cam.rotateByMouse(diff, scaled_dt);
    }

    if (input.getKey(KEY_T) == KeyState::Held) {
        // toggle wireframe renderer
        if (input.keyRepeatCooldown.at(KEY_T) <= 0.0f) {
            rend.debug.wireframe = !rend.debug.wireframe;
            input.keyRepeatCooldown[KEY_T] = 0.1f;
        }
    }
    if (input.getKey(KEY_R) == KeyState::Held) {
        // remesh current chunk
        if (input.keyRepeatCooldown.at(KEY_R) <= 0.0f) {
            rend.debug.wireframe = !rend.debug.wireframe;
            input.keyRepeatCooldown[KEY_R] = 0.1f;
        }
    }
    if (input.getKey(KEY_LEFT_SHIFT) == KeyState::Held) {
        cam.moveSpeed = Camera::SPRINT_MOVESPEED;
    } else {
        cam.moveSpeed = Camera::BASE_MOVESPEED;
    }
    if (input.getKey(KEY_W) == KeyState::Held) {
        cam.move(Direction::FORWARD, scaled_dt);
    }
    if (input.getKey(KEY_S) == KeyState::Held) {
        cam.move(Direction::BACKWARD, scaled_dt);
    }
    if (input.getKey(KEY_A) == KeyState::Held) {
        cam.move(Direction::LEFT, scaled_dt);
    }
    if (input.getKey(KEY_D) == KeyState::Held) {
        cam.move(Direction::RIGHT, scaled_dt);
    }
    if (input.getKey(KEY_SPACE) == KeyState::Held) {
        cam.move(Direction::UP, scaled_dt);
    }
    if (input.getKey(KEY_E) == KeyState::Held) {
        cam.move(Direction::UP, scaled_dt);
    }
    if (input.getKey(KEY_Q) == KeyState::Held) {
        cam.move(Direction::DOWN, scaled_dt);
    }

    if (input.getKey(KEY_LEFT) == KeyState::Held) {
        cam.rotate(Direction::LEFT, scaled_dt);
    }
    if (input.getKey(KEY_RIGHT) == KeyState::Held) {
        cam.rotate(Direction::RIGHT, scaled_dt);
    }
    if (input.getKey(KEY_UP) == KeyState::Held) {
        cam.rotate(Direction::UP, scaled_dt);
    }
    if (input.getKey(KEY_DOWN) == KeyState::Held) {
        cam.rotate(Direction::DOWN, scaled_dt);
    }
    input.prevmousepos = input.mousepos;
}
