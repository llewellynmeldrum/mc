#pragma once 

#include "Types.h"
#include <array>
enum class BlockType: u64{
    AIR=0,
    GRASS_BLOCK,
    DIRT_BLOCK,
    COUNT,
};

inline std::array<f32,static_cast<size_t>(BlockType::COUNT)> blockOpacity ={
    0.0,
    1.0,
    1.0,
};

struct Block{
    BlockType id; 
    constexpr inline u64 idx() const noexcept{
        return static_cast<u64>(id);
    }
    constexpr inline u64 texture_id() const noexcept{
        return idx()-1; // this is hacky
    }
    constexpr inline bool isOpaque()const noexcept{
        return blockOpacity[idx()]>=1.0;
    }
    constexpr inline bool isAir()const noexcept{
        return id==BlockType::AIR;
    }
};








