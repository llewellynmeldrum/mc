#pragma once 
#include "Types.h"
#include "UnpackedLightValue.hpp"
#include "cppslop.hpp"
#include <limits>
#include <range/v3/view/enumerate.hpp>
using SunLight = u8;
struct BlockLight{
    union{
        struct{
            u8 r{};
            u8 g{};
            u8 b{};
        };
        std::array<u8,3> arr;
    };
    constexpr BlockLight(u8 _r, u8 _g, u8 _b) {
        r=_r;
        g=_g;
        b=_b;
    }
    constexpr bool operator==(BlockLight rhs) const noexcept{
        return this->r == rhs.r
        && this->g == rhs.g
        && this->b == rhs.b;
    }
    auto is_nonzero()const noexcept{ return (r|g|b) != 0; };
    constexpr u8 avg()const noexcept{
        return static_cast<u8>(std::round((r+g+b)/3.0f));
    }

};

FORWARD_DECL_STRUCT(PackedLightValue)
UnpackedLightValue unpack(PackedLightValue const& v);
BlockLight unpack_blocklight(PackedLightValue const& v);
SunLight unpack_sunlight(PackedLightValue const& v);
