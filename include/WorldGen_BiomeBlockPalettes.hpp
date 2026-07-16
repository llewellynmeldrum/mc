#pragma once 
#include "Biomes.hpp"
#include "WorldGen_BiomeFeatureSets.hpp"
struct BiomePalette{
    BlockType topsoil {BlockType::GRASS_BLOCK};
    BlockType soil {BlockType::DIRT_BLOCK};
    BlockType crust {BlockType::STONE_BLOCK};
};
inline EnumMap<BiomeID, BiomePalette> biome_palettes = {
    {BiomeID::Ocean,  BiomePalette{
        .topsoil = BlockType::DIRT_BLOCK, 
        .soil = BlockType::DIRT_BLOCK, 
        .crust = BlockType::STONE_BLOCK, 
    }},
    {BiomeID::Plains,  BiomePalette{
        .topsoil = BlockType::GRASS_BLOCK, 
        .soil = BlockType::DIRT_BLOCK, 
        .crust = BlockType::STONE_BLOCK, 
    }},
    {BiomeID::Desert,  BiomePalette{
        .topsoil = BlockType::SAND_BLOCK, 
        .soil = BlockType::SAND_BLOCK, 
        .crust = BlockType::STONE_BLOCK, 
    }},
    {BiomeID::Tundra,  BiomePalette{
        .topsoil = BlockType::SNOW_GRASS_BLOCK, 
        .soil = BlockType::DIRT_BLOCK, 
        .crust = BlockType::STONE_BLOCK, 
    }},
    {BiomeID::Beach,  BiomePalette{
        .topsoil = BlockType::SAND_BLOCK, 
        .soil = BlockType::SAND_BLOCK, 
        .crust = BlockType::STONE_BLOCK, 
    }},
};
