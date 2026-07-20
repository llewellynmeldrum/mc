#pragma once 
#include "Array2D.hpp"
#include "CoordIteration.hpp"
#include "CoordTypes.hpp"
#include "WorldGen_Config.hpp"

#include "FastNoiseLite.h"

#define LIST_FUNDAMENTAL_NOISE_PARAMS \
X(heat)\
X(rain)\
X(cont)\
X(grad)

#define LIST_NOISE_PARAMS \
X(heat)\
X(rain)\
X(cont)\
X(grad)\
X(hill)\
X(mountain)


struct NoiseParams{
#define X(var) f32 var;
    LIST_NOISE_PARAMS
#undef X
};

static inline constexpr f32 WORLD_NOISE_SCALE = 1.3f;

struct NoiseGenerator{
#define X(var) const NoiseGen var;
    LIST_NOISE_PARAMS
#undef X

    NoiseParams sample_all(f32 wx, f32 wz) const noexcept{
#define X(VAR) .VAR = VAR.sample(wx,wz),
        return {
            LIST_NOISE_PARAMS
        };
#undef X
    }
    NoiseGenerator(i32 world_seed):
    heat(NoiseConfig{
        .type = NoiseType::OpenSimplex2,
        .seed = world_seed + 4823,
        .freq = 0.0005f         * WORLD_NOISE_SCALE,
        .frac_type = FractalType::FBm,
        .frac_octaves = 3,
    }),
    rain(NoiseConfig{
        .type = NoiseType::OpenSimplex2,
        .seed = world_seed + 93221,
        .freq = 0.001f      * WORLD_NOISE_SCALE,
        .frac_type = FractalType::FBm,
        .frac_octaves = 3,
    }),
    cont(NoiseConfig{
        .type = NoiseType::OpenSimplex2,
        .seed = world_seed + 59392,
        .freq = 0.0005f         * WORLD_NOISE_SCALE,
        .frac_type = FractalType::FBm,
        .frac_octaves = 5,
        .frac_lacunarity = 2.0,
        .frac_persistence = 0.5,
    }),
    grad(NoiseConfig{
        .type = NoiseType::OpenSimplex2,
        .seed = world_seed + 1829459,
        .freq = 0.008f * WORLD_NOISE_SCALE,
        .frac_type = FractalType::FBm,
        .frac_octaves = 5,
    }),
    hill(NoiseConfig{
        .type = NoiseType::OpenSimplex2,
        .seed = world_seed + 1234,
        .freq = 0.00066f         * WORLD_NOISE_SCALE,
        .frac_type = FractalType::FBm,
        .frac_octaves = 5,
        .frac_persistence= 0.4,
    }),
    mountain(NoiseConfig{
        .type = NoiseType::OpenSimplex2,
        .seed = world_seed + 4321,
        .freq = 0.002f      * WORLD_NOISE_SCALE,
        .frac_type = FractalType::Ridged,
        .frac_octaves = 4,
    })
    {}
};


inline ArrayList2D<NoiseParams> generate_chunk_terrain_noise(const GenConfig& cfg, WorldBlockPos chunk_origin){
    ArrayList2D<NoiseParams> res(ChunkInfo::Extents2D);
    auto to_world = [chunk_origin](i32 cx, i32 cz){
        const auto& [wx_offset, _,wz_offset] = chunk_origin;
        return std::make_pair(cx + wx_offset, cz+wz_offset);
    };
    ForEachInRangeEx({0,0},ChunkInfo::Extents2D,[&](i32 cx, i32 cz){
        auto [wx,wz] = to_world(cx,cz);
        res[cx,cz] = cfg.noise.sample_all(wx,wz);
    });
    return res;
}
