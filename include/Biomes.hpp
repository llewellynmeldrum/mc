#pragma once 
#include "Block.hpp"
#include "ChunkGen_config.hpp"
#include "Types.h"
#include <utility>
// Biomes should describe ECOLOGY / climate, and feature palettes.
#define LIST_OF_BIOMES \
X(SNOWY_DESERT,         .nickname="ARCTIC"  , .moisture = {0.0f, 0.3f}, .temp = {0.0f,0.3f}          )  \
X(SNOWY_PLAINS,         .nickname="TUNDRA"  , .moisture = {0.3f, 0.6f}, .temp = {0.0f,0.3f}          )  \
X(SNOWY_FOREST,         .nickname="TAIGA"   , .moisture = {0.6f, 1.0f}, .temp = {0.0f,0.3f}          )  \
\
X(TEMPERATE_DESERT,     .nickname="GOBI"    , .moisture = {0.0f, 0.3f}, .temp = {0.3f,0.8f}        )  \
X(TEMPERATE_PLAINS,     .nickname="PRAIRIE" , .moisture = {0.3f, 0.6f}, .temp = {0.3f,0.8f}        )  \
X(TEMPERATE_FOREST,     .nickname="FOREST"  , .moisture = {0.6f, 0.9f}, .temp = {0.3f,0.8f}        )  \
X(TEMPERATE_RAINFOREST, .nickname="JUNGLE"  , .moisture = {0.9f, 1.0f}, .temp = {0.3f,0.8f}        )  \
\
X(HOT_DESERT,           .nickname="SAHARA"    , .moisture = {0.0f, 0.3f}, .temp = {0.8f, 1.0f}       )  \
X(HOT_PLAINS,           .nickname="SHRUBLAND" , .moisture = {0.3f, 0.6f}, .temp = {0.8f, 1.0f}       )  \
X(HOT_FOREST,           .nickname="SAVANNAH"  , .moisture = {0.6f, 0.9f}, .temp = {0.8f, 1.0f}       )  \
X(HOT_RAINFOREST,       .nickname="AMAZON"  ,   .moisture = {0.9f, 1.0f}, .temp = {0.8f, 1.0f}       )  \
\

enum struct TerrainZone{
    Ocean, 
    Coast,
    Inland,
};

TerrainZone classify_terrain_zone(f32 continentalness){
    if (continentalness > 0.55f){
        return TerrainZone::Inland;
    }else if (continentalness > 0.45f){
        return TerrainZone::Coast;
    }else {// <0.45f
        return TerrainZone::Ocean;
    }
}

enum struct BiomeType{
#define X(val, ...) val,
LIST_OF_BIOMES
#undef X
    COUNT
};

struct MinMax {
    f32 min, max;
};
struct BiomeDef{
    std::string_view name; // set by macro
    BiomeType biome_type; // set by macro
    std::string_view nickname;
    MinMax moisture;
    MinMax temp;
};

inline constexpr std::array<BiomeDef, std::to_underlying(BiomeType::COUNT)> biome_info{
#define X(var, ...) BiomeDef {.name=#var, .biome_type=BiomeType:: var __VA_OPT__(,) __VA_ARGS__}, 
    LIST_OF_BIOMES
#undef X
};

template<> 
struct std::formatter<BiomeType>{
	constexpr auto parse(std::format_parse_context& ctx){return ctx.begin();}
	auto format(const BiomeType& val, std::format_context& ctx)const {
        const auto& info = biome_info[std::to_underlying(val)];
		return std::format_to(ctx.out(), "{} ({})", info.name, info.nickname);
    }
};


struct FeaturePalette {
    BlockType topsoil;
    BlockType soil;
    BlockType crust;
    BlockType river_bed;
    BlockType wood_log;
    BlockType leaves;
    BlockType air;
    BlockType water;
    constexpr static auto n_grass_stages = 4uz;
    const std::array<BlockType, n_grass_stages> grass = {
        BlockType::GRASS_TUFT0,
        BlockType::GRASS_TUFT1,
        BlockType::GRASS_TUFT2,
        BlockType::GRASS_TUFT3 
    };
};

inline constexpr FeaturePalette default_desert_palette{
    .topsoil = BlockType::SAND_BLOCK,
    .soil = BlockType::SANDSTONE,
    .crust = BlockType::STONE_BLOCK,
    .river_bed = BlockType::SANDSTONE,
    .wood_log = BlockType::CACTUS,
    .leaves = BlockType::AIR,
    .air = BlockType::AIR,
    .water = BlockType::WATER_BLOCK,
    .grass = {
        BlockType::DEAD_BUSH,
        BlockType::DEAD_BUSH,
        BlockType::DEAD_BUSH,
        BlockType::DEAD_BUSH,
    },
};
inline constexpr FeaturePalette default_plains_palette {
    .topsoil = BlockType::GRASS_BLOCK,     // i.e grass for plains, NULL for desert (none)
    .soil = BlockType::DIRT_BLOCK,         // i.e dirt for plains, sand for desert.
    .crust = BlockType::SNOW_BLOCK,       // i.e stone for plains, sandstone for desert.
    .river_bed = BlockType::DIRT_BLOCK,  // i.e stone for plains, stone for desert.
    .wood_log = BlockType::OAK_LOG,
    .leaves = BlockType::OAK_LEAF,
    .air = BlockType::AIR,
    .water = BlockType::WATER_BLOCK,
    .grass={
        BlockType::GRASS_TUFT0,
        BlockType::GRASS_TUFT1,
        BlockType::GRASS_TUFT2,
        BlockType::GRASS_TUFT3 
    },
};
inline constexpr FeaturePalette default_forest_palette {
    .topsoil = BlockType::GRASS_BLOCK,     
    .soil = BlockType::DIRT_BLOCK,        
    .crust = BlockType::SNOW_BLOCK,       
    .river_bed = BlockType::DIRT_BLOCK, 
    .wood_log = BlockType::OAK_LOG,
    .leaves = BlockType::OAK_LEAF,
    .air = BlockType::AIR,
    .water = BlockType::WATER_BLOCK,
    .grass={
        BlockType::GRASS_TUFT0,
        BlockType::GRASS_TUFT1,
        BlockType::GRASS_TUFT2,
        BlockType::GRASS_TUFT3 
    },
};
inline constexpr FeaturePalette default_rainforest_palette {
    .topsoil = BlockType::GRASS_BLOCK, 
    .soil = BlockType::DIRT_BLOCK,     
    .crust = BlockType::SNOW_BLOCK,     
    .river_bed = BlockType::DIRT_BLOCK,
    .wood_log = BlockType::OAK_LOG,
    .leaves = BlockType::OAK_LEAF,
    .air = BlockType::AIR,
    .water = BlockType::WATER_BLOCK,
    .grass={
        BlockType::GRASS_TUFT0,
        BlockType::GRASS_TUFT1,
        BlockType::GRASS_TUFT2,
        BlockType::GRASS_TUFT3 
    },
};

inline std::unordered_map<BiomeType, FeaturePalette> biome_palette = {
    {BiomeType::SNOWY_DESERT,       default_desert_palette},
    {BiomeType::TEMPERATE_DESERT,   default_desert_palette},
    {BiomeType::HOT_DESERT,         default_desert_palette},

    {BiomeType::SNOWY_PLAINS,       default_plains_palette},
    {BiomeType::TEMPERATE_PLAINS,   default_plains_palette},
    {BiomeType::HOT_PLAINS,         default_plains_palette},

    {BiomeType::SNOWY_FOREST,       default_forest_palette},
    {BiomeType::TEMPERATE_FOREST,   default_forest_palette},
    {BiomeType::HOT_FOREST,         default_forest_palette},

    //{BiomeType::SNOWY_RAINFOREST,         default_rainforest_palette},
    {BiomeType::TEMPERATE_RAINFOREST,   default_rainforest_palette},
    {BiomeType::HOT_RAINFOREST,         default_rainforest_palette},
};

GenConfig default_desert_gen_cfg{
};
GenConfig default_plains_gen_cfg{
};
GenConfig default_forest_gen_cfg{
};
GenConfig default_rainforest_gen_cfg{
};
inline std::unordered_map<BiomeType, GenConfig> biome_gen_cfg = {
    {BiomeType::SNOWY_DESERT,       default_desert_gen_cfg},
    {BiomeType::TEMPERATE_DESERT,   default_desert_gen_cfg},
    {BiomeType::HOT_DESERT,         default_desert_gen_cfg},

    {BiomeType::SNOWY_PLAINS,       default_plains_gen_cfg},
    {BiomeType::TEMPERATE_PLAINS,   default_plains_gen_cfg},
    {BiomeType::HOT_PLAINS,         default_plains_gen_cfg},

    {BiomeType::SNOWY_FOREST,       default_forest_gen_cfg},
    {BiomeType::TEMPERATE_FOREST,   default_forest_gen_cfg},
    {BiomeType::HOT_FOREST,         default_forest_gen_cfg},

    {BiomeType::TEMPERATE_RAINFOREST,   default_rainforest_gen_cfg},
    {BiomeType::HOT_RAINFOREST,         default_rainforest_gen_cfg},
};

