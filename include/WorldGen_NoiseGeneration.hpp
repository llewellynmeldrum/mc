#pragma once 
#include "Array2D.hpp"
#include "CoordIteration.hpp"
#include "CoordTypes.hpp"

#include "NoiseGen.hpp"
#include "FastNoiseLite.h"
#include "cppslop.hpp"
#include <utility>

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

enum struct NoiseParamType{
#define X(var) var,
    LIST_NOISE_PARAMS
#undef X
    COUNT
};
inline std::size_t to_idx(NoiseParamType param){
    return std::to_underlying(param);
}
constexpr static i32 N_NOISE_PARAMS = std::to_underlying(NoiseParamType::COUNT);

struct NoiseParams{
#define X(var) f32 var;
    LIST_NOISE_PARAMS
#undef X
};

static inline constexpr f32 WORLD_NOISE_SCALE = 1.3f;

struct NoiseGenerator{
    NoiseGenerator()=default;
    NoiseGenerator(i32 world_seed, const std::vector<NoiseConfig>& cfgs, u32 _gen_epoch = 0):
        #define X(VAR) VAR(world_seed, cfgs[std::to_underlying(NoiseParamType:: VAR)]),
        LIST_NOISE_PARAMS
        #undef X
        gen_epoch(_gen_epoch)
    {}

    #define X(var) NoiseGen var;
    LIST_NOISE_PARAMS
    #undef X
    u32 gen_epoch;

    NoiseParams sample_each(f32 wx, f32 wz) const noexcept{
        return {
            #define X(VAR) .VAR = VAR.sample(wx,wz),
            LIST_NOISE_PARAMS
            #undef X
        };
    }
};


FORWARD_DECL_STRUCT(GenConfig)
ArrayList2D<NoiseParams> generate_chunk_terrain_noise(const GenConfig& cfg, WorldBlockPos chunk_origin);
