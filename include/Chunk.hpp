#pragma once
#include <functional>
#include <mdspan>
#include <queue>
#include <ranges>
#include <unordered_map>
#include <utility>
#include <atomic>
#include <vector>

#include "CommonUtils.hpp"
#include "Types.h"
#include "cppslop.hpp"
#include "AnsiCodes.hpp"

#include "Vertex.hpp"
#include "Block.hpp"

#include "ChunkHelpers.hpp"
#include "ChunkStorage.hpp"
#include "ChunkHelpers.hpp"


#include "glbinding/gl/enum.h"
#include "glmWrapper.hpp"

// Contains (currently unused) debug information mostly about biome.
struct ChunkMetadata {
    ChunkMetadata() = default;
    ~ChunkMetadata() = default;
    struct {
        std::array<f32, CHUNK_SIZE> data{};
        inline auto span(this auto& self){
            return std::mdspan(self.storage.data(), CHUNK_XWIDTH, CHUNK_HEIGHT, CHUNK_ZWIDTH);
        }
        inline decltype(auto) operator[](this auto& self, i16 x, i16 y, i16 z) {
            return self.span()[x, y, z];
        }
    } blockTemperature;

    struct {
        std::array<f32, CHUNK_SIZE> data{};
        inline auto span(this auto& self){
            return std::mdspan(self.storage.data(), CHUNK_XWIDTH, CHUNK_HEIGHT, CHUNK_ZWIDTH);
        }
        inline decltype(auto) operator[](this auto& self, i16 x, i16 y, i16 z) {
            return self.span()[x, y, z];
        }
    } blockHumidity;

};

struct Chunk{
    Chunk() = default;
    ~Chunk() = default;

    std::array<Block,CHUNK_SIZE>data{};

    Chunk(std::span<Block> sp){
        for (const auto& [i, val]: std::views::enumerate(sp)){
            data[i] = std::move(val);
        }
    }
    constexpr inline bool isAllAir()const noexcept{
        for (const auto& block: data){
            if (!block.isAir()) return false;
        }
        return true;
    }
    bool tryWrite(PendingBlockWrite write);
    struct{
        u32 finishedGeneration  : 1 = 0;
        u32 isDirty             : 1 = 1;
        u32 isMeshing           : 1 = 0;
        u32 finishedMeshing     : 1 = 0;
    }flags;
    auto begin(){
        return data.begin();
    }
    auto end(){
        return data.end();
    }

    static constexpr glm::ivec3 Extents = { CHUNK_XWIDTH, CHUNK_HEIGHT, CHUNK_ZWIDTH };


    inline decltype(auto) span(this auto& self){
        return std::mdspan(self.data.data(), CHUNK_XWIDTH, CHUNK_HEIGHT, CHUNK_ZWIDTH);
    }
    inline decltype(auto) operator[](this auto& self, i16 x, i16 y, i16 z) {
        return self.span()[x, y, z];
    }

    inline void setBlock(BlockType t, i16 x, i16 y, i16 z) {
        span()[x, y, z] = Block( t );
    }
    inline void setBlock(BlockType t, ChunkBlockPos pos) {
        return setBlock(t, pos.x, pos.y, pos.z); 
    }

    inline void setBlocks(BlockType t, ChunkBlockPos pos1, ChunkBlockPos pos2) {
        auto min = LM::min(pos1,pos2);
        auto max = LM::max(pos1,pos2);
        for (auto [x,y,z] : EachInRange(min,max)){
            setBlock(t, x, y, z);
        }
    }

    // @brief sets a column of blocks starting at origin of size (1xheightx1) to t
    inline void setColumn(BlockType t, ChunkBlockPos origin, i32 height) {
        for (auto y: EachInRange(origin.y, origin.y+height)) {
            setBlock(t, origin.x, y, origin.z);
        }
    }


    inline decltype(auto) at(this auto& self, i32 x, i32 y, i32 z) {
        if (x < 0 || x >= CHUNK_XWIDTH||y < 0 || y >= CHUNK_HEIGHT|| z < 0 || z >= CHUNK_ZWIDTH){
            throw std::out_of_range("Chunk coordinates requested by .at() call are out of range!");
        }
        return self[x, y, z];
    }

    inline decltype(auto) at(this auto& self, ChunkBlockPos pos) {
        return self.at(pos.x, pos.y, pos.z); 
    }

    inline void fillChunk(BlockType t) {
        for (auto& block : data) {
            block = Block(t);
        }
    }
};

