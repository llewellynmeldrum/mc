#include "Input.hpp"
#include "Context.hpp"
#include "GLFW/glfw3.h"

static void glfw_key_callback(GLFWwindow* window, int key, int code, int action, int mods);
void Input::setup(GLFWwindow* ptr){
    keyRepeatCooldown.insert({KEY_T, 0});
    glfwSetKeyCallback(ptr, glfw_key_callback);
}

void Input::poll(){
    glfwPollEvents();
}

static void glfw_key_callback(GLFWwindow* window, int key, int code, int action, int mods) {
    auto* ctx = (Context*)glfwGetWindowUserPointer(window);
    auto& input = ctx->input;
    if (key == GLFW_KEY_UNKNOWN) return; // Ignore unknown keys
    KeyState& key_state = input.keyState[code];
    
    if (action == GLFW_PRESS){
        if (key_state==KeyState::JustPressed || key_state==KeyState::Held){
            input.keyState[code] = KeyState::Held;
        } else {
            input.keyState[code] = KeyState::JustPressed;
        }
    } else if (action == GLFW_RELEASE){
        if (key_state==KeyState::JustReleased || key_state==KeyState::Released){
            input.keyState[code] = KeyState::Released;
        }else {
            input.keyState[code] = KeyState::JustReleased;
        }
        input.keyState[code] = KeyState::JustPressed;
    }
}
