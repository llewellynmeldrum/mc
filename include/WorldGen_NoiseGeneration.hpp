#pragma once 
#include "Array2D.hpp"
#include "CoordIteration.hpp"
#include "CoordTypes.hpp"
#include "WorldGen_Config.hpp"

#include "FastNoiseLite.h"

#define LIST_NOISE_PARAMS \
X(heat)\
X(rain)\
X(cont)\
X(grad)\
X(density0)\
X(hill)\
X(mountain)



struct NoiseGenerator{
#define X(var) const NoiseGen var;
    LIST_NOISE_PARAMS
#undef X

    NoiseGenerator(i32 world_seed):
    heat(NoiseConfig{
        .type = NoiseType::Perlin,
        .seed = world_seed + 4823,
        .freq = 0.001f,
        .frac_type = FractalType::FBm,
        .frac_octaves = 3,
        .frac_lacunarity = 1.5f,
        .frac_persistence = 0.9f,
    }),
    rain(NoiseConfig{
        .type = NoiseType::Perlin,
        .seed = world_seed + 93221,
        .freq = 0.001f,
        .frac_type = FractalType::FBm,
        .frac_octaves = 3,
        .frac_lacunarity = 1.5f,
        .frac_persistence = 0.9f,
    }),
    cont(NoiseConfig{
        .type = NoiseType::Perlin,
        .seed = world_seed + 59392,
        .freq = 0.002f,
        .frac_type = FractalType::FBm,
        .frac_octaves = 3,
        .frac_lacunarity = 1.5f,
        .frac_persistence = 0.9f,
    }),
    grad(NoiseConfig{
        .type = NoiseType::Perlin,
        .seed = world_seed + 1234,
        .freq = 0.001f,
        .frac_type = FractalType::FBm,
        .frac_octaves = 5,
    }),
    density0(NoiseConfig{
        .type = NoiseType::Perlin,
        .seed = world_seed + 5853,
        .freq = 0.002f,
        .frac_type = FractalType::FBm,
        .frac_octaves = 3,
        .frac_lacunarity = 2.0f,
        .frac_persistence = 0.5f,
    }),
    hill(NoiseConfig{
        .type = NoiseType::Perlin,
        .seed = world_seed + 1234,
        .freq = 0.005f,
        .frac_type = FractalType::FBm,
        .frac_octaves = 5,
    }),
    mountain(NoiseConfig{
        .type = NoiseType::Perlin,
        .seed = world_seed + 4321,
        .freq = 0.002f,
        .frac_type = FractalType::Ridged,
        .frac_octaves = 4,
    })
    {}
};

struct NoiseParams{
#define X(var) f32 var;
    LIST_NOISE_PARAMS
#undef X
};

inline ArrayList2D<NoiseParams> generate_chunk_terrain_noise(const GenConfig& cfg, WorldBlockPos chunk_origin){
    ArrayList2D<NoiseParams> res(Chunk_Extents2);
    auto to_world = [chunk_origin](i32 cx, i32 cz){
        const auto& [wx_offset, _,wz_offset] = chunk_origin;
        return std::make_pair(cx + wx_offset, cz+wz_offset);
    };
    ForEachInRangeEx({0,0},Chunk_Extents2,[&](i32 cx, i32 cz){
        auto [wx,wz] = to_world(cx,cz);
#define X(var) res[cx,cz].var = cfg.noise.var.sample(wx,wz);
        LIST_NOISE_PARAMS
#undef X
    });
    return res;
}
