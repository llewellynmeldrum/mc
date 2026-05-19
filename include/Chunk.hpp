#pragma once
#include <functional>
#include <mdspan>
#include <queue>
#include <ranges>
#include <unordered_map>
#include <utility>
#include <atomic>
#include <vector>

#include "AnsiCodes.hpp"
#include "Types.h"
#include "Block.hpp"
#include "cppslop.hpp"
#include "glbinding/gl/enum.h"
#include "glmWrapper.hpp"
#include "ChunkHelpers.hpp"
#include "Vertex.hpp"
#include "NoiseSystem.hpp"
#include "ChunkHelpers.hpp"
#include "NoiseSystem.hpp"
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
        u32 isDirty             : 1 = 1;
        u32 isMeshing           : 1 = 0;
        u32 finishedMeshing     : 1 = 0;
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

inline auto operator<=>(const ivec3& lhs, const ivec3& rhs){
    auto cmp_x = lhs.x<=>rhs.x;
    if ( cmp_x != 0) return cmp_x; // NOLINT

    auto cmp_y = lhs.y<=>rhs.y;
    if ( cmp_y != 0) return cmp_y; // NOLINT

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
    bool operator==(this const PendingBlockWrite& lhs, const PendingBlockWrite& rhs){
        if (lhs.priority==rhs.priority) return true;
        else return lhs.sourceChunkWorldPos == rhs.sourceChunkWorldPos;
    }
    auto operator<=>(this const PendingBlockWrite& lhs, const PendingBlockWrite& rhs){
        auto cmp1 = lhs.priority<=>rhs.priority;
        if ( cmp1 != 0) return cmp1; //NOLINT
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

struct ChunkSpan{
    using Extents = std::extents<i64, CHUNK_XWIDTH,CHUNK_HEIGHT,CHUNK_ZWIDTH>;
    private:
        std::mdspan<Block, Extents>span;
    public:
    inline Block& operator[](i16 x, i16 y, i16 z) {
        return span[x,y,z];
    }
    inline Block& operator[](ivec3 v) {
        return span[v.x,v.y,v.z];
    }
    ChunkSpan(Block* _data): span(_data){}
    ~ChunkSpan() = default;
    bool tryWrite(PendingBlockWrite write);
};

// [x,y,z] accessible NON OWNING VIEW of chunk block storage.
struct ChunkStore{
private:
    std::vector<Block> buf={};
public:
    ChunkStore(const Chunk& chunk){
        std::ranges::copy(chunk.data,buf.begin());
    }
    ChunkStore() = default;
    ~ChunkStore() = default;
    using Extents = std::extents<i64, CHUNK_XWIDTH,CHUNK_HEIGHT,CHUNK_ZWIDTH>;
    inline auto span(){
        return std::mdspan(data(), CHUNK_XWIDTH, CHUNK_HEIGHT ,CHUNK_ZWIDTH);
    }
    inline const auto cspan()const{
        return std::mdspan(cdata(), CHUNK_XWIDTH, CHUNK_HEIGHT ,CHUNK_ZWIDTH);
    }
    inline Block& operator[](i16 x, i16 y, i16 z) {
        return span()[x, y, z];
    }
    inline Block& operator[](ivec3 v) {
        return span()[v.x, v.y, v.z];
    }
    inline const Block& at(i16 x, i16 y, i16 z) const{
        return cspan()[x,y,z];
    }
    inline const Block& at(ivec3 v) const{
        return cspan()[v.x,v.y,v.z];
    }
    auto begin(){ return buf.begin(); }
    auto end(){ return buf.end(); }
    Block* data(){ return buf.data(); }
    const Block* cdata()const { return buf.data(); }
    auto& buffer(){ return buf; }

    // implicit conversion to a ChunkSpan. 
    operator ChunkSpan (){
        return {data()};
    }
};
/// [x,y,z] accessible Inplace (std::array) block storage. No heap allocation.
//  Default initialized to air

// [x,y,z] accessible non stack block storage. (likely) Incurs heap allocation.
// Default initialized to air


//auto operator==(const ivec3& lhs, const ivec3& rhs){
//    return lhs.x==rhs.x;
//}

#define    ALL_CTORS_DEFAULT(T)\
T(const T&)=default;\
T(T&&)=default;\
T& operator=(T&&)=default;\
T& operator=(const T&)=default;
struct GenConfig{
    constexpr static u32 SEA_LEVEL = 64;
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
    ALL_CTORS_DEFAULT(GenJob);
    GenJob(ivec3 _worldOffset, u64 _worldSeed, GenConfig _cfg)
        :head(_worldOffset)
        , worldSeed(_worldSeed)
        , cfg(_cfg)
        {}
    ChunkTaskHeader head;
    u64 worldSeed;
    GenConfig cfg;
};

// QUEUE: GenResultQueue
// PRODUCER: Generator Thread
// CONSUMER: Main Thread
struct GenResult{
    GenResult(ivec3 worldOffset) :head(worldOffset) {}
    ChunkTaskHeader head;
    ChunkStore chunkBlocks;
    ChunkMetadata meta;
    PendingWriteList deferredWrites; // for if a leaf from a tree in chunk generates outside the chunk.
};

// QUEUE: MeshJobQueue
// PRODUCER: Main Thread
// CONSUMER: Mesher thread.
FORWARD_DECL_STRUCT(TextureAtlas)
struct MeshJob{
    ChunkTaskHeader head;
    ChunkStore blocks;
    std::array<ChunkStore,6> surroundingChunks;
    ChunkMetadata meta;
    const TextureAtlas* atlas;

    MeshJob(
        ivec3 _worldPos,
        const Chunk* _chunk_ptr, 
        std::array<const Chunk*, 6> _surroundingChunkPtrs,
        const ChunkMetadata* _meta,
        const TextureAtlas* _atlas)
        :
        head(_worldPos),
        blocks(*_chunk_ptr)
    {
        for (const auto& [i,chunk_ptr]: std::views::enumerate(_surroundingChunkPtrs)){
            surroundingChunks[i] = ChunkStore{*chunk_ptr};
        }
        meta = *_meta;
        atlas = _atlas;
    }
};

// QUEUE: MeshResultQueue
// PRODUCER: Mesher Thread
// CONSUMER: Main thread.
struct MeshResult{
    ChunkTaskHeader head;
    std::vector<Vertex> vertices{};
    std::vector<u32> indices{};
};


