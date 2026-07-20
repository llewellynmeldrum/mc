#pragma once 
#include "WorldGen_BiomeFeatures.hpp"
#include "WorldGen_FeatureClassifcation.hpp"

namespace features{
    inline TreeFeature no_tree{ {.is_enabled=false,} };
    inline GrassFeature no_grass{ {.is_enabled=false}, };
    inline MultiSegmentFeature no_multi{ {.is_enabled=false}};
    inline SingleBlockFeature no_single{ {.is_enabled=false}};

// SECTION: TREES
    inline TreeFeature regular_oak_tree{
        .shared{
            .is_enabled=true,
            .density_spawn_remap{
                    {-1.0f, 0.0f},
                    {-0.4f, 0.0001f},
                    {+0.0f, 0.02f},
                    {+0.4f, 0.04f},
                    {+1.0f, 0.10f},
            }
        },
        .log_height_rng { {5, 8}, RandOffset::TreeHeight,},
        .canopies{
            {.hoz_extent=1, .height = 2, .corner_decay_rng = {0.75f, RandOffset::TreeDecay}},
            {.hoz_extent=2, .height = 2, .corner_decay_rng = {0.75f, RandOffset::TreeDecay}},
        },
        .log_block = BlockType::OAK_LOG,
        .leaf_block = BlockType::OAK_LEAF,
    };
    inline TreeFeature regular_spruce_tree{
        .shared{
            .is_enabled=true,
            .density_spawn_remap{
                    {-1.0f, 0.0f},
                    {-0.4f, 0.01f},
                    {+0.0f, 0.03f},
                    {+0.4f, 0.05f},
                    {+1.0f, 0.10f},
            },
            .target_surfaces {
                BlockType::SNOW_GRASS_BLOCK
            },
        },
        .log_height_rng { {7, 11}, RandOffset::TreeHeight,},
        .canopies{
            {.hoz_extent=1, .height = 1, .corner_decay_rng = {1.00f, RandOffset::TreeDecay}},
            {.hoz_extent=0, .height = 1, .corner_decay_rng = {1.00f, RandOffset::TreeDecay}},
            {.hoz_extent=1, .height = 1, .corner_decay_rng = {1.00f, RandOffset::TreeDecay}},
            {.hoz_extent=2, .height = 1, .corner_decay_rng = {1.00f, RandOffset::TreeDecay}},
            {.hoz_extent=1, .height = 1, .corner_decay_rng = {0.00f, RandOffset::TreeDecay}},
            {.hoz_extent=2, .height = 1, .corner_decay_rng = {1.00f, RandOffset::TreeDecay}},
            {.hoz_extent=1, .height = 1, .corner_decay_rng = {1.00f, RandOffset::TreeDecay}},
        },
        .log_block = BlockType::SPRUCE_LOG,
        .leaf_block = BlockType::SPRUCE_LEAF,
        .top_canopy_y_offset = 0,
        .spruce_crown=true,
    
    };
    inline TreeFeature regular_palm_tree{
        .shared{
            .is_enabled=true,
            .density_spawn_remap{
                    {-1.0f, 0.0f},
                    {-0.4f, 0.00f},
                    {+0.0f, 0.0001f},
                    {+0.4f, 0.0001f},
                    {+1.0f, 0.0010f},
            },
            .target_surfaces {
                BlockType::SAND_BLOCK
            },
        },
        .log_height_rng { {4, 5}, RandOffset::TreeHeight,},
        .canopies{
            {.hoz_extent=1, .height = 1, .corner_decay_rng = {0.00f, RandOffset::TreeDecay}},
        },
        .log_block = BlockType::PALM_LOG,
        .leaf_block = BlockType::PALM_LEAF,
        .top_canopy_y_offset = 0,
        .spruce_crown = true,
    
    };
// SECTION: FOLIAGE
    inline GrassFeature short_grass{
        .shared{
            .is_enabled=true,
            .density_spawn_remap{
                    {-0.4f, 0.10f},
                    {+0.0f, 0.13f},
                    {+0.3f, 0.35f},
                    {+1.0f, 0.60f},
            },
            .target_surfaces {
                BlockType::GRASS_BLOCK
            },
        },
        .growth_stages {
            BlockType::GRASS_TUFT0,
            BlockType::GRASS_TUFT1,
            BlockType::GRASS_TUFT2,
            BlockType::GRASS_TUFT3,
        },
    };
    inline GrassFeature short_snow_grass{
        .shared{
            .is_enabled = true,
            .density_spawn_remap{
                    {-0.4f, 0.10f},
                    {+0.0f, 0.13f},
                    {+0.3f, 0.35f},
                    {+1.0f, 0.60f},
            },
            .target_surfaces {
                BlockType::SNOW_GRASS_BLOCK
            },
        },
        .growth_stages {
            BlockType::SNOW_GRASS_TUFT0,
            BlockType::SNOW_GRASS_TUFT1,
            BlockType::SNOW_GRASS_TUFT2,
            BlockType::SNOW_GRASS_TUFT3,
        },
    };
// SECTION: SINGLE BLOCK FEATURES
    inline SingleBlockFeature dead_bush{
        .shared{
            .is_enabled=true,
            .density_spawn_remap{
                    {-0.6f,    0.003f},
                    {+0.0f,    0.002f},
                    {+0.6f,    0.001f},
            },
            .target_surfaces {
                BlockType::SAND_BLOCK
            },
        },
        .block = BlockType::DEAD_BUSH,
        .jitter {RandOffset::DeadBush, 2} 
    };
    inline SingleBlockFeature baby_cactus{
        .shared{
            .is_enabled=false,
            .density_spawn_remap{
                {-0.6f,    0.0000f},
                {+0.0f,    0.0000f},
                {+0.6f,    0.0000f},
            },
            .target_surfaces {
                BlockType::SAND_BLOCK
            },
        },
        .block = BlockType::BABY_CACTUS,
        .jitter {RandOffset::BabyCactus, 2} 

    };
    inline SingleBlockFeature bush{
        .shared{
            .is_enabled=true,
            .density_spawn_remap{
                {-0.6f,    0.0002f},
                {+0.0f,    0.0001f},
                {+0.6f,    0.0100f},
            },
            .target_surfaces {
                BlockType::SAND_BLOCK
            },
        },
        .block = BlockType::BUSH,
        .jitter {RandOffset::Bush, 2} 

    };
    inline SingleBlockFeature single_mossy{
        .shared{
            .is_enabled=true,
            .density_spawn_remap{
                    {-0.6f,    0.002f},
                    {+0.0f,    0.001f},
                    {+0.6f,    0.000f},
            },
            .target_surfaces {
                BlockType::SAND_BLOCK
            },
        },
        .block = BlockType::MOSSY_COBBLE_BOT_HALF_SLAB,
        .jitter {RandOffset::DeadBush, 2} 
    };
    inline SingleBlockFeature oak_sapling{
        .shared{
            .is_enabled=true,
            .density_spawn_remap{
                    {-0.6f,    0.002f},
                    {+0.0f,    0.001f},
                    {+0.6f,    0.001f},
            },
            .target_surfaces {
                BlockType::GRASS_BLOCK
            },
        },
        .block = BlockType::OAK_SAPLING,
        .jitter {RandOffset::DeadBush, 2} 
    };

// SECTION: MULTI SEGMENT FEATURES
    inline MultiSegmentFeature regular_cactus{
        .shared{
            .is_enabled=true,
            .density_spawn_remap{
                    {-0.6f,    0.000f},
                    {+0.0f,    0.001f},
                    {+0.6f,    0.003f},
            },
            .target_surfaces {
                BlockType::SAND_BLOCK
            },
        },
        .root= BlockType::CACTUS_ROOT,
        .stem = BlockType::CACTUS_STEM,
        .head = BlockType::CACTUS_HEAD,
        .jitter {RandOffset::AdultCactus, 2} 
    };
} // namespace features
