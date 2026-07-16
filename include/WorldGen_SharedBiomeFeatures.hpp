#pragma once 
#include "WorldGen_BiomeFeatures.hpp"

namespace features{
    constexpr inline TreeFeature regular_oak_tree{
        .is_enabled=true,
        .log_height_rng { {5, 8}, RandOffset::TreeHeight,},
        .num_canopies = 2,
        .canopies{
            TreeCanopyLayer{.hoz_extent=1, .height = 2, .corner_decay_rng = {0.75f, RandOffset::TreeDecay}},
            TreeCanopyLayer{.hoz_extent=2, .height = 2, .corner_decay_rng = {0.75f, RandOffset::TreeDecay}},
        },
        .log_block = BlockType::OAK_LOG,
        .leaf_block = BlockType::OAK_LEAF,
    };
    constexpr inline TreeFeature regular_spruce_tree{
        .is_enabled=true,
        .log_height_rng { {5, 8}, RandOffset::TreeHeight,},
        .num_canopies = 7,
        .canopies{
            TreeCanopyLayer{.hoz_extent=1, .height = 1, .corner_decay_rng = {1.00f, RandOffset::TreeDecay}},
            TreeCanopyLayer{.hoz_extent=0, .height = 1, .corner_decay_rng = {1.00f, RandOffset::TreeDecay}},
            TreeCanopyLayer{.hoz_extent=1, .height = 1, .corner_decay_rng = {1.00f, RandOffset::TreeDecay}},
            TreeCanopyLayer{.hoz_extent=2, .height = 1, .corner_decay_rng = {1.00f, RandOffset::TreeDecay}},
            TreeCanopyLayer{.hoz_extent=1, .height = 1, .corner_decay_rng = {0.00f, RandOffset::TreeDecay}},
            TreeCanopyLayer{.hoz_extent=2, .height = 1, .corner_decay_rng = {1.00f, RandOffset::TreeDecay}},
            TreeCanopyLayer{.hoz_extent=1, .height = 1, .corner_decay_rng = {1.00f, RandOffset::TreeDecay}},
        },
        .log_block = BlockType::SPRUCE_LOG,
        .leaf_block = BlockType::SPRUCE_LEAF,
        .top_canopy_y_offset = 0,
        .target_surfaces {
            BlockType::SNOW_GRASS_BLOCK
        },
        .spruce_crown=true,
    
    };
    constexpr inline TreeFeature regular_palm_tree{
        .is_enabled=true,
        .log_height_rng { {6, 9}, RandOffset::TreeHeight,},
        .num_canopies = 2,
        .canopies{
            TreeCanopyLayer{.hoz_extent=2, .height = 1, .corner_decay_rng = {1.00f, RandOffset::TreeDecay}},
            TreeCanopyLayer{.hoz_extent=3, .height = 1, .corner_decay_rng = {0.00f, RandOffset::TreeDecay}},
        },
        .log_block = BlockType::PALM_LOG,
        .leaf_block = BlockType::PALM_LEAF,
        .top_canopy_y_offset = 1,
        .target_surfaces {
            BlockType::SAND_BLOCK
        },
        .spruce_crown=true,
    
    };
    constexpr inline TreeFeature no_tree{
        .is_enabled=false,
        .log_height_rng { {4, 8}, RandOffset::TreeHeight,},
        .canopies{
            TreeCanopyLayer{.hoz_extent=2, .height = 2, .corner_decay_rng = {0.75f, RandOffset::TreeDecay}},
            TreeCanopyLayer{.hoz_extent=3, .height = 3, .corner_decay_rng = {0.75f, RandOffset::TreeDecay}},
        },
        .log_block = BlockType::OAK_LOG,
        .leaf_block = BlockType::OAK_LEAF,
    };
} // namespace features
