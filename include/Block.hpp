#pragma once 

#include "Types.h"
#include <array>
#include <cmath>
#include <mdspan>
struct BlockType{

};
struct Block{
    BlockDef id; // used to index into the texture atlas, also used to identify its blockType
};

constexpr u16 CHUNK_EXTENT = 8; // x/y/z
constexpr u16 CHUNK_SIZE = 8*8*8; // x/y/z
struct Chunk{
    std::array<Block, CHUNK_SIZE> data{};
    Block& operator[](u8 x, u8 y, u8 z){
        return std::mdspan(data.data(), CHUNK_EXTENT, CHUNK_EXTENT, CHUNK_EXTENT)[x,y,z];
    }
};
int main(){
    Chunk c{};
    Block& b = c[0,0,0];
}
