#include "DebugOptions.hpp"
#include "FormatSpecs.hpp"

#include "Input.hpp"
#include "KeyCodes.hpp"
#include "Engine.hpp"
#include "GLFWWrapper.hpp"
#include <print>

using namespace glm;
void Input::handle(
    Profiler& profiler, 
    DebugUI& ui, 
    Window& win, 
    Camera& player_cam, 
    Camera& drone_cam, 
    Renderer& rend, 
    bool& paused,
    bool& chunk_updates_paused,
    bool& dbg_modify_chunks
) {
    profiler.bench_start("input");
    poll();

    auto signal = 
    mapToggleKey(KEY_ESCAPE, [&]{
        if (paused){
            paused = false;
            return InputSignal::CONTINUE;
        } else{
            win.scheduleClose();
            return InputSignal::RETURN;
        }
    });
    if (signal == InputSignal::RETURN){ return; }

    updateCooldowns(profiler.dt_s);


    mapToggleKey(KEY_GRAVE_ACCENT, [&]{
        ui.is_ui_expanded = !ui.is_ui_expanded;
    });
    if (scroll.y != prevscroll.y){
        drone_cam.ortho_zoom *= pow(1.1f,-scroll.y*drone_cam.zoom_sens*profiler.dt_s);
    }
    mapToggleKey(KEY_P, [&]{
        paused = !paused;
        if (!paused){
            win.captureCursor();
            player_cam.enableMousePanning();
        }else{ 
            win.freeCursor();
            player_cam.disableMousePanning();
        }
    });
    mapToggleKey(KEY_G, [&]{
        chunk_updates_paused= !chunk_updates_paused;
    });
    mapToggleKey(KEY_L, [&]{
        pause_logging = !pause_logging;
    });

    if (paused) return; // WARNING: Anything below here is ignored during paused frames


    if (mousepos != prevmousepos) {
        const vec2 diff = prevmousepos - mousepos;
        player_cam.rotateByMouse(diff, profiler.dt_s);
    }

    mapToggleKey(KEY_B, [&]{
        dbg_modify_chunks = !dbg_modify_chunks;
    });
    mapToggleKey(KEY_T, [&]{
        rend.debug.wireframe = !rend.debug.wireframe;
    });
    mapToggleKey(KEY_H, [&]{
        DebugOption::showDebugUI = !DebugOption::showDebugUI;
    });
    mapToggleKey(KEY_C, [&]{
        DebugOption::fill_chunk_boundaries = !DebugOption::fill_chunk_boundaries;
        bool b =         DebugOption::fill_chunk_boundaries;
        LOG_DEBUG("{}->{}",!b,b);
    });
//    mapToggleKey(KEY_R,[&]{
//        unMeshAllChunks();
//        unGenerateAllChunks();
//    });
//
//    mapToggleKey(KEY_M,[&]{
//        unMeshAllChunks();
//    });

    mapHeldKey(KEY_LEFT_SHIFT,[&](bool isHeld){
        if (isHeld){
            player_cam.moveSpeed = Camera::SPRINT_MOVESPEED;
            player_cam.keyboard_sensitivity= Camera::SPRINT_KEYBOARD_SENSITVITY;
        }else{
            mapHeldKey(KEY_LEFT_CONTROL,[&](bool isHeld){
                if (isHeld){
                    player_cam.moveSpeed = Camera::WALK_MOVESPEED;
                }else{
                    player_cam.moveSpeed = Camera::BASE_MOVESPEED;
                }
            });
            player_cam.keyboard_sensitivity= Camera::BASE_KEYBOARD_SENSITIVITY;
        }
    });

    mapHeldKey(KEY_W,[&]{
		player_cam.move(Direction::FORWARD, profiler.dt_s);
	});
    mapHeldKey(KEY_S,[&]{
		player_cam.move(Direction::BACKWARD, profiler.dt_s);
	});
    mapHeldKey(KEY_A,[&]{
		player_cam.move(Direction::LEFT, profiler.dt_s);
	});
    mapHeldKey(KEY_D,[&]{
		player_cam.move(Direction::RIGHT, profiler.dt_s);
	});
    mapHeldKey(KEY_SPACE,[&]{
		player_cam.move(Direction::UP, profiler.dt_s);
	    drone_cam.move(Direction::UP, profiler.dt_s);
	});
    mapHeldKey(KEY_E,[&]{
		player_cam.move(Direction::UP, profiler.dt_s);
	});
    mapHeldKey(KEY_Q,[&]{
		player_cam.move(Direction::DOWN, profiler.dt_s);
	});

    mapHeldKey(KEY_LEFT,[&]{
		player_cam.rotate(Direction::LEFT, profiler.dt_s);
	});
    mapHeldKey(KEY_RIGHT,[&]{
		player_cam.rotate(Direction::RIGHT, profiler.dt_s);
	});
    mapHeldKey(KEY_UP,[&]{
		player_cam.rotate(Direction::UP, profiler.dt_s);
	});
    mapHeldKey(KEY_DOWN,[&]{
		player_cam.rotate(Direction::DOWN, profiler.dt_s);
	});
    prevmousepos = mousepos;
    prevscroll = scroll;
    profiler.bench_end("input");
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
    auto* ctx = (Engine*)glfwGetWindowUserPointer(window);
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
