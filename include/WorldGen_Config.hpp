#pragma once 
#include "LM.hpp"
#include "Types.h"
#include "WorldGen_BiomeClassification.hpp"
#include "cppslop.hpp"
#include "glm_math_extensions.hpp"

constexpr static inline i32 default_sea_level = {130};
constexpr static inline i32 default_world_seed = {1237};

FORWARD_DECL_STRUCT(NoiseGenerator)
struct GenConfig{
    GenConfig(const GenConfig&) = default;
    GenConfig(GenConfig&&) = default;
    GenConfig&            operator=(const GenConfig&) = delete;
    GenConfig&            operator=(GenConfig&&) = delete;

    GenConfig(const NoiseGenerator& _noise):
        noise(_noise)
    {}
    const NoiseGenerator& noise;
    i32 sea_level = default_sea_level;
    i32 world_seed = default_world_seed;
    RemapTable cont_noise_to_base{
        // unsure as to how large to make the coast.
        // I think it makes sense for the coast to extend INTO the ocean a little.
        {-1.00f,                    sea_level-70},
        {+COAST.min - .50f,         sea_level-40},
        {+COAST.min - .30f,         sea_level-20},
        {+COAST.min - .10f,         sea_level-6},
        { COAST.min,                sea_level-5},
        {COAST.mid()                ,sea_level-2},
        { COAST.max,                sea_level+3},
        {+0.66f,                    sea_level+6},
        {+0.80f,                    sea_level+10},
        {+1.00f,                    sea_level+15},
    };
    RemapTable cont_noise_to_hill_weight{
        {-1.00f,            0.00f},
        { COAST.min - .30f, 0.00f},
        { COAST.min - .10f, 0.00f},
        { COAST.min,        0.10f},
        { COAST.mid(),      0.30f},
        { COAST.max,        0.40f},
        { COAST.max + .10f, 0.70f},
        { COAST.max + .20f, 0.80f},
        { COAST.max + .30f, 0.90f},
        {+1.00f,            1.00f}, 
    };
    RemapTable hill_noise_to_hill_height{
        {-1.00f,                    0},
        { COAST.mid(),              0},
        { COAST.mid() + 0.10f,      0},
        { COAST.mid() + 0.45f,      90},
        {+1.00f,                    95}, 
    };
};


inline void shuffle_seed(GenConfig& cfg){
    cfg.world_seed = {LM::random<i32>()};
}
