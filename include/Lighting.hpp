#pragma once 
#include "ChunkConcurrency.hpp"
LightingResult perform_light_work(LightingJob && job);
inline void set_blocklight(ChunkLightStore& store, ChunkBlockPos pos, BlockLight v){
    store.at(pos).set_blocklight_r(v.r);
    store.at(pos).set_blocklight_g(v.g);
    store.at(pos).set_blocklight_b(v.b);
}
inline void set_sunlight(ChunkLightStore& store, ChunkBlockPos pos, u8 v){
    store.at(pos).set_sunlight_intensity(v);
}
