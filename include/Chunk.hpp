#pragma once 
#include "Types.h"
#include "Block.hpp"
#include <mdspan>
#include "glmWrapper.hpp"
struct Chunk; // forward decl for using defs

using ChunkCoordinate = glm::ivec2;
// A fat chunk stores a chunks position and a ptr to it.
// Mainly used for DS's which map coord->chunk
using FatChunk = std::pair<ChunkCoordinate, Chunk>;

// A ChunkView is a non owning view of a sequence of chunks, paired with their coordinates
using ChunkView = std::pair<ChunkCoordinate, std::unique_ptr<Chunk>>;









struct ChunkCoordHasher{
    std::size_t operator()(const glm::ivec2& v)const{
        return (std::hash<i32>{}(v.x) << 1) ^(std::hash<i32>{}(v.y));
    }
};
constexpr const i64 CHUNK_XWIDTH= 8; // x/y/z
constexpr const i64 CHUNK_ZWIDTH = 8; // x/y/z
constexpr const i64 CHUNK_HEIGHT = 256; // x/y/z
constexpr const i64 CHUNK_SIZE = CHUNK_XWIDTH * CHUNK_ZWIDTH * CHUNK_HEIGHT; // x/y/z

struct Chunk{
    std::array<Block, CHUNK_SIZE> data{}; // all blocks are implicitly 0, i.e air
    inline Block& operator[](u16 x, u16 y, u16 z){
        return std::mdspan(data.data(), CHUNK_XWIDTH, CHUNK_HEIGHT, CHUNK_ZWIDTH)[x,y,z];
    }
    inline const Block& operator[](u16 x, u16 y, u16 z) const{
        return std::mdspan(data.data(), CHUNK_XWIDTH, CHUNK_HEIGHT, CHUNK_ZWIDTH)[x,y,z];
    }
    inline void setBlock(this auto& self, BlockType t, u16 x, u16 y, u16 z){
        self[x,y,z]=Block{.id=t};
    }
    inline void setBlocks(this auto& self, BlockType t, vec3 pos1, vec3 pos2){
        vec3 min = vec3(std::min(pos1.x,pos2.x),std::min(pos1.y,pos2.y),std::min(pos1.z,pos2.z));
        vec3 max = vec3(std::max(pos1.x,pos2.x),std::max(pos1.y,pos2.y),std::max(pos1.z,pos2.z));
        for (u64 x = min.x; x<=max.x; x++){
        for (u64 y = min.y; y<=max.y; y++){
        for (u64 z = min.z; z<=max.z; z++){
                    self.setBlock(t,x,y,z);
        }}}
    }
    inline void setBlock(BlockType t, vec3 pos){
        return setBlock(t,pos.x,pos.y,pos.z);
    }
    inline Block getBlock(this auto& self, u16 x, u16 y, u16 z){
        if (x<0 || x>=CHUNK_XWIDTH) return NULL_BLOCK;
        if (y<0 || y>=CHUNK_HEIGHT) return NULL_BLOCK;
        if (z<0 || z>=CHUNK_ZWIDTH) return NULL_BLOCK;
        return self[x,y,z];
    }
    inline  Block getBlock(this auto& self, vec3 pos){
        return self.getBlock(pos.x,pos.y,pos.z);
    }
    inline void fillChunk(BlockType t){
        for (auto& block: data){
            block=Block{.id=t};
        }
    }
    static ChunkCoordinate worldToChunkPos(const vec3& worldPos);
    static vec3 chunkToWorldPos(const ChunkCoordinate& chunkPos);
};


