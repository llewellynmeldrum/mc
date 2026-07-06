#include "DebugOptions.hpp"
#include "FormatSpecs.hpp"

#include "Input.hpp"
#include "KeyCodes.hpp"
#include "Engine.hpp"
#include "GLFWWrapper.hpp"
#include <print>

InputContext::InputContext(Engine* sim) : 
 profiler(sim-> profiler),
 ui(sim->ui),
 win(sim-> win),
 player_cam(sim->player_cam),
 drone_cam(sim->drone_cam),
 rend(sim->rend),
 paused(sim->paused),
 chunk_updates_paused(sim->chunk_updates_paused),
 dbg_modify_chunks(sim->dbg_modify_chunks),
 dirty_current_chunk(sim->dirty_current_chunk)
{}
using namespace glm;
void Input::poll(InputContext ctx) {
    ctx.profiler.bench_start("input");
    poll();

    auto signal = 
    mapToggleKey(KEY_ESCAPE, [&]{
        if (ctx.paused){
            ctx.paused = false;
            return InputSignal::CONTINUE;
        } else{
            ctx.win.scheduleClose();
            return InputSignal::RETURN;
        }
    });
    if (signal == InputSignal::RETURN){ return; }

    updateCooldowns(ctx.profiler.dt_s);


    mapToggleKey(KEY_GRAVE_ACCENT, [&]{
        ctx.ui.is_ui_expanded = !ctx.ui.is_ui_expanded;
    });
    if (scroll.y != prevscroll.y){
        ctx.drone_cam.ortho_zoom *= pow(1.1f,-scroll.y*ctx.drone_cam.zoom_sens*ctx.profiler.dt_s);
    }
    mapToggleKey(KEY_P, [&]{
        ctx.paused = !ctx.paused;
        if (!ctx.paused){
            ctx.win.captureCursor();
            ctx.player_cam.enableMousePanning();
        }else{ 
            ctx.win.freeCursor();
            ctx.player_cam.disableMousePanning();
        }
    });
    mapToggleKey(KEY_G, [&]{
        ctx.chunk_updates_paused= !ctx.chunk_updates_paused;
    });
    mapToggleKey(KEY_L, [&]{
        pause_logging = !pause_logging;
    });
    mapToggleKey(KEY_X, [&]{
        ctx.dirty_current_chunk = !ctx.dirty_current_chunk;
    });

    if (ctx.paused) return; // WARNING: Anything below here is ignored during paused frames


    if (mousepos != prevmousepos) {
        const vec2 diff = prevmousepos - mousepos;
        ctx.player_cam.rotateByMouse(diff, ctx.profiler.dt_s);
    }

    mapToggleKey(KEY_B, [&]{
        ctx.dbg_modify_chunks = !ctx.dbg_modify_chunks;
    });
    mapToggleKey(KEY_T, [&]{
        ctx.rend.debug.wireframe = !ctx.rend.debug.wireframe;
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
            ctx.player_cam.moveSpeed = Camera::SPRINT_MOVESPEED;
            ctx.player_cam.keyboard_sensitivity= Camera::SPRINT_KEYBOARD_SENSITVITY;
        }else{
            mapHeldKey(KEY_LEFT_CONTROL,[&](bool isHeld){
                if (isHeld){
                    ctx.player_cam.moveSpeed = Camera::WALK_MOVESPEED;
                }else{
                    ctx.player_cam.moveSpeed = Camera::BASE_MOVESPEED;
                }
            });
            ctx.player_cam.keyboard_sensitivity= Camera::BASE_KEYBOARD_SENSITIVITY;
        }
    });

    mapHeldKey(KEY_W,[&]{
		ctx.player_cam.move(Direction::FORWARD, ctx.profiler.dt_s);
	});
    mapHeldKey(KEY_S,[&]{
		ctx.player_cam.move(Direction::BACKWARD, ctx.profiler.dt_s);
	});
    mapHeldKey(KEY_A,[&]{
		ctx.player_cam.move(Direction::LEFT, ctx.profiler.dt_s);
	});
    mapHeldKey(KEY_D,[&]{
		ctx.player_cam.move(Direction::RIGHT, ctx.profiler.dt_s);
	});
    mapHeldKey(KEY_SPACE,[&]{
		ctx.player_cam.move(Direction::UP, ctx.profiler.dt_s);
	    ctx.drone_cam.move(Direction::UP, ctx.profiler.dt_s);
	});
    mapHeldKey(KEY_E,[&]{
		ctx.player_cam.move(Direction::UP, ctx.profiler.dt_s);
	});
    mapHeldKey(KEY_Q,[&]{
		ctx.player_cam.move(Direction::DOWN, ctx.profiler.dt_s);
	});

    mapHeldKey(KEY_LEFT,[&]{
		ctx.player_cam.rotate(Direction::LEFT, ctx.profiler.dt_s);
	});
    mapHeldKey(KEY_RIGHT,[&]{
		ctx.player_cam.rotate(Direction::RIGHT, ctx.profiler.dt_s);
	});
    mapHeldKey(KEY_UP,[&]{
		ctx.player_cam.rotate(Direction::UP, ctx.profiler.dt_s);
	});
    mapHeldKey(KEY_DOWN,[&]{
		ctx.player_cam.rotate(Direction::DOWN, ctx.profiler.dt_s);
	});
    prevmousepos = mousepos;
    prevscroll = scroll;
    ctx.profiler.bench_end("input");
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
