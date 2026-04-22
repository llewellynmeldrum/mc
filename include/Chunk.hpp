#pragma once 
#include "Types.h"
#include "Block.hpp"
#include <mdspan>
constexpr u16 CHUNK_EXTENT = 8; // x/y/z
constexpr u16 CHUNK_SIZE = 8*8*8; // x/y/z
struct Chunk{
    std::array<Block, CHUNK_SIZE> data; // all blocks are implicitly 0, i.e air
    inline Block& operator[](u8 x, u8 y, u8 z){
        return std::mdspan(data.data(), CHUNK_EXTENT, CHUNK_EXTENT, CHUNK_EXTENT)[x,y,z];
    }
    constexpr inline void placeBlock(this auto& self, BlockType t, u8 x, u8 y, u8 z){
        self[x,y,z]=Block{t};
    }
};
