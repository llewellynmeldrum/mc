#pragma once 

#include "Biomes.hpp"
#include "MinMax.hpp"
#include "WorldGen_NoiseGeneration.hpp"


struct BiomeMatchTable{
    MinMax heat;
    MinMax rain;
    MinMax grad;
    MinMax cont;
    BiomeType biome;
};

// Representing the entire range, i.e "this biome doesnt care what the distance to x param is"
static constexpr inline auto ANY = MinMax(0.00f,1.00f);
static constexpr inline MinMax OCEAN =  {0.00f,0.45f};
static constexpr inline MinMax COAST =  {0.45f,0.55f};
static constexpr inline MinMax INLAND = {0.55f,1.00f};
static const inline std::vector<BiomeMatchTable> biome_match_tables{
  //{   .heat = {0.00f,0.00f},  .rain = {0.00f,0.00f} , .grad = {0.00f,0.00f},  .cont = {1.00f,1.00f},  .match = BiomeType::Example},

    {   .heat = ANY,            .rain = ANY,            .grad = ANY,            .cont = OCEAN,        .biome = BiomeType::Ocean },
    {   .heat = {0.00f},        .rain = ANY,            .grad = ANY,            .cont = INLAND,        .biome = BiomeType::Tundra},
    {   .heat = {1.00f},        .rain = ANY,            .grad = ANY,            .cont = INLAND,        .biome = BiomeType::Desert},
    {   .heat = {0.50f},        .rain = {0.50f},        .grad = ANY,            .cont = INLAND,        .biome = BiomeType::Plains},
    {   .heat = ANY,            .rain = ANY,            .grad = ANY,            .cont = COAST,        .biome = BiomeType::Beach},
};

inline auto classify_biome(const NoiseParams& noise){
    auto min_dist = numeric_max<f32>();
    BiomeType res = BiomeType::Ocean; // ?
    for (const auto& target: biome_match_tables){
        f32 dh = target.heat.dist_to(noise.heat);
        f32 dr = target.rain.dist_to(noise.rain);
        f32 dg = target.grad.dist_to(noise.grad);
        f32 dc = target.cont.dist_to(noise.cont);
        f32 dist = (dh+dr+dg+dc)/4.0f;
        if (dist < min_dist){
            min_dist = dist;
            res = target.biome;
        }
    }
    return res;
}

inline auto classify_chunk_biomes(const ArrayList2D<NoiseParams>& noise_list){
    assert(noise_list.extentX == CHUNK_XWIDTH);
    assert(noise_list.extentZ == CHUNK_ZWIDTH);
    ArrayList2D<BiomeType> biome_list(Chunk_Extents2);
    ForEachInRangeEx({0,0},Chunk_Extents2,[&](i32 cx, i32 cz){
        biome_list[cx,cz] = classify_biome(noise_list[cx,cz]);
    });
    return biome_list;
}
