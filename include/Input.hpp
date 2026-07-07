#pragma once
#include "Types.h"
#include "KeyCodes.hpp"
#include <print>
#include <unordered_map>
#include <array>
#include "glmWrapper.hpp"
#include "CoordIteration.hpp"
#include "Assertion.hpp"
#include "VariadicConcepts.hpp"
FORWARD_DECL_STRUCT(GLFWwindow)
// src/Input.cpp


struct Input {
    void update();
    void poll();
    void set_clipboard(std::string str);
    std::string get_clipboard();

    bool get_key_state(KeyCode code);
    void end_frame();
    Input(GLFWwindow* _win_ptr);
    Input() = delete;
    ~Input() = default;

    f32 default_key_CD_s= 0.15f;

    template<typename ...Args>
        requires variadic_all_same<KeyCode, Args...>
    bool is_down(Args... args){
        return (is_down_impl(args) && ...);
    }
    template<typename ...Args>
        requires variadic_all_same<KeyCode, Args...>
    bool just_pressed(Args... args){
        return (just_pressed_impl(args) && ...);
    }
    template<typename ...Args>
        requires variadic_all_same<KeyCode, Args...>
    bool just_released(Args... args){
        return (just_released_impl(args) && ...);
    }

    bool is_down_impl(KeyCode code);
    bool just_pressed_impl(KeyCode code);
    bool just_released(KeyCode code);


    std::array<bool, KEY_MAX+1> pressed{};
    std::array<bool, KEY_MAX+1> prev_pressed{};

    bool no_mods(){
        return !(mods.alt || mods.caps || mods.ctrl || mods.super || mods.num_lock || mods.shift);
    }
    KeyModifiers mods;

    glm::vec2 mousepos = { 0.0, 0.0 };  // mapped to ndc like coords
    glm::vec2 prevmousepos = { 0.0, 0.0 };

    glm::vec2 scroll = { 0.0, 0.0 };  
    glm::vec2 prevscroll = { 0.0, 0.0 };
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
private:
    GLFWwindow* win_ptr{};


};
