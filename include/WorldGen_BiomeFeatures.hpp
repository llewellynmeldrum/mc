#pragma once 


#include "Block.hpp"
#include "MinMax.hpp"
#include "Types.h"
#include "WorldGen_RNG.hpp"
#include "WorldGen_BlockWriter.hpp"
#include "WorldGen_Utils.hpp"
#include "cpp23_ranges.hpp"
#include "glm_math_extensions.hpp"
#include <print>

struct FeatureShared{
    bool is_enabled{false};
    RemapTable density_spawn_remap{};
    f32 min_density() const noexcept{
        return density_spawn_remap.min_x();
    }
    f32 max_density() const noexcept{
        return density_spawn_remap.max_x();
    }
    std::vector<BlockType> target_surfaces{
        BlockType::GRASS_BLOCK,
    };
    // todo; add some sort of noise/rng offset here
    bool should_place(WorldBlockPos origin, f32 sampled_density, BlockWriter& blocks);
};
template<typename T>
concept is_biome_feature = 
requires(T e, WorldBlockPos o, f32 d, BlockWriter& b){ 
    {e.shared} ->  std::same_as<FeatureShared>;
    e.place(o,d,b);
};

struct FeatureJitterModule{
    RNG01 rng {RandOffset::TreeJitter};
    i32 min_dist {2};

    inline f32 perform_roll(WorldBlockPos origin)const noexcept{
        return rng.roll01(origin.x, origin.z);
    }
    inline bool should_accept(WorldBlockPos origin, f32 density, const RemapTable& density_to_chance)const noexcept{
        f32 roll = perform_roll(origin);
        f32 min_adjacent_roll = roll;
        if (min_dist>0){
            i32 diff = min_dist-1;
            for (i32 dx = -diff; dx<=diff; dx++){
                for (i32 dz = -diff; dz<=diff; dz++){
                    f32 roll = rng.roll01(
                        origin.x+dx,
                        origin.z+dz
                    );
                    min_adjacent_roll = std::min(min_adjacent_roll, roll);
                }
            }
        }
        if (roll != min_adjacent_roll){  
            return false;
        }

        auto chance = density_to_chance.remap<f32>(roll);
        return roll <= chance;
    }
};

struct TreeFeature{
    FeatureShared shared;
    using value_type = TreeFeature;
    void place(WorldBlockPos origin, f32 density, BlockWriter& blocks)const noexcept;
    bool should_place(WorldBlockPos origin, f32 sampled_density, BlockWriter& blocks)const;

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

    RNGRange<i32> log_height_rng{{4,8}, RandOffset::TreeHeight};
    FeatureJitterModule jitter{RandOffset::TreeJitter, 2};

    // canopies are defined TOP TO BOTTOM! i.e the first canopy is always at log_height+1
    // (there will always be exactly one layer of leaves above the top log)
    std::vector<TreeCanopyLayer> canopies{};


    BlockType log_block;
    BlockType leaf_block;

    // Number of blocks above the highest log that will be leaves only
    i32 top_canopy_y_offset{1};
    bool spruce_crown = false;

};
// 



struct SingleBlockFeature {
    FeatureShared shared;
    using value_type = SingleBlockFeature;
    void place(WorldBlockPos origin, f32 density, BlockWriter& blocks)const noexcept;
    bool should_place(WorldBlockPos origin, f32 sampled_density, BlockWriter& blocks)const;


    BlockType block{};
    FeatureJitterModule jitter{RandOffset::SingleBlockJitter, 0};
};

struct GrassFeature {
    FeatureShared shared;
    using value_type = GrassFeature;
    void place(WorldBlockPos origin, f32 density, BlockWriter& blocks)const noexcept;
    bool should_place(WorldBlockPos origin, f32 sampled_density, BlockWriter& blocks)const;
    

    std::vector<BlockType> growth_stages{};
    FeatureJitterModule jitter{RandOffset::SingleBlockJitter, 0};
    f32 promotion_chance = 0.30f;
    f32 demotion_chance = 0.15f;
};

struct MultiSegmentFeature {
    FeatureShared shared;
    using value_type = FeatureShared;
    void place(WorldBlockPos origin, f32 density, BlockWriter& blocks)const noexcept;
    bool should_place(WorldBlockPos origin, f32 sampled_density, BlockWriter& blocks)const;

    RNGRange<i32> stem_height_rng{{1,3}, RandOffset::TreeHeight};
    BlockType root{};
    BlockType stem{};
    BlockType head{};
    FeatureJitterModule jitter{RandOffset::MultiSegmentJitter, 1};

};

