#pragma once 
// represents a boxed range
#include "NumericConcepts.hpp"
#include "Types.h"
template<typename T>
struct MinMax{
    constexpr MinMax(T point): min(point), max(point){}
    constexpr MinMax(T _min, T _max): min(_min), max(_max){}
    T min;
    T max;
    constexpr T dist_to(T x) const noexcept{
        if (x < min) return min - x;
        if (x > max) return x - max;
        return 0.0f;
    }
    constexpr f32 val() const noexcept{
        return max-min;
    }
    consteval f32 mid() const noexcept{
        return min+(max-min)*.5f;
    }
};
