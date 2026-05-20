#pragma once

#include "Types.h"
#include "glm/vec3.hpp"
namespace lmath {

template <class T>
constexpr T unlerp(T a, T b, T t) noexcept {
    if (glm::abs(a - b) < 1e-9) {
        return T(0);
    }
    return (t - a) / (b - a);
}

constexpr bool isVecInBounds(glm::ivec3 v, glm::ivec3 lo, glm::ivec3 hi) noexcept {
    return lo.x <= v.x && v.x < hi.x &&  //
           lo.y <= v.y && v.y < hi.y &&  //
           lo.z <= v.z && v.z < hi.z;    //
}

constexpr inline i32 euclid_mod(i32 a, i32 b) {
    i32 r = a % b;
    return r + (r>0)*b;
}
constexpr inline glm::ivec3 euclid_mod(glm::ivec3 a, glm::ivec3 b) {
    return {
        euclid_mod(a.x,b.x),
        euclid_mod(a.y,b.y),
        euclid_mod(a.z,b.z),
    };
}

constexpr inline i32 floor_div(i32 a, i32 b) {
    return static_cast<i32>(
        std::floor(
            static_cast<f64>(a) /
            static_cast<f64>(b)
        )
    );
}
constexpr inline glm::ivec3 floor_div(glm::ivec3 a, glm::ivec3 b) {
    return {
        floor_div(a.x,b.x),
        floor_div(a.y,b.y),
        floor_div(a.z,b.z),
    };
}

}  // namespace lmath
