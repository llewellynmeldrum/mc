#pragma once

#include <type_traits>
#include <cstdlib>
#include "Types.hpp"
template <typename T>
concept Vec3Like = requires(T v) {
    { v.x } -> std::convertible_to<float>;
    { v.y } -> std::convertible_to<float>;
    { v.z } -> std::convertible_to<float>;
};
namespace lmath {

template <class T>
constexpr T unlerp(T a, T b, T t) noexcept {
    if (std::abs(a - b) < 1e-9) {
        return T(0);
    }
    return (t - a) / (b - a);
}

template <typename T>
    requires Vec3Like<T>
constexpr bool isVecInBounds(T v, T lo, T hi) noexcept {
    return lo.x <= v.x && v.x < hi.x &&  //
           lo.y <= v.y && v.y < hi.y &&  //
           lo.z <= v.z && v.z < hi.z;    //
}
template <typename T>
constexpr T euclid_mod(T a, T b) {
    static_assert(std::is_floating_point<T>() == false);
    T r = a % b;
    return r + (b & (r >> (sizeof(T) * 8 - 1)));
}

}  // namespace lmath
