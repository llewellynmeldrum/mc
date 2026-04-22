#pragma once 
#include "Logger.hpp"
#include "Types.h"
#include "Block.hpp"
#include <mdspan>
constexpr u16 CHUNK_XWIDTH= 8; // x/y/z
constexpr u16 CHUNK_ZWIDTH = 8; // x/y/z
constexpr u16 CHUNK_HEIGHT = 8; // x/y/z
constexpr u16 CHUNK_SIZE = 8*8*8; // x/y/z
struct Chunk{
    std::array<Block, CHUNK_SIZE> data{}; // all blocks are implicitly 0, i.e air
    inline Block& operator[](u8 x, u8 y, u8 z){
        return std::mdspan(data.data(), CHUNK_XWIDTH, CHUNK_HEIGHT, CHUNK_ZWIDTH)[x,y,z];
    }
    inline const Block& operator[](u8 x, u8 y, u8 z) const{
        return std::mdspan(data.data(), CHUNK_XWIDTH, CHUNK_HEIGHT, CHUNK_ZWIDTH)[x,y,z];
    }
    constexpr inline void placeBlock(this auto& self, BlockType t, u8 x, u8 y, u8 z){
        self[x,y,z]=Block{.id=t};
    }
};
