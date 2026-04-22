#pragma once 
#include <format>
#include "glbindingWrapper.hpp"
#include "glbinding-aux/types_to_string.h"

#include "AnsiCodes.hpp"
#include "Types.h"
#include "glmWrapper.hpp"

// specializations of dbg_fmt for glm types 
inline constexpr std::string dbg_fmt(const glm::vec<4,float>& val){
    return std::format("[{}{}{}, {}{}{}, {}{}{}, {}{}{}]",
                            fmt::red, val.x, fmt::clear,
                            fmt::green, val.y, fmt::clear,
                            fmt::blue, val.z, fmt::clear,
                            fmt::grey , val.w, fmt::clear);
}
inline constexpr std::string dbg_fmt(const glm::vec<3,float>& val){
    return std::format("[{}{}{}, {}{}{}, {}{}{}]",
                            fmt::red, val.x, fmt::clear,
                            fmt::green, val.y, fmt::clear,
                            fmt::blue, val.z, fmt::clear);
}
inline constexpr std::string dbg_fmt(const glm::vec<2,float>& val){
    return std::format("[{}{}{}, {}{}{}]", 
                            fmt::red, val.x, fmt::clear,
                            fmt::green, val.y, fmt::clear);
}
inline constexpr std::string dbg_fmt(const glm::mat4& val){
    std::string expr_str{};
    expr_str.append("\n");
    constexpr u64 ext = 4;
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
// specializations of dbg_fmt for my types
constexpr std::string dbg_fmt(const Vertex& val){
    return std::format("[{}, {}]", dbg_fmt(val.pos), dbg_fmt(val.txCoords));
}
