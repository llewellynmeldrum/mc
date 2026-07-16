#pragma once 

#include "Biomes.hpp"
#include "MinMax.hpp"
#include "WorldGen_NoiseGeneration.hpp"


struct BiomeMatchTable{
    BiomeID biome;
    MinMax<f32> heat;
    MinMax<f32> rain;
    MinMax<f32> grad;
    MinMax<f32> cont;
};

// Representing the entire range, i.e "this biome doesnt care what the distance to x param is"
static constexpr inline auto ANY = MinMax(0.00f,1.00f);
static constexpr inline MinMax OCEAN =  {0.00f,0.42f};
static constexpr inline MinMax COAST =  {0.42f,0.44f};
static constexpr inline MinMax INLAND = {0.44f,1.00f};

static_assert(OCEAN.min == 0);
static_assert(OCEAN.max == COAST.min && COAST.max == INLAND.min);
static_assert(INLAND.max == 1.0f);

static const inline std::vector<BiomeMatchTable> biome_match_tables{
    {.biome = BiomeID::Ocean ,  .heat = ANY,            .rain = ANY,            .grad = ANY,            .cont = OCEAN,         },
    {.biome = BiomeID::Tundra,  .heat = {0.00f,0.25f},  .rain = ANY,            .grad = ANY,            .cont = INLAND,        },
    {.biome = BiomeID::Desert,  .heat = {0.75f,1.00f},  .rain = ANY,            .grad = ANY,            .cont = INLAND,        },
    {.biome = BiomeID::Plains,  .heat = {0.50f},        .rain = {0.50f},        .grad = ANY,            .cont = INLAND,        },
    {.biome = BiomeID::Beach ,  .heat = ANY,            .rain = ANY,            .grad = ANY,            .cont = COAST,         },
};

inline auto classify_biome(const NoiseParams& noise){
    auto min_dist = numeric_max<f32>();
    BiomeID res = BiomeID::Ocean; // ?
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
    ArrayList2D<BiomeID> biome_list(Chunk_Extents2);
    ForEachInRangeEx({0,0},Chunk_Extents2,[&](i32 cx, i32 cz){
        biome_list[cx,cz] = classify_biome(noise_list[cx,cz]);
    });
    return biome_list;
}
