#pragma once 

#include "Types.h"
#include "Mesh.hpp"
#include "Context.hpp"
#include <array>
#include <cmath>
#include <mdspan>
enum class BlockType{
    GRASS_BLOCK,
    DIRT_BLOCK,
};
struct Block{
    BlockType id; 
    constexpr u64 texture_id(){
        return static_cast<u64>(id);
    }
};








constexpr u16 CHUNK_EXTENT = 8; // x/y/z
constexpr u16 CHUNK_SIZE = 8*8*8; // x/y/z
struct Chunk{
    std::array<Block, CHUNK_SIZE> data{};
    Block& operator[](u8 x, u8 y, u8 z){
        return std::mdspan(data.data(), CHUNK_EXTENT, CHUNK_EXTENT, CHUNK_EXTENT)[x,y,z];
    }
    constexpr inline void placeBlock(this auto& self, BlockType t, u8 x, u8 y, u8 z){
        self[x,y,z]=Block{t};
    }
    std::vector<Mesh> build_meshes(){
        return {};
    }
};
