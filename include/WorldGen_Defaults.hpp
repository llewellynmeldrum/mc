#pragma once 
#include "WorldGen_BiomeClassification.hpp"
#include "WorldGen_NoiseGeneration.hpp"
#include "glm_math_extensions.hpp"
inline std::vector<NoiseConfig> noise_config_defaults(){
    return {
        NoiseConfig{
            .type = NoiseType::OpenSimplex2,
            .seed_offset = 4823,
            .freq = 0.0005f         * WORLD_NOISE_SCALE,
            .frac_type = FractalType::FBm,
            .frac_octaves = 3,
        },
        NoiseConfig{
            .type = NoiseType::OpenSimplex2,
            .seed_offset =  93221,
            .freq = 0.001f      * WORLD_NOISE_SCALE,
            .frac_type = FractalType::FBm,
            .frac_octaves = 3,
        },
        NoiseConfig{
            .type = NoiseType::OpenSimplex2,
            .seed_offset = 59392,
            .freq = 0.0005f         * WORLD_NOISE_SCALE,
            .frac_type = FractalType::FBm,
            .frac_octaves = 5,
            .frac_lacunarity = 2.0,
            .frac_persistence = 0.5,
        },
        NoiseConfig{
            .type = NoiseType::OpenSimplex2,
            .seed_offset = 1829459,
            .freq = 0.008f * WORLD_NOISE_SCALE,
            .frac_type = FractalType::FBm,
            .frac_octaves = 5,
        },
        NoiseConfig{
            .type = NoiseType::OpenSimplex2,
            .seed_offset =  1234,
            .freq = 0.00066f         * WORLD_NOISE_SCALE,
            .frac_type = FractalType::FBm,
            .frac_octaves = 5,
            .frac_persistence= 0.4,
        },
        NoiseConfig{
            .type = NoiseType::OpenSimplex2,
            .seed_offset =  4321,
            .freq = 0.002f      * WORLD_NOISE_SCALE,
            .frac_type = FractalType::Ridged,
            .frac_octaves = 4,
        },
    };
}
inline std::vector<RemapTable> remap_table_defaults(){
    using glm::vec2;
    return {
        //cont_noise_to_base(remap_tables[0]),
        RemapTable{
            vec2{-1.00f                   ,-70},
            vec2{+COAST.min - .50f        ,-40},
            vec2{+COAST.min - .30f        ,-20},
            vec2{+COAST.min - .10f        ,-6 },
            vec2{ COAST.min               ,-5 },
            vec2{ COAST.mid()             ,-2 },
            vec2{ COAST.max               ,+0 },
            vec2{+0.66f                   ,+2 },
            vec2{+0.80f                   ,+2},
            vec2{+1.00f                   ,+2},
        },
        //cont_noise_to_hill_weight(remap_tables[1]),
        RemapTable{
            vec2{-1.00f,            0.00f},
            vec2{ COAST.min - .30f, 0.00f},
            vec2{ COAST.min - .10f, 0.00f},
            vec2{ COAST.min,        0.10f},
            vec2{ COAST.mid(),      0.30f},
            vec2{ COAST.max,        0.40f},
            vec2{ COAST.max + .10f, 0.70f},
            vec2{ COAST.max + .20f, 0.80f},
            vec2{ COAST.max + .30f, 0.90f},
            vec2{+1.00f,            1.00f}, 
        },
        //hill_noise_to_hill_height(remap_tables[2])
        RemapTable{
            vec2{-1.00f,                    0},
            vec2{ COAST.mid(),              0},
            vec2{ COAST.mid() + 0.10f,      0},
            vec2{ COAST.mid() + 0.45f,      90},
            vec2{+1.00f,                    95}, 
        }
    };
}
