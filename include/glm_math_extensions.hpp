#pragma once 
#include "Types.h"
#include "glm/ext/vector_float2.hpp"

// A remap table contains a set of x and y coordinates.
// Its purpose is to smoothly blend between Y coordinate values, based on the x coordinate supplied.
struct RemapTable{
    struct vec2{
        f32 x;
        f32 y;
    };
    
    RemapTable() = default;
    RemapTable(std::initializer_list<glm::vec2> v): map(std::move(v)){}
    std::vector<glm::vec2> map;

    f32 min_x() const noexcept{
        return map.front().x;
    }
    f32 max_x() const noexcept{
        return map.back().x;
    }
    f32 min_y() const noexcept{
        return map.front().y;
    }
    f32 max_y() const noexcept{
        return map.back().y;
    }
    // given some input x,
    // Find the values within `map`'s x-column which bound x, 
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
    template<typename T>
    constexpr inline T remap(f32 x)const noexcept{
        if (x <= map.front().x){
            return map.front().y;
        }
        if (x >= map.back().x){
            return map.back().y;
        }
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
        return static_cast<T>(map.back().y); //fallback to max 
    }
};

template<typename res_type_>
struct GenericRemapTable{
    using from_type = f32;
    using res_type = res_type_;
    using pair_type = std::pair<from_type,res_type>;
    constexpr GenericRemapTable(std::initializer_list<pair_type> v)
        : m_size(v.size())
        , map(std::move(v))
    {}
    std::size_t m_size;
    std::vector<pair_type> map;
    auto size()const noexcept{return m_size;}

    from_type min_x() const noexcept{
        return map.front().first;
    }
    from_type max_x() const noexcept{
        return map.back().first;
    }
    res_type min_y() const noexcept{
        return map.front().second;
    }
    res_type max_y() const noexcept{
        return map.back().second;
    }
    // given some input x,
    // Find the values within `map`'s x-column which bound x, 
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
    constexpr inline res_type remap(from_type x)const noexcept{
        if (x <= map.front().first){
            return map.front().second;
        }
        if (x >= map.back().first){
            return map.back().second;
        }
        for (i32 i = 1; i<map.size(); i++){
            from_type left_edge = map[i-1].first;
            from_type right_edge = map[i].first;

            res_type min_y = map[i-1].second;
            res_type max_y = map[i].second;

            if (left_edge <= x && x <= right_edge){
                from_type t = glm::smoothstep(left_edge,right_edge,x);
                res_type mix = glm::mix(min_y,max_y,t);
                return mix;
            }
        }
        return static_cast<res_type>(map.back().second); //fallback to max 
    }
};

template<std::ranges::contiguous_range Range>
constexpr inline auto remap(Range r, f32 x){
    auto map = std::span(r);
    if (x <= map.front().first){
        return map.front().second;
    }
    if (x >= map.back().first){
        return map.back().second;
    }
    for (i32 i = 1; i<map.size(); i++){
        auto left_edge = map[i-1].first;
        auto right_edge = map[i].first;

        auto min_y = map[i-1].second;
        auto max_y = map[i].second;

        if (left_edge <= x && x <= right_edge){
            float t = glm::smoothstep(left_edge,right_edge,x);
            auto mix = glm::mix(min_y,max_y,t);
            return mix;
        }
    }
    return map.back().second; //fallback to max 
}
