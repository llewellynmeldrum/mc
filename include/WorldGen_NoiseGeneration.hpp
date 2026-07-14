#pragma once 
#include "Array2D.hpp"
#include "CoordIteration.hpp"
#include "CoordTypes.hpp"
#include "WorldGen_Config.hpp"

#include "FastNoiseLite.h"

struct NoiseGenerator{
    const NoiseGen heat;
    const NoiseGen rain;
    const NoiseGen cont;
    const NoiseGen grad;
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
    })
    {}
};

struct NoiseParams{
    f32 heat;
    f32 rain;
    f32 grad;
    f32 cont;
};

inline ArrayList2D<NoiseParams> generate_chunk_terrain_noise(const GenConfig& cfg, WorldBlockPos chunk_origin){
    ArrayList2D<NoiseParams> res(Chunk_Extents2);
    auto to_world = [chunk_origin](i32 cx, i32 cz){
        const auto& [wx_offset, _,wz_offset] = chunk_origin;
        return std::make_pair(cx + wx_offset, cz+wz_offset);
    };
    ForEachInRangeEx({0,0},Chunk_Extents2,[&](i32 cx, i32 cz){
        auto [wx,wz] = to_world(cx,cz);
        res[cx,cz].heat = cfg.noise.heat.sample(wx,wz); 
        res[cx,cz].rain = cfg.noise.rain.sample(wx,wz); 
        res[cx,cz].cont = cfg.noise.cont.sample(wx,wz); 
        res[cx,cz].grad = cfg.noise.grad.sample(wx,wz); 
    });
    return res;
}
