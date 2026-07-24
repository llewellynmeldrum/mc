#include "DebugOptions.hpp"
#include "FormatSpecs.hpp"

#include "Input.hpp"
#include "GLFW/glfw3.h"
#include "GlobalDebugLog.hpp"
#include "KeyCodes.hpp"
#include "Engine.hpp"
#include "GLFWWrapper.hpp"
#include <print>

KeyModifiers::KeyModifiers(int mods) :
    // mods| MOD_KEY
    // 010 | 010 == 010 YES
    // 010 | 100 == 110 NO!!
    // 000 | 100 == 100 NO!!
    shift       ( (mods | GLFW_MOD_SHIFT     ) == mods     ),
    ctrl        ( (mods | GLFW_MOD_CONTROL   ) == mods   ),
    alt         ( (mods | GLFW_MOD_ALT       ) == mods       ),
    super       ( (mods | GLFW_MOD_SUPER     ) == mods     ),
    caps        ( (mods | GLFW_MOD_CAPS_LOCK ) == mods ),
    num_lock    ( (mods | GLFW_MOD_NUM_LOCK  ) == mods  )
{}

void Input::key_callback(GLFWwindow* win_ptr, int key, int scancode, int action, int mods) {
    auto* ctx = (Engine*)glfwGetWindowUserPointer(win_ptr);
//    LOG_DEBUG("cb MODS: {}, ({})",mods, KeyModifiers{mods});
    ctx->input.mods = {mods};
}

Input::Input(GLFWwindow* _win_ptr): win_ptr(_win_ptr){
    glfwSetKeyCallback(_win_ptr,key_callback);
}
bool Input::get_key_state(KeyCode code){
    return glfwGetKey(win_ptr, code) == GLFW_PRESS;
}
void Input::update() {
    for (auto i = KEY_MIN+1; i<=KEY_MAX; i++){
        prev_pressed[i] = pressed[i];
        pressed[i] = get_key_state(i);
    }
}
void Input::end_frame(){
    prevmousepos = mousepos;
    prevscroll = scroll;
}
bool Input::is_down_impl(KeyCode code) {
    assert(code>KEY_MIN && code<KEY_MAX);
    return pressed[code];
}

bool Input::just_pressed_impl(KeyCode code) {
    assert(code>KEY_MIN && code<KEY_MAX);
    return !prev_pressed[code] && pressed[code];
}

bool Input::just_released(KeyCode code) {
    assert(code>KEY_MIN && code<KEY_MAX);
    return prev_pressed[code] && !pressed[code];
}

void Input::set_clipboard(std::string str){
    LOG_DEBUG("CLIPBOARD {}", str);
    push_notification("Copied '{}' to clipboard.", str);
    glfwSetClipboardString(win_ptr, str.c_str());
}

std::string Input::get_clipboard(){
    const char* res = glfwGetClipboardString(win_ptr);
    return {res}; //NOLINT
}
void Input::poll() {
    glfwPollEvents();
    update();
}

