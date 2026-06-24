#include "FormatSpecs.hpp"

#include "Input.hpp"
#include "KeyCodes.hpp"
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

    input.updateCooldowns(profiler.dt_s);

    input.mapToggleKey(KEY_P, [this]{
        togglePause();
    });
    input.mapToggleKey(KEY_L, [this]{
        pause_logging = !pause_logging;
    });

    if (isPaused()) return; // WARNING: Anything below here is ignored during paused frames


    if (input.mousepos != input.prevmousepos) {
        const vec2 diff = input.prevmousepos - input.mousepos;
        playerCam.rotateByMouse(diff, profiler.dt_s);
    }
    if (input.scroll.y != input.prevscroll.y){
        
        droneCam.ortho_zoom *= pow(1.1f,-input.scroll.y*droneCam.zoom_sens*profiler.dt_s);
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
        unMeshAllChunks();
        unGenerateAllChunks();
    });

    input.mapToggleKey(KEY_M,[this]{
        unMeshAllChunks();
    });

    input.mapHeldKey(KEY_LEFT_SHIFT,[this](bool isHeld){
        if (isHeld){
            playerCam.moveSpeed = Camera::SPRINT_MOVESPEED;
            playerCam.keyboard_sensitivity= Camera::SPRINT_KEYBOARD_SENSITVITY;
        }else{
            input.mapHeldKey(KEY_LEFT_CONTROL,[this](bool isHeld){
                if (isHeld){
                    playerCam.moveSpeed = Camera::WALK_MOVESPEED;
                }else{
                    playerCam.moveSpeed = Camera::BASE_MOVESPEED;
                }
            });
            playerCam.keyboard_sensitivity= Camera::BASE_KEYBOARD_SENSITIVITY;
        }
    });

    input.mapHeldKey(KEY_W,[this]{
		playerCam.move(Direction::FORWARD, profiler.dt_s);
	});
    input.mapHeldKey(KEY_S,[this]{
		playerCam.move(Direction::BACKWARD, profiler.dt_s);
	});
    input.mapHeldKey(KEY_A,[this]{
		playerCam.move(Direction::LEFT, profiler.dt_s);
	});
    input.mapHeldKey(KEY_D,[this]{
		playerCam.move(Direction::RIGHT, profiler.dt_s);
	});
    input.mapHeldKey(KEY_SPACE,[this]{
		playerCam.move(Direction::UP, profiler.dt_s);
	    droneCam.move(Direction::UP, profiler.dt_s);
	});
    input.mapHeldKey(KEY_E,[this]{
		playerCam.move(Direction::UP, profiler.dt_s);
	});
    input.mapHeldKey(KEY_Q,[this]{
		playerCam.move(Direction::DOWN, profiler.dt_s);
	});

    input.mapHeldKey(KEY_LEFT,[this]{
		playerCam.rotate(Direction::LEFT, profiler.dt_s);
	});
    input.mapHeldKey(KEY_RIGHT,[this]{
		playerCam.rotate(Direction::RIGHT, profiler.dt_s);
	});
    input.mapHeldKey(KEY_UP,[this]{
		playerCam.rotate(Direction::UP, profiler.dt_s);
	});
    input.mapHeldKey(KEY_DOWN,[this]{
		playerCam.rotate(Direction::DOWN, profiler.dt_s);
	});
    input.prevmousepos = input.mousepos;
    input.prevscroll = input.scroll;
}
static void glfw_key_callback(GLFWwindow* window, int key, int code, int action, int mods);
// TODO: implement mouse lookaround input

void Input::set_callbacks(GLFWwindow* ptr) {
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
