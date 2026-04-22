#pragma once 
#include "Types.h"
#include "KeyCodes.hpp"
#include <unordered_map>
#include <array>
struct GLFWwindow;
// src/Input.cpp
struct Input{
    Input()=default;
    ~Input()=default;
    void setupInput(GLFWwindow* ptr);
    void poll();


    inline bool isPressed(KeyCode code){
        if (code<KEY_MIN || code > KEY_MAX){
            return false;
        }
        return pressed[code];
    }

    inline KeyState getKey(KeyCode code){
        if (code<KEY_MIN || code > KEY_MAX){
            return KeyState::INVALID;
        }
        return keyState[code];
    }
    std::array<bool, KEY_MAX+1> pressed;
    // refers to key state, relative to the last 2 poll calls. If KEY_RELEASED,
    std::array<KeyState, KEY_MAX+1> keyState; 
    std::unordered_map<Key, f32> keyRepeatCooldown;
};










