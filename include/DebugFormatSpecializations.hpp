#pragma once 
#include "Block.hpp"
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
    i64 ext = 4;
    for (i64 row = 0; row< ext; row++){
        expr_str.append("| ");
        for (i64 col = 0; col< ext; col++){
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
inline std::string dbg_fmt(const BlockType& val){
    switch(val){
        case BlockType::AIR: return "BlockType::AIR";
        case BlockType::GRASS_BLOCK: return "BlockType::GRASS_BLOCK";
        case BlockType::DIRT_BLOCK : return "BlockType::DIRT_BLOCK ";
        case BlockType::COUNT: return "BlockType::COUNT";
        default:    
            break;
    }
    return "INVALID";
}
inline std::string dbg_fmt(const Block& val){
    return dbg_fmt(val.id);
}

#include "CommonUtils.hpp"
inline std::string dbg_fmt(const Direction& val){
    switch(val){
		case Direction::FORWARD: return "Direction::FORWARD";
		case Direction::BACKWARD: return "Direction::BACKWARD";
		case Direction::LEFT: return "Direction::LEFT";
		case Direction::RIGHT: return "Direction::RIGHT";
		case Direction::DOWN: return "Direction::DOWN";
		case Direction::UP: return "Direction::UP"; 
        default:    
            break;
    }
    return "INVALID";
}

