#include "DebugFormatSpecializations.hpp"

#include "Input.hpp"
#include "Context.hpp"
#include "GLFWWrapper.hpp"

static void glfw_key_callback(GLFWwindow* window, int key, int code, int action, int mods);
// TODO: implement mouse lookaround input

void Input::setupInput(GLFWwindow* ptr) {
    keyRepeatCooldown.insert({ KEY_T, 0 });
    keyRepeatCooldown.insert({ KEY_R, 0 });
    glfwSetKeyCallback(ptr, glfw_key_callback);
}

void Input::poll() {
    glfwPollEvents();
}

static void glfw_key_callback(GLFWwindow* window, int key, int code, int action, int mods) {
    auto* ctx = (Context*)glfwGetWindowUserPointer(window);
    auto& input = ctx->input;
    if (key == GLFW_KEY_UNKNOWN)
        return;  // Ignore unknown keys
    KeyState& key_state = input.keyState[key];
    if (action == GLFW_PRESS) {
        input.keyState[key] = KeyState::Held;
    } else if (action == GLFW_RELEASE) {
        input.keyState[key] = KeyState::Released;
    }
}
