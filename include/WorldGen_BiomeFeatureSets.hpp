#pragma once 

#include <type_traits>

#include "Biomes.hpp"
#include "ChunkMesher_RawData.hpp"
#include "CoordTypes.hpp"
#include "WorldGen_BiomeFeatures.hpp"
#include "WorldGen_RNG.hpp"
#include "WorldGen_SharedBiomeFeatures.hpp"
#include "EnumMap.hpp"
#include "FormatSpecs.hpp"
#include "LM.hpp"

//template<typename FeatureT>

struct BiomeFeatureSet{
    f32 tree_density_threshold = 0.02f;
    i32 min_tree_dist {2};
    RNG01 tree_jitter_rng {RandOffset::TreeJitter};
    TreeFeature tree;
    // call this on every surface + {0,1,0} block of the chunk.
    // Skip trees which are under sea level
    inline bool should_place_tree(WorldBlockPos origin, const TreeFeature& tree, f32 sampled_density, BlockWriter& blocks)const {
        auto cpos = toChunkBlockPos(origin) - BlockOffset{0,1,0};
        bool surface_is_acceptable = LM::linear_contains(tree.target_surfaces, blocks.at(cpos));
        if (!tree.is_enabled){
            return false;
        }
        if (!surface_is_acceptable){
            return false;
        }
        if (blocks.at(cpos + ChunkBlockPos{0,1,0}) == BlockType::WATER_BLOCK){
            return false;
        }
        std::array<f32, 3*3> rolls;
        f32 max_adjacent_roll = 0.0f;
        i32 diff = min_tree_dist-1;
        for (i32 dx = -diff; dx<=diff; dx++){
            for (i32 dz = -diff; dz<=diff; dz++){
                // TODO: cache the values per chunk?
                // Would also need to cache the values in a range of chunk_width + diff, for edge blocks
                f32 roll = tree_jitter_rng.roll01(
                    origin.x+dx,
                    origin.z+dz
                );
                max_adjacent_roll = std::max(max_adjacent_roll, roll);
            }
        }
        f32 roll = tree_jitter_rng.roll01(origin.x, origin.z); 
        if (roll < max_adjacent_roll) return false; // reject trees which are too close
        // NOTE: unsure about this condition. Shouldnt thresholds closer to zero make it more common?
        if (roll >1.0f - sampled_density * tree_density_threshold){
//            std::println("PLACING TREE ",origin);
            return true;
        }else {
 //           std::println("NOT PLACING TREE @{}, {} * {} ({}) <= {}",origin,roll, sampled_density, roll*sampled_density,tree_density_threshold);
            return false;
        }
    }
};
// tree.place()

inline constexpr EnumMap<BiomeID, BiomeFeatureSet> biome_features {
    {BiomeID::Plains, 
        {
            .tree = features::regular_oak_tree
        }
    },
    {BiomeID::Tundra,
        {
            .tree_density_threshold = 0.03f,
            .tree = features::regular_spruce_tree
        }
    },
    {BiomeID::Beach, 
        BiomeFeatureSet{
            .tree_density_threshold = 0.005f,
            .tree = features::regular_palm_tree
        }
    },
};

