#pragma once 
#include "BlockLight.hpp"
#include "SharedShaderConfig.hpp"
#include "Types.h"
#include "UnpackedLightValue.hpp"
struct PackedLightValue{
    PackedLightValue(u8 r, u8 g, u8 b, u8 sun = 15) {
        set_blocklight_r(r);
        set_blocklight_g(g);
        set_blocklight_b(b);
        set_sunlight_intensity(sun);
    }
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

    u16 packed_data{0};
    // (lower 16 bits of packed_0 in Vertex)
    // sunlight_intensity ->  0000000000001111
    // blocklight_r       ->  0000000011110000
    // blocklight_g       ->  0000111100000000
    // blocklight_b       ->  1111000000000000

    void set_blocklight_rgb(BlockLight rhs){
        set_blocklight_rgb(pack(rhs));
    }
    void set_blocklight_rgb(PackedLightValue rhs){
        packed_data = (this->reset_blocklight_rgb() | rhs.mask_blocklight_rgb());
    }
    u16 mask_blocklight_rgb() const noexcept{
        return packed_data & BLOCKLIGHT_MASK;
    }
    u16 reset_blocklight_rgb() const noexcept{
        return packed_data & (~BLOCKLIGHT_MASK);
    }

    void set_sunlight_rgb(PackedLightValue rhs){
        packed_data = (reset_sunlight_rgb() | rhs.mask_sunlight_rgb());
    }
    u16 mask_sunlight_rgb() const noexcept{
        return packed_data & SUNLIGHT_INTENSITY_MASK;
    }
    u16 reset_sunlight_rgb() const noexcept{
        return packed_data & (~SUNLIGHT_INTENSITY_MASK);
    }


    //

    void set_sunlight_intensity(u8 val) noexcept{
        SET_BITFIELD_MEMBER_VAL(packed_data, SUNLIGHT_INTENSITY_MASK,SUNLIGHT_INTENSITY_OFFSET,val);
    }
    void set_blocklight_r(u8 val) noexcept {
        SET_BITFIELD_MEMBER_VAL(packed_data, BLOCKLIGHT_R_MASK,BLOCKLIGHT_R_OFFSET,val);
    }
    void set_blocklight_g(u8 val) noexcept {
        SET_BITFIELD_MEMBER_VAL(packed_data, BLOCKLIGHT_G_MASK,BLOCKLIGHT_G_OFFSET,val);
    }
    void set_blocklight_b(u8 val) noexcept {
        SET_BITFIELD_MEMBER_VAL(packed_data, BLOCKLIGHT_B_MASK,BLOCKLIGHT_B_OFFSET,val);
    }

    u8 get_sunlight() const noexcept{
        return UNPACK_BITFIELD_MEMBER(packed_data, SUNLIGHT_INTENSITY_MASK,SUNLIGHT_INTENSITY_OFFSET);
    }
    u16 get_blocklight() const noexcept {
        return UNPACK_BITFIELD_MEMBER(packed_data, BLOCKLIGHT_MASK, BLOCKLIGHT_OFFSET);
    }
    u8 get_blocklight_r() const noexcept {
        return UNPACK_BITFIELD_MEMBER(packed_data, BLOCKLIGHT_R_MASK,BLOCKLIGHT_R_OFFSET);
    }
    u8 get_blocklight_g() const noexcept {
        return UNPACK_BITFIELD_MEMBER(packed_data, BLOCKLIGHT_G_MASK,BLOCKLIGHT_G_OFFSET);
    }
    u8 get_blocklight_b() const noexcept {
        return UNPACK_BITFIELD_MEMBER(packed_data, BLOCKLIGHT_B_MASK,BLOCKLIGHT_B_OFFSET);
    }
};
