#pragma once 
#include <utility>

#include "LM.hpp"
#include "Types.h"
#include "WorldGen_BiomeClassification.hpp"
#include "WorldGen_NoiseGeneration.hpp"
#include "cppslop.hpp"
#include "glm_math_extensions.hpp"

constexpr static inline i32 default_sea_level = {130};
constexpr static inline i32 default_world_seed = {1237};

struct GenConfig{
    GenConfig() = default;
    GenConfig(GenConfig&&) = default;
    GenConfig(const GenConfig&) = default;
    GenConfig&            operator=(const GenConfig&) = default;
    GenConfig&            operator=(GenConfig&&) = default;

    // asign defaults, change them in WorldGen_Defaults.hpp
    GenConfig(const_span<NoiseConfig> cfgs, const_span<RemapTable> remap_tables):
        #define X(VAR) VAR ##_cfg(cfgs[to_idx(NoiseParamType::VAR)]),
        LIST_NOISE_PARAMS
        #undef X
        cont_noise_to_base(remap_tables[0]),
        cont_noise_to_hill_weight(remap_tables[1]),
        hill_noise_to_hill_height(remap_tables[2])
    {
        apply_noise_cfg();
    }
    #define X(VAR) NoiseConfig VAR ##_cfg;
    LIST_NOISE_PARAMS
    #undef X
    RemapTable cont_noise_to_base;
    RemapTable cont_noise_to_hill_weight;
    RemapTable hill_noise_to_hill_height;

    NoiseGenerator noise;
    i32 sea_level = default_sea_level;
    i32 world_seed = default_world_seed;

    static inline void copy(GenConfig& victim, const GenConfig& goat) noexcept{
        victim = goat;
        victim.apply_noise_cfg();
    }

    void apply_noise_cfg() noexcept{
        // apply the seed
        noise = NoiseGenerator{
            world_seed,
            {
                #define X(VAR) VAR ##_cfg,
                LIST_NOISE_PARAMS
                #undef X
            }
        };
    }

};


inline void shuffle_seed(GenConfig& cfg){
    cfg.world_seed = {LM::random<i32>()};
}
