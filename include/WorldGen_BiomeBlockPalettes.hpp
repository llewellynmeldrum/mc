#pragma once 
#include "Biomes.hpp"
#include "WorldGen_BiomeFeatureSets.hpp"
#include "Block.hpp"
struct BiomePalette{
    BlockType topsoil {};
    BlockType soil {};
    BlockType crust {};
};
inline EnumMap<BiomeID, BiomePalette> biome_palettes = {
    {BiomeID::Ocean,  BiomePalette{
        .topsoil = BlockType::SAND_BLOCK, 
        .soil = BlockType::SAND_BLOCK, 
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
