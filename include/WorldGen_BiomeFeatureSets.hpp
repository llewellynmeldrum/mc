#pragma once 


#include "Biomes.hpp"
#include "ChunkMesher_RawData.hpp"
#include "CoordTypes.hpp"
#include "WorldGen_BiomeFeatures.hpp"
#include "WorldGen_RNG.hpp"
#include "WorldGen_SharedBiomeFeatures.hpp"
#include "EnumMap.hpp"
#include "FormatSpecs.hpp"
#include "LM.hpp"


struct BiomeFeatureSet{
    TreeFeature tree = features::no_tree;
    GrassFeature grass = features::no_grass;
    MultiSegmentFeature multi_seg = features::no_multi;
    std::vector<SingleBlockFeature> single_blocks {features::no_single};
};

inline EnumMap<BiomeID, BiomeFeatureSet> biome_features {
    {BiomeID::Plains, 
        {
            .tree = features::regular_oak_tree,
            .grass = features::short_grass,
            .single_blocks{
                features::oak_sapling,
            },
        }
    },
    {BiomeID::Tundra,
        {
            .tree = features::regular_spruce_tree,
            .grass = features::short_snow_grass,
        }
    },
    {BiomeID::Beach, 
        {
            .tree = features::regular_palm_tree,
            .single_blocks{
                features::single_mossy,
                features::bush,
            },
        }
    },
    {BiomeID::Desert, 
        BiomeFeatureSet{
            .multi_seg = features::regular_cactus,
            .single_blocks = {
                features::dead_bush,
                features::baby_cactus,
            }
        }
    },
};

