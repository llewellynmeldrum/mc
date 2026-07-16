#pragma once 
#include "CoordTypes.hpp"
#include "MinMax.hpp"
#include "Types.h"
#include "WorldGen_Utils.hpp"
template<typename T>
struct RNGRange{
    constexpr RNGRange(MinMax<T> _range, u32 _offset)
        :range(_range), offset(_offset){ }
    constexpr RNGRange(MinMax<T> _range, RandOffset _offset)
        :RNGRange(_range,std::to_underlying(_offset)){ }

    MinMax<T> range;
    u32 offset;
    f32 sample_rng(WorldBlockPos pos) const noexcept{
        return wpos_seeded_rand01(pos, offset);
    }
    T roll(WorldBlockPos pos)const noexcept{
        return range.min + sample_rng(pos) * range.val();
    }
};
struct BinaryWorldPosRNG{
    f32 chance;
    u32 offset;
    constexpr BinaryWorldPosRNG(f32 chance, u32 _offset)
        :chance(chance), offset(_offset){ }
    constexpr BinaryWorldPosRNG(f32 chance, RandOffset _offset)
        :BinaryWorldPosRNG(chance,std::to_underlying(_offset)){ }
    bool roll(WorldBlockPos pos)const noexcept{
        return (sample_rng(pos) <= chance);
    }
private:
    f32 sample_rng(WorldBlockPos pos) const noexcept{
        return wpos_seeded_rand01(pos, offset);
    }
};

struct RNG01{
    u32 offset;
    constexpr RNG01(u32 _offset)
        : offset(_offset){ }
    constexpr RNG01(RandOffset _offset)
        :RNG01(std::to_underlying(_offset)){ }

    f32 roll01(WorldBlockPos pos)const noexcept{
        return wpos_seeded_rand01(pos, offset);
    }
    // WARNING: ensure the inputs are world x and world z
    f32 roll01(i32 wx, i32 wz)const noexcept{
        return wpos_seeded_rand01(wx,wz, offset);
    }
};

