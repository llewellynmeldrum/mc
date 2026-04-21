#pragma once 
#include "Types.h"
#include "KeyCodes.hpp"
#include <unordered_map>
#include <array>
struct GLFWwindow;
struct Input{
    void setup(GLFWwindow* ptr);
    void poll();

    struct Key{
        u32 code{KEY_UNKNOWN};
        bool pressed{false};
        Key(unsigned char c): code(static_cast<u32>(c)){} // implicit cast from uchar
        u32 operator=(Key&& k){ return k.code; } // implicit cast to u32
    };

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










