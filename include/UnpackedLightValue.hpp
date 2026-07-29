#pragma once 
#include "Types.h"
#include <range/v3/range/concepts.hpp>
struct UnpackedLightValue{
    constexpr UnpackedLightValue()= default;
    constexpr UnpackedLightValue(u8 _r, u8 _g, u8 _b)
        : r(_r)
        , g(_g)
        , b(_b)
    {}
    constexpr UnpackedLightValue(u8 _r, u8 _g, u8 _b, u8 _s)
        : r(_r)
        , g(_g)
        , b(_b)
        , s(_s)
    {}
    union{
        struct{
            u8 r{};
            u8 g{};
            u8 b{};
        };
        std::array<u8,3> rgb;
    };
    bool operator==(UnpackedLightValue rhs){
        return this->r == rhs.r
        && this->g == rhs.g
        && this->b == rhs.b;
    }
    u8 s{};
    auto can_propogate()const noexcept{ return (r>1||g>1||b>1); };
    auto is_nonzero()const noexcept{ return (r|g|b) != 0; };
    auto each_rgb(this auto& self){
        return std::ranges::subrange{self.rgb.begin(), self.rgb.end()};
    }
};
