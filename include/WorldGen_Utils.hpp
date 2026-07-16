#pragma once 
// from https://www.reedbeta.com/blog/hash-functions-for-gpu-rendering/
#include "CoordTypes.hpp"
#include "Types.h"
enum struct RandOffset: u32{
    None = 0,
    TreeHeight = 69,
    TreeJitter = 123,
    GrassGrowth = 0244,
    RejectGrass = 21244,
    TreeDecay = 1337,
};

inline u32 pcg_hash(u32 input) {
    u32 state = input * 747796405u + 2891336453u;
    u32 word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
    return (word >> 22u) ^ word;
}

inline u32 pcg_hash(std::pair<i32, i32> both) {
    return (pcg_hash(both.first) << 4) ^ pcg_hash(both.second);
}
inline u32 pcg_hash(WorldBlockPos v) {
    return (pcg_hash(v[0]) << 4) ^ (pcg_hash(v[1]) << 5) ^ pcg_hash(v[2]) ;
}

inline f32 wpos_seeded_rand01(i32 wx, i32 wz, u32 offset){
    static constexpr u32 big_prime = 120'067;
    u32 offset_hash = pcg_hash(offset);
    u32 hash = offset_hash + pcg_hash({wx, wz});
    hash = hash%big_prime;
    f32 zero_to_one = static_cast<f32>(hash)/static_cast<f32>(big_prime);
    assert(zero_to_one >= 0.0f && zero_to_one <= 1.0f);
    return zero_to_one;
}
inline f32 wpos_seeded_rand01(i32 wx, i32 wz, RandOffset offset=RandOffset::None){
    static constexpr u32 big_prime = 120'067;
    u32 offset_hash = pcg_hash(std::to_underlying(offset));
    u32 hash = offset_hash + pcg_hash({wx, wz});
    hash = hash%big_prime;
    f32 zero_to_one = static_cast<f32>(hash)/static_cast<f32>(big_prime);
    assert(zero_to_one >= 0.0f && zero_to_one <= 1.0f);
    return zero_to_one;
}
inline f32 wpos_seeded_rand01(WorldBlockPos wpos, u32 offset){
    static constexpr u32 big_prime = 120'067;
    u32 offset_hash = pcg_hash(offset);
    u32 hash = offset_hash + pcg_hash(wpos);
    hash = hash%big_prime;
    f32 zero_to_one = static_cast<f32>(hash)/static_cast<f32>(big_prime);
    assert(zero_to_one >= 0.0f && zero_to_one <= 1.0f);
    return zero_to_one;
}
inline f32 wpos_seeded_rand01(WorldBlockPos wpos, RandOffset offset=RandOffset::None){
    static constexpr u32 big_prime = 120'067;
    u32 offset_hash = pcg_hash(std::to_underlying(offset));
    u32 hash = offset_hash + pcg_hash(wpos);
    hash = hash%big_prime;
    f32 zero_to_one = static_cast<f32>(hash)/static_cast<f32>(big_prime);
    assert(zero_to_one >= 0.0f && zero_to_one <= 1.0f);
    return zero_to_one;
}
