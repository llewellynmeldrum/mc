#pragma once
#include <mdspan>
#include <queue>
#include <ranges>
#include <unordered_map>
#include <utility>
#include <atomic>

#include "Types.h"
#include "Block.hpp"
#include "cppslop.hpp"
#include "glmWrapper.hpp"
#include "ChunkHelpers.hpp"
#include "Vertex.hpp"
#include "noise/module/perlin.h"
#include "ChunkHelpers.hpp"
// each attribute of a chunks metadata can be 3d indexed by block (operator[x,y,z])
// Contains (currently unused) debug information mostly about biome.
struct ChunkMetadata {
    ChunkMetadata() = default;
    ~ChunkMetadata() = default;
    struct {
        std::array<f32, CHUNK_SIZE> data{};
        MD_ACCESS_MACRO(f32, data)
    } blockTemperature;

    struct {
        std::array<f32, CHUNK_SIZE> data{};
        MD_ACCESS_MACRO(f32, data)
    } blockHumidity;

};
struct Chunk{
    Chunk() = default;
    ~Chunk() = default;
    Chunk(std::span<Block> sp){
        for (const auto& [i, val]: std::views::enumerate(sp)){
            data[i] = std::move(val);
        }
    }
    std::array<Block,CHUNK_SIZE>data{};
    struct{
        u32 finishedGeneration  : 1 = 0;
        u32 isDirty             : 2 = 1;
        u32 isMeshing           : 3 = 0;
        u32 finishedMeshing     : 4 = 0;
    }flags;

    static constexpr ivec3        Extents = { CHUNK_XWIDTH, CHUNK_HEIGHT, CHUNK_ZWIDTH };


    MD_ACCESS_MACRO(Block,data)

    inline void setBlock(this auto& self, BlockType t, i16 x, i16 y, i16 z) {
        self[x, y, z] = Block( t );
    }

    inline void setBlocks(this auto& self, BlockType t, vec3 pos1, vec3 pos2) {
        vec3 min =
            vec3(std::min(pos1.x, pos2.x), std::min(pos1.y, pos2.y), std::min(pos1.z, pos2.z));
        vec3 max =
            vec3(std::max(pos1.x, pos2.x), std::max(pos1.y, pos2.y), std::max(pos1.z, pos2.z));
        for (i64 x = min.x; x < max.x; x++) {
            for (i64 y = min.y; y < max.y; y++) {
                for (i64 z = min.z; z < max.z; z++) {
                    self.setBlock(t, x, y, z);
                }
            }
        }
    }
    // @brief sets a column of blocks starting at origin of size (1xheightx1) to t
    inline void setColumn(this auto& self, BlockType t, ivec3 origin, i32 height) {
        for (i64 y = origin.y; y < origin.y + height; y++) {
            self.setBlock(t, origin.x, y, origin.z);
        }
    }
    inline void setBlock(BlockType t, vec3 pos) { return setBlock(t, pos.x, pos.y, pos.z); }

    inline Block getBlock(this auto& self, i16 x, i16 y, i16 z) {
        if (x < 0 || x >= CHUNK_XWIDTH)
            return AIR_BLOCK;
        if (y < 0 || y >= CHUNK_HEIGHT)
            return AIR_BLOCK;
        if (z < 0 || z >= CHUNK_ZWIDTH)
            return AIR_BLOCK;
        return self[x, y, z];
    }
    inline Block getBlock(this auto& self, vec3 pos) { return self.getBlock(pos.x, pos.y, pos.z); }
    inline void  fillChunk(BlockType t) {
        for (auto& block : data) {
            block = Block(t);
        }
    }
};

auto operator<=>(const ivec3& lhs, const ivec3& rhs){
    auto cmp_x = lhs.x<=>rhs.x;
    if ( cmp_x != nullptr) return cmp_x;

    auto cmp_y = lhs.x<=>rhs.x;
    if ( cmp_y != nullptr) return cmp_y;

    return lhs.z<=>rhs.z;
}
struct PendingBlockWrite{
    BlockType block;
    ivec3 worldPos;
    u8 priority;
    ivec3 sourceChunkWorldPos;
    PendingBlockWrite() = delete;
    PendingBlockWrite(const ivec3 sourceChunkwpos, const ivec3& targetWPos, BlockType bt);
    ~PendingBlockWrite() = default;
    bool operator==(this PendingBlockWrite&& lhs, PendingBlockWrite&& rhs){
        return lhs.priority==rhs.priority;
    }
    auto operator<=>(this PendingBlockWrite&& lhs, PendingBlockWrite&& rhs){
        auto cmp1 = lhs.priority<=>rhs.priority;
        if ( cmp1 != nullptr) return cmp1;
        // TIE BREAK DETERMINISTICALLY BASED ON SOURCE CHUNK
        else return lhs.sourceChunkWorldPos <=> rhs.sourceChunkWorldPos;
    }
};
// max heap (prio queue) containing block writes
// since we are placing based on priority anyway, im not sure this being a prio queue is even necessary
using PendingWriteQueue = std::priority_queue<PendingBlockWrite>;

// unordered, heap allocated list of block writes
using PendingWriteList = std::vector<PendingBlockWrite>;

//
//

template<
std::size_t xExtent=CHUNK_XWIDTH,
std::size_t yExtent=CHUNK_HEIGHT,
std::size_t zExtent=CHUNK_ZWIDTH>
struct BaseBlockSpan{
    using Extents = std::extents<std::size_t, xExtent,yExtent,zExtent>;
    inline Block& operator[](this auto& self, i16 x, i16 y, i16 z) {
        return std::mdspan(self.data.data(), CHUNK_XWIDTH, CHUNK_HEIGHT, CHUNK_ZWIDTH)[x, y, z];
    }
    inline Block& operator[](this auto& self, ivec3 v) {
        return std::mdspan(self.data.data(), CHUNK_XWIDTH, CHUNK_HEIGHT, CHUNK_ZWIDTH)[v.x, v.y, v.z];
    }
    BaseBlockSpan() = default;
    ~BaseBlockSpan() = default;
    auto begin(){
        return span.begin();
    }
    auto end(){
        return span.end();
    }
    auto& data(){
        return span;
    }
    bool tryWrite(PendingBlockWrite write);
private:
    std::mdspan<Block, Extents>span={};
};

// [x,y,z] accessible NON OWNING VIEW of chunk block storage.
template<
std::size_t xExtent=CHUNK_XWIDTH,
std::size_t yExtent=CHUNK_HEIGHT,
std::size_t zExtent=CHUNK_ZWIDTH,
std::ranges::contiguous_range Cont = std::vector<Block>>
struct BaseBlockStore{
    BaseBlockStore() = default;
    ~BaseBlockStore() = default;
    using Extents = std::extents<std::size_t, xExtent,yExtent,zExtent>;
    inline Block& operator[](this auto& self, i16 x, i16 y, i16 z) {
        return std::mdspan(self.data.data(), CHUNK_XWIDTH, CHUNK_HEIGHT, CHUNK_ZWIDTH)[x, y, z];
    }
    inline Block& operator[](this auto& self, vec3 v) {
        return std::mdspan(self.data.data(), CHUNK_XWIDTH, CHUNK_HEIGHT, CHUNK_ZWIDTH)[v.x, v.y, v.z];
    }
    auto begin(){ return buf.begin(); }
    auto end(){ return buf.end(); }
    auto& data(){ return buf; }
    // implicit conversion to a BlockSpan. 
    operator BaseBlockSpan<xExtent,yExtent,zExtent> (this auto& self){
        auto&& span = std::mdspan(self.data.data(), CHUNK_XWIDTH, CHUNK_HEIGHT, CHUNK_ZWIDTH);
        BaseBlockSpan<xExtent,yExtent,zExtent>(std::move(span));
    }
private:
    Cont buf={};
};
/// [x,y,z] accessible Inplace (std::array) block storage. No heap allocation.
//  Default initialized to air
using BlockStoreIP = BaseBlockStore<CHUNK_XWIDTH,CHUNK_HEIGHT,CHUNK_ZWIDTH, std::array<Block,CHUNK_SIZE>>;

// [x,y,z] accessible non stack block storage. (likely) Incurs heap allocation.
// Default initialized to air
using BlockStore = BaseBlockStore<CHUNK_XWIDTH, CHUNK_HEIGHT, CHUNK_ZWIDTH, std::vector<Block>>;

using BlockSpan = BaseBlockSpan<CHUNK_XWIDTH, CHUNK_HEIGHT, CHUNK_ZWIDTH>;

//auto operator==(const ivec3& lhs, const ivec3& rhs){
//    return lhs.x==rhs.x;
//}

// noisecontrollers control your body
struct GenConfig{
    struct NoiseController{
        noise::module::Perlin height;
    }noise;
};

extern std::atomic<u32> id_counter;
struct ChunkTaskHeader{
    ivec3 worldOffset;
    u32 id;
    ChunkTaskHeader(ivec3 _worldOffset) 
        :worldOffset(std::move(_worldOffset))
        ,id(id_counter.fetch_add(1))
    {}
};
// QUEUE: GenJobQueue
// PRODUCER: Main Thread
// CONSUMER: Generator Thread
struct GenJob{
    ChunkTaskHeader head;
    u64 worldSeed;
    GenConfig cfg;
};

// QUEUE: GenResultQueue
// PRODUCER: Generator Thread
// CONSUMER: Main Thread
struct GenResult{
    ChunkTaskHeader head;
    BlockStoreIP chunkBlocks;
    ChunkMetadata meta;
    PendingWriteList deferredWrites; // for if a leaf from a tree in chunk generates outside the chunk.
};

// QUEUE: MeshJobQueue
// PRODUCER: Main Thread
// CONSUMER: Mesher thread.
FORWARD_DECL_STRUCT(TextureAtlas)
struct MeshJob{
    ChunkTaskHeader head;
    BlockStore blocks;
    std::array<BlockStore,6> surroundingChunks;
    ChunkMetadata meta;
    TextureAtlas* atlas;

    MeshJob(ivec3 worldLocal, const Chunk* chunk, auto surroundingChunks, auto meta, auto* atlas){
        this->head.worldOffset= worldLocal;
        std::ranges::copy(chunk->data, blocks.begin());
        this->surroundingChunks = std::move(surroundingChunks);
        this->meta = std::move(meta);
        this->atlas = atlas;
    }
};

// QUEUE: MeshResultQueue
// PRODUCER: Mesher Thread
// CONSUMER: Main thread.
struct MeshResult{
    ChunkTaskHeader head;
    std::vector<Vertex> vertices;
    std::vector<u32> indices;
};


