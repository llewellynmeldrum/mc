#pragma once 
#include "BlockLight.hpp"
#include "SharedShaderConfig.hpp"
#include "Types.h"
#include "UnpackedLightValue.hpp"
#include "BitField.hpp"
struct PackedLightField{
};
struct PackedLightValue{
    using storage_type = u16;
    storage_type store{0};

BITFIELD_MEMBER(sunlight_intensity, 0,  4L, v/15.0)
BITFIELD_MEMBER(blocklight_r,       4,  4L, v/15.0)
BITFIELD_MEMBER(blocklight_g,       8,  4L, v/15.0)
BITFIELD_MEMBER(blocklight_b,       12, 4L, v/15.0)
    PackedLightValue(u8 r, u8 g, u8 b, u8 sun = 15) {
        set_blocklight_r(r);
        set_blocklight_g(g);
        set_blocklight_b(b);
        set_sunlight_intensity(sun);
    }
    static constexpr auto BLOCKLIGHT_MASK = blocklight_r.mask | blocklight_g.mask |blocklight_b.mask;
    PackedLightValue() = default;
    PackedLightValue(const PackedLightValue&) = default;
    PackedLightValue(PackedLightValue&&) = default;
    PackedLightValue& operator=(const PackedLightValue&) = default;
    PackedLightValue& operator=(PackedLightValue&&) = default;
    auto operator<=>(const PackedLightValue&)const  = default;
    auto pack(UnpackedLightValue const& v){
        return PackedLightValue{
            v.r,v.g,v.b,v.s
        };
    }
    auto pack(BlockLight const& v){
        return PackedLightValue{v.r,v.g,v.b};
    }

    // (lower 16 bits of packed_0 in Vertex)
    // sunlight_intensity ->  0000000000001111
    // blocklight_r       ->  0000000011110000
    // blocklight_g       ->  0000111100000000
    // blocklight_b       ->  1111000000000000

    void set_blocklight_rgb(BlockLight rhs){ set_blocklight_rgb(pack(rhs)); }
    void set_blocklight_rgb(PackedLightValue rhs){
        store = (this->reset_blocklight_rgb() | rhs.mask_blocklight_rgb());
    }
    u16 mask_blocklight_rgb() const noexcept{
        return store & BLOCKLIGHT_MASK;
    }
    u16 reset_blocklight_rgb() const noexcept{
        return store & (~BLOCKLIGHT_MASK);
    }

    void set_sunlight_rgb(PackedLightValue rhs){
        store = (reset_sunlight_rgb() | rhs.mask_sunlight_rgb());
    }
    u16 mask_sunlight_rgb() const noexcept{
        return store & sunlight_intensity.mask;
    }
    u16 reset_sunlight_rgb() const noexcept{
        return store & (~sunlight_intensity.mask);
    }

};
