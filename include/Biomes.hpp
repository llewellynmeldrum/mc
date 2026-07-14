#pragma once 
#include "Block.hpp"
#include "Breakpoints.hpp"
#include "WorldGen_Config.hpp"
#include "Types.h"
#include <utility>
// Biomes should describe ECOLOGY / climate, and feature palettes.
#define LIST_OF_BIOMES \
X(Ocean)\
X(Plains)\
X(Tundra)\
X(Desert)\
X(Beach)\

enum struct BiomeType{
#define X(val, ...) val,
LIST_OF_BIOMES
#undef X
    COUNT
};

struct BiomeDef{
    std::string_view name; // set by macro
    BiomeType biome_type; // set by macro
};

inline constexpr std::array<BiomeDef, std::to_underlying(BiomeType::COUNT)> biome_info{
#define X(var, ...) BiomeDef {.name=#var, .biome_type=BiomeType:: var},
    LIST_OF_BIOMES
#undef X
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
        BlockType::AIR,
        BlockType::AIR,
        BlockType::AIR,
        BlockType::AIR,
    };
};

inline constexpr FeaturePalette default_beach_palette{
    .topsoil = BlockType::SAND_BLOCK,
    .soil = BlockType::DIRT_BLOCK,
    .crust = BlockType::STONE_BLOCK,
    .river_bed = BlockType::SAND_BLOCK,
    .wood_log = BlockType::PALM_LOG,
    .leaves = BlockType::PALM_LEAF,
    .air = BlockType::AIR,
    .water = BlockType::WATER_BLOCK,
    .grass = {
        BlockType::BUSH,
        BlockType::BUSH,
        BlockType::BUSH,
        BlockType::BUSH,
    },
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
inline constexpr FeaturePalette default_tundra_palette{
    .topsoil = BlockType::SNOW_GRASS_BLOCK,     
    .soil = BlockType::DIRT_BLOCK,        
    .crust = BlockType::STONE_BLOCK,      
    .river_bed = BlockType::DIRT_BLOCK, 
    .wood_log = BlockType::SPRUCE_LOG,
    .leaves = BlockType::SPRUCE_LEAF,
    .air = BlockType::AIR,
    .water = BlockType::WATER_BLOCK,
    .grass={
        BlockType::GRASS_TUFT0,
        BlockType::GRASS_TUFT1,
        BlockType::GRASS_TUFT2,
        BlockType::GRASS_TUFT3 
    },
};

inline constexpr FeaturePalette default_ocean_palette{
    .topsoil = BlockType::DIRT_BLOCK,     
    .soil = BlockType::DIRT_BLOCK,        
    .crust = BlockType::STONE_BLOCK,      
    .water = BlockType::WATER_BLOCK,
    .grass={
        BlockType::GRASS_TUFT0,
        BlockType::GRASS_TUFT1,
        BlockType::GRASS_TUFT2,
        BlockType::GRASS_TUFT3 
    },
};

inline std::unordered_map<BiomeType, FeaturePalette> biome_palette = {
    {BiomeType::Ocean, default_ocean_palette},
    {BiomeType::Plains, default_plains_palette},
    {BiomeType::Desert, default_desert_palette},
    {BiomeType::Tundra, default_tundra_palette},
    {BiomeType::Beach, default_beach_palette},
};
inline const auto& get_palette(BiomeType biome){
    auto it = biome_palette.find(biome);
    if (it == biome_palette.end()){
        BREAKPOINT();
    }else{
        return it->second;
    }
}

// FORMAT SPECIALISATION
template<> 
struct std::formatter<BiomeType>{
	constexpr auto parse(std::format_parse_context& ctx){return ctx.begin();}
	auto format(const BiomeType& val, std::format_context& ctx)const {
        const auto& info = biome_info[std::to_underlying(val)];
		return std::format_to(ctx.out(), "{}", info.name);
    }
};
