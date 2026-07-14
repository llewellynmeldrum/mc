#pragma once 
#include "LM.hpp"
#include "Types.h"

constexpr static inline i32 default_sea_level = {130};
constexpr static inline i32 default_world_seed = {1237};

struct GenConfig{
    i32 sea_level = default_sea_level;
    i32 world_seed = default_world_seed;
};


inline void shuffle_seed(GenConfig& cfg){
    cfg.world_seed = {LM::random<i32>()};
}
