#pragma once 
#include "Types.h"
#include "glm/ext/vector_float2.hpp"
// given some input x, along with a `remap_table`, 
// Find the values within `remap_table`'s x-column which bound x, 
// and smoothly blend x between the two.
// For example:
// remap_table={
//  {0.0f, 100},
//  {0.5f, 120},
//  {1.0f, 200},
// };
// x = 0.25 -> between map[0].x and map[1].x, therefore == mix(100,120, 0.25)
// Note that a smoothstep stage makes the blend SOMEWHERE between 0->0.5. The above example assumes a linear blend
// Smoothstep produces a smooth, S shaped curve given some linear input. Example:
// https://www.desmos.com/calculator/cq5bnndshk
constexpr inline i32 remap_curve (f32 x, std::vector<glm::vec2> map){
    x = glm::clamp(x, map.front().x, map.back().x);
    for (i32 i = 1; i<map.size(); i++){
        auto left_edge = map[i-1].x;
        auto right_edge = map[i].x;

        auto min_y = map[i-1].y;
        auto max_y = map[i].y;

        if (left_edge <= x && x <= right_edge){
            float t = glm::smoothstep(left_edge,right_edge,x);
            float mix = glm::mix(min_y,max_y,t);
            return mix;
        }
    }
    return static_cast<i32>(map.back().y); //fallback to max 
}
