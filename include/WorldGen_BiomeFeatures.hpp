#pragma once 


#include "Block.hpp"
#include "MinMax.hpp"
#include "Types.h"
#include "WorldGen_RNG.hpp"
#include "WorldGen_BlockWriter.hpp"
#include "WorldGen_Utils.hpp"
#include "cpp23_ranges.hpp"

// perhaps canopies are defined as arrays of TreeCanopy objects,
// which each describe their width, height, and corner 
struct TreeCanopyLayer{
    i8 hoz_extent; // horizontal extent, i.e distance in blocks from center (including center block)
    i8 height;     // height of the canopy layer
    BinaryWorldPosRNG corner_decay_rng {0.75f, RandOffset::TreeDecay};
    constexpr bool is_corner(i32 dx, i32 dz) const noexcept{
        return std::abs(dx) + std::abs(dz) == (hoz_extent*2);
    }
    constexpr bool is_decayed(WorldBlockPos wpos) const noexcept{
        return corner_decay_rng.roll(wpos);
    }
};
struct TreeFeature{
    bool is_enabled{false};
    RNGRange<i32> log_height_rng{{4,8}, RandOffset::TreeHeight};
    // canopies are defined TOP TO BOTTOM! i.e the first canopy is always at log_height+1
    // (there will always be exactly one layer of leaves above the top log)
    i32 num_canopies{1};
    std::array<TreeCanopyLayer, 16> canopies;

    BlockType log_block;
    BlockType leaf_block;

    // Number of blocks above the highest log that will be leaves only
    i32 top_canopy_y_offset{1};
    std::array<BlockType,1> target_surfaces{
        BlockType::GRASS_BLOCK,
    };
    bool spruce_crown = false;

    void place(WorldBlockPos origin, f32 sampled_density, BlockWriter& blocks)const noexcept;
};
// 

