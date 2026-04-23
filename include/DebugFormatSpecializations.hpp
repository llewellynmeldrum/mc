#pragma once 
#ifdef INCLUDE_LOGGER_LAST
#error "Logger.hpp must be included AFTER DebugFormat* headers!"
#endif
#include "DebugFormat.hpp"
#include <format>
#include "AnsiCodes.hpp"
#include "Types.h"
#include "glmWrapper.hpp"

// specializations of dbg_fmt for glm types 
//
template<glm::length_t L, typename T, glm::qualifier Q>
inline constexpr std::string dbg_fmt(const glm::vec<L, T, Q>& v){
    return "CHOSE THE GLM ONE";
}
inline std::string dbg_fmt(const glm::vec4& val){
    return std::format("[{}{}{}, {}{}{}, {}{}{}, {}{}{}]",
                            fmt::red, val.x, fmt::clear,
                            fmt::green, val.y, fmt::clear,
                            fmt::blue, val.z, fmt::clear,
                            fmt::grey , val.w, fmt::clear);
}
inline std::string dbg_fmt(const glm::vec3& val){
    return std::format("[{}{}{}, {}{}{}, {}{}{}]",
                            fmt::red, val.x, fmt::clear,
                            fmt::green, val.y, fmt::clear,
                            fmt::blue, val.z, fmt::clear);
}
inline std::string dbg_fmt(const glm::vec2& val){
    return std::format("[{}{}{}, {}{}{}]", 
                            fmt::red, val.x, fmt::clear,
                            fmt::green, val.y, fmt::clear);
}
inline std::string dbg_fmt(const glm::ivec4& val){
    return std::format("[{}{}{}, {}{}{}, {}{}{}, {}{}{}]",
                            fmt::red, val.x, fmt::clear,
                            fmt::green, val.y, fmt::clear,
                            fmt::blue, val.z, fmt::clear,
                            fmt::grey , val.w, fmt::clear);
}
inline std::string dbg_fmt(const glm::ivec3& val){
    return std::format("[{}{}{}, {}{}{}, {}{}{}]",
                            fmt::red, val.x, fmt::clear,
                            fmt::green, val.y, fmt::clear,
                            fmt::blue, val.z, fmt::clear);
}
inline std::string dbg_fmt(const glm::ivec2& val){
    return std::format("[{}{}{}, {}{}{}]", 
                            fmt::red, val.x, fmt::clear,
                            fmt::green, val.y, fmt::clear);
}
inline std::string dbg_fmt(const glm::mat4& val){
    std::string expr_str{};
    expr_str.append("\n");
    u64 ext = 4;
    for (u64 row = 0; row< ext; row++){
        expr_str.append("| ");
        for (u64 col = 0; col< ext; col++){
            expr_str.append(std::format("{: 3.1f}",val[col][row]));
            if (col!=ext-1){
                expr_str.append(" ");
            }
        }
        expr_str.append(" |");
        if (row!=ext-1)
        expr_str.append("\n");
    }
    return expr_str;
}

#include "Vertex.hpp"
#include "KeyCodes.hpp"
inline std::string dbg_fmt(const Vertex& val){
    return std::format("[{}, {}]", dbg_fmt(val.pos), dbg_fmt(val.txCoords));
}
inline std::string dbg_fmt(const KeyState& val){
    switch(val){
        case KeyState::JustPressed: return "JustPressed"; break;
        case KeyState::JustReleased:return "JustReleased"; break;
        case KeyState::Held:        return "Held"; break;
        case KeyState::Released:    return "Released"; break;

        case KeyState::INVALID: 
            [[fallthrough]];
        default:    
            break;
    }
    return "INVALID";
}
