#pragma once
#include "Types.h"
#include "KeyCodes.hpp"
#include <print>
#include <unordered_map>
#include <array>
#include "glmWrapper.hpp"
#include "CoordIteration.hpp"
#include "Assertion.hpp"
struct GLFWwindow;
// src/Input.cpp
enum struct InputSignal{
    CONTINUE=0,
    RETURN=1,
};


FORWARD_DECL_STRUCT(Profiler)
FORWARD_DECL_STRUCT(Input)
FORWARD_DECL_STRUCT(Window)
FORWARD_DECL_STRUCT(Camera)
FORWARD_DECL_STRUCT(Renderer)
FORWARD_DECL_STRUCT(DebugUI)
struct Input {
    void handle(Profiler& profiler,
	    DebugUI& dbg_ui,
	    Window& win,
	    Camera& player_cam,
	    Camera& drone_cam,
	    Renderer& rend,
	    bool& paused,
	    bool& chunk_updates_paused,
        bool& dbg_modify_chunks,
        bool& dirty_current_chunk
    );

    Input() = default;
    ~Input() = default;
    void set_callbacks(GLFWwindow* ptr);
    void poll();

    f32 default_key_CD_s= 0.15f;
    inline bool isPressed(KeyCode code) {
        if (code < KEY_MIN || code > KEY_MAX) {
            return false;
        }
        return pressed[code];
    }

    inline KeyState getKey(KeyCode code) {
        if (code < KEY_MIN || code > KEY_MAX) {
            return KeyState::INVALID;
        }
        return keyState[code];
    }
    std::array<bool, KEY_MAX + 1> pressed;
    // refers to key state, relative to the last 2 poll calls. If KEY_RELEASED,
    std::array<KeyState, KEY_MAX + 1> keyState{};
    std::array<f32,KEY_MAX> keyRepeatCooldown{};

    inline void updateCooldowns(f32 dt_s){
        for (auto& cd: keyRepeatCooldown) {
            if (cd > 0.0f) {
                cd -= dt_s;
            }
        }
    }



    // DEFAULTED COOLDOWN VERSION 
    template <typename Fn, typename ...Args>
        requires return_type_is<void,Fn&&,Args&&...>
    inline void mapToggleKey(KeyCode k, Fn&& callable, Args&&... vargs){
        mapToggleKeyImpl<void>( k, default_key_CD_s, std::forward<Fn>(callable), std::forward<Args>(vargs)...
        );
    }

    // DEFAULTED COOLDOWN VERSION (signal returning)
    template <typename Fn, typename ...Args>
        requires return_type_is<InputSignal, Fn&&,Args...>
    [[nodiscard]] 
    inline InputSignal mapToggleKey(KeyCode k, Fn&& callable, Args&&... vargs){
        return mapToggleKeyImpl<InputSignal>(k, default_key_CD_s, std::forward<Fn>(callable),std::forward<Args>(vargs)...);
    }

    template <typename Fn>
    inline void mapHeldKey(KeyCode key, Fn&& callable){
        assert(KEY_MAX>=key && key>KEY_MIN);
        bool held = getKey(key)==KeyState::Held;
        if constexpr(std::is_invocable_v<decltype(callable), bool>){
            std::invoke(std::forward<Fn>(callable),held); 
        } else{
            if (held){
                std::invoke(std::forward<Fn>(callable)); 
            }
        }
    }

    glm::vec2 mousepos = { 0.0, 0.0 };  // mapped to ndc like coords
    glm::vec2 scroll = { 0.0, 0.0 };  // mapped to ndc like coords
    glm::vec2 prevmousepos = { 0.0, 0.0 };
    glm::vec2 prevscroll = { 0.0, 0.0 };
private:
    template <typename ReturnT, typename Fn, typename ...Args>
        requires return_type_is<void, Fn&&,Args&&...> || return_type_is<InputSignal,Fn&&,Args&&...>
    inline ReturnT mapToggleKeyImpl(Key key, f32 s_cooldown, Fn&& callable, Args&&... vargs){
        assert(KEY_MAX>=key && key>KEY_MIN);
        std::size_t idx= key - KEY_MIN;
        if (getKey(key) == KeyState::Held) {
            auto& cd = keyRepeatCooldown[idx];
            if (cd <= 0.0f) {
                cd = s_cooldown;
                if constexpr(std::same_as<void,ReturnT>){
                    callable(std::forward<Args>(vargs)...);
                } else{
                    return std::invoke(callable,std::forward<Args>(vargs)...);
                }
            }
        }
        if constexpr(std::same_as<void,ReturnT>){
            return;
        } else {
            return InputSignal::CONTINUE;
        }
    }


};
