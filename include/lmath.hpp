#pragma once

#include <cstdlib>
namespace lmath {

template <class T>
constexpr T unlerp(T a, T b, T t) noexcept {
    if (std::abs(a - b) < 1e-9) {
        return T(0);
    }
    return (t - a) / (b - a);
}
}  // namespace lmath
