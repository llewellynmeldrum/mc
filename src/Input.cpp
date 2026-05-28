#include "DebugFormatSpecializations.hpp"

#include "Input.hpp"
#include "Simulation.hpp"
#include "GLFWWrapper.hpp"

using namespace glm;
void Simulation::handleInputs() {
    auto signal = 
    input.mapToggleKey(KEY_ESCAPE, [this]{
        if (isPaused()){
            unpause();
            return InputSignal::CONTINUE;
        } else{
            win.scheduleClose();
            return InputSignal::RETURN;
        }
    });
    if (signal == InputSignal::RETURN){ return; }

    input.updateCooldowns(time.dt_s);

    input.mapToggleKey(KEY_P, [this]{
        togglePause();
    });

    if (isPaused()) return; // WARNING: Anything below here is ignored during paused frames


    if (input.mousepos != input.prevmousepos) {
        const vec2 diff = input.prevmousepos - input.mousepos;
        cam.rotateByMouse(diff, time.dt_s);
    }

    input.mapToggleKey(KEY_T, [this]{
        rend.debug.wireframe = !rend.debug.wireframe;
    });
    input.mapToggleKey(KEY_H, [this]{
        rend.debug.showDebugUI = !rend.debug.showDebugUI;
    });
    input.mapToggleKey(KEY_C, [this]{
        rend.debug.showChunkBoundaries = !rend.debug.showChunkBoundaries;
    });
    input.mapToggleKey(KEY_R,[this]{
        unGenerateAllChunks();
        unMeshAllChunks();
    });

    input.mapToggleKey(KEY_M,[this]{
        unMeshAllChunks();
    });

    static_assert(KEY_MAX>=KEY_LEFT_SHIFT && KEY_LEFT_SHIFT>KEY_MIN);
    input.mapHeldKey(KEY_LEFT_SHIFT,[this](bool isHeld){
        if (isHeld){
            cam.moveSpeed = Camera::SPRINT_MOVESPEED;
        }else{
            cam.moveSpeed = Camera::BASE_MOVESPEED;
        }
    });
    input.mapHeldKey(KEY_W,[this]{
		cam.move(Direction::FORWARD, time.dt_s);
	});
    input.mapHeldKey(KEY_S,[this]{
		cam.move(Direction::BACKWARD, time.dt_s);
	});
    input.mapHeldKey(KEY_A,[this]{
		cam.move(Direction::LEFT, time.dt_s);
	});
    input.mapHeldKey(KEY_D,[this]{
		cam.move(Direction::RIGHT, time.dt_s);
	});
    input.mapHeldKey(KEY_SPACE,[this]{
		cam.move(Direction::UP, time.dt_s);
	});
    input.mapHeldKey(KEY_E,[this]{
		cam.move(Direction::UP, time.dt_s);
	});
    input.mapHeldKey(KEY_Q,[this]{
		cam.move(Direction::DOWN, time.dt_s);
	});

    input.mapHeldKey(KEY_LEFT,[this]{
		cam.rotate(Direction::LEFT, time.dt_s);
	});
    input.mapHeldKey(KEY_RIGHT,[this]{
		cam.rotate(Direction::RIGHT, time.dt_s);
	});
    input.mapHeldKey(KEY_UP,[this]{
		cam.rotate(Direction::UP, time.dt_s);
	});
    input.mapHeldKey(KEY_DOWN,[this]{
		cam.rotate(Direction::DOWN, time.dt_s);
	});
    input.prevmousepos = input.mousepos;
}
static void glfw_key_callback(GLFWwindow* window, int key, int code, int action, int mods);
// TODO: implement mouse lookaround input

void Input::setupInput(GLFWwindow* ptr) {
    glfwSetKeyCallback(ptr, glfw_key_callback);
}

void Input::poll() {
    glfwPollEvents();
}

static void glfw_key_callback(GLFWwindow* window, int key, int code, int action, int mods) {
    auto* ctx = (Simulation*)glfwGetWindowUserPointer(window);
    auto& input = ctx->input;
    //    LOG_DEBUG("Key {} pressed, code:{}, action:{},mods={},", key, code, action, mods);
    if (key == GLFW_KEY_UNKNOWN) {
        return;
    }
    KeyState& key_state = input.keyState[key];
    if (action == GLFW_PRESS) {
        input.keyState[key] = KeyState::Held;
    } else if (action == GLFW_RELEASE) {
        input.keyState[key] = KeyState::Released;
    }
}
