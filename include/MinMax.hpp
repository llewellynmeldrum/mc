#pragma once 
// represents a boxed range
#include "Types.h"
struct MinMax{
    constexpr MinMax(f32 point): min(point), max(point){}
    constexpr MinMax(f32 _min, f32 _max): min(_min), max(_max){}
    f32 min;
    f32 max;
    constexpr f32 dist_to(f32 x) const{
        if (x < min) return min - x;
        if (x > max) return x - max;
        return 0.0f;
    }
};
