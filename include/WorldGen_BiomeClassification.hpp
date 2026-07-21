#pragma once 

#include "Biomes.hpp"
#include "MinMax.hpp"
#include "WorldGen_NoiseGeneration.hpp"


static constexpr f32 NOISE_MIN = -1.0f;
static constexpr f32 NOISE_MAX = +1.0f;

// Representing the entire range, i.e "this biome doesnt care what the distance to x param is"
static constexpr inline auto ANY = MinMax(-1.00f,+1.00f);

static constexpr f32 coastline_x = -0.2f;
static constexpr f32 coastline_size = 0.10f;

static constexpr f32 far_inland_x = coastline_x + 0.35f;

static constexpr inline MinMax COAST =  {coastline_x - coastline_size*2.0f,coastline_x + coastline_size};


static constexpr inline MinMax OCEAN {NOISE_MIN, COAST.min};
static constexpr inline MinMax NEAR_INLAND                {COAST.max, far_inland_x};
static constexpr inline MinMax FAR_INLAND                                      {NEAR_INLAND.max,NOISE_MAX};

struct BiomeMatchTable{
    BiomeID biome;
    MinMax<f32> heat{ANY};
    MinMax<f32> rain{ANY};
    MinMax<f32> grad{ANY};
    MinMax<f32> cont{ANY};
};

static_assert(OCEAN.min == -1.0f);
static_assert(OCEAN.max == COAST.min && COAST.max == NEAR_INLAND.min);
static_assert(FAR_INLAND.max == 1.0f);

static const inline std::vector<BiomeMatchTable> biome_match_tables{

    BiomeMatchTable{
        .biome = BiomeID::Ocean,
        .heat = {-0.4f,1.0f},
        .cont = OCEAN,
    },
    BiomeMatchTable{
        .biome = BiomeID::IceBeach,
        .heat = {-0.4f},
        .cont = COAST,
    },
    BiomeMatchTable{
        .biome = BiomeID::FrozenOcean,
        .heat = {-0.4f},
        .cont = OCEAN,
    },
    BiomeMatchTable{
        .biome = BiomeID::Beach,
        .heat = {+0.3f},
        .cont = COAST,
    },
    BiomeMatchTable{
        .biome = BiomeID::Plains,
        .heat = {0.0f},
        .cont = NEAR_INLAND,
    },
    BiomeMatchTable{
        .biome = BiomeID::Tundra,
        .heat = {-0.4f},
        .cont = FAR_INLAND,
    },
    BiomeMatchTable{
        .biome = BiomeID::Desert,
        .heat = {+0.4f},
        .cont = FAR_INLAND,
    },
};

struct BiomeClassification{
    BiomeID id{BiomeID::Ocean};
    f32 heat_score{};
    f32 rain_score{};
    f32 grad_score{};
    f32 cont_score{};
    f32 dist_heat{};
    f32 dist_rain{};
    f32 dist_grad{};
    f32 dist_cont{};
    void update(BiomeMatchTable target, const NoiseParams& noise){
        dist_heat = target.heat.dist_to(noise.heat);
        dist_rain = target.rain.dist_to(noise.rain);
        dist_grad = target.grad.dist_to(noise.grad);
        dist_cont = target.cont.dist_to(noise.cont);
        heat_score = BiomeClassification::calc_score(dist_heat) * 100.0f;
        rain_score = BiomeClassification::calc_score(dist_rain) * 100.0f;
        grad_score = BiomeClassification::calc_score(dist_grad) * 100.0f;
        cont_score = BiomeClassification::calc_score(dist_cont) * 100.0f;
    }
    constexpr static f32 calc_score (f32 dist){
        constexpr static f32 max_dist = NOISE_MAX-NOISE_MIN;
        f32 dist_ratio = dist / max_dist;
        return 1.0f - dist_ratio;
    };
    constexpr f32 avg_score() const noexcept{
        return (heat_score+rain_score+grad_score+cont_score)/4.0f;
    }
    constexpr f32 avg_dist() const noexcept{
        return (dist_heat+dist_rain+dist_grad+dist_cont)/4.0f;
    }
};
inline auto classify_biome_verbose(const NoiseParams& noise){
    auto min_dist = numeric_max<f32>();
    BiomeClassification res;
    for (const auto& target: biome_match_tables){
        res.update(target, noise);
        f32 dist = res.avg_dist();
        if (dist < min_dist){
            min_dist = dist;
            res.id = target.biome;
        }
    }
    return res;
}
inline auto classify_biome(const NoiseParams& noise){
    auto min_dist = numeric_max<f32>();
    BiomeID res;
    for (const auto& target: biome_match_tables){
        f32 dist_heat = target.heat.dist_to(noise.heat);
        f32 dist_rain = target.rain.dist_to(noise.rain);
        f32 dist_grad = target.grad.dist_to(noise.grad);
        f32 dist_cont = target.cont.dist_to(noise.cont);
        f32 dist = (dist_heat+dist_rain+dist_grad+dist_cont)/4.0f;
        if (dist < min_dist){
            min_dist = dist;
            res = target.biome;
        }
    }
    return res;
}

inline auto classify_chunk_biomes(const ArrayList2D<NoiseParams>& noise_list){
    assert(noise_list.extentX == ChunkInfo::XWIDTH);
    assert(noise_list.extentZ == ChunkInfo::ZWIDTH);
    ArrayList2D<BiomeID> biome_list(ChunkInfo::Extents2D);
    ForEachInRangeEx({0,0},ChunkInfo::Extents2D,[&](i32 cx, i32 cz){
        biome_list[cx,cz] = classify_biome(noise_list[cx,cz]);
    });
    return biome_list;
}
