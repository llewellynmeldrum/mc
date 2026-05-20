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
#include "CommonUtils.hpp"
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
#include "lmath.hpp"


struct PendingBlockWrite{
    BlockType block;
    WorldBlockPos blockPos;
    u8 priority;
    WorldChunkCoord sourceChunkCoord;
    PendingBlockWrite(const WorldChunkCoord _sourceChunkCoord, const WorldBlockPos& _blockPos, BlockType bt);

    PendingBlockWrite() = delete;
    ~PendingBlockWrite() = default;

    inline bool operator==(this const PendingBlockWrite& lhs, const PendingBlockWrite& rhs){
        if (lhs.priority==rhs.priority) return true;
        else return lhs.sourceChunkCoord == rhs.sourceChunkCoord;
    }
    inline auto operator<=>(this const PendingBlockWrite& lhs, const PendingBlockWrite& rhs){
        auto cmp1 = lhs.priority<=>rhs.priority;
        if ( cmp1 != 0) return cmp1; //NOLINT
        // TIE BREAK DETERMINISTICALLY BASED ON SOURCE CHUNK
        else return lhs.sourceChunkCoord <=> rhs.sourceChunkCoord;
    }
};
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
    bool tryWrite(PendingBlockWrite write);
    std::array<Block,CHUNK_SIZE>data{};
    struct{
        u32 finishedGeneration  : 1 = 0;
        u32 isDirty             : 1 = 1;
        u32 isMeshing           : 1 = 0;
        u32 finishedMeshing     : 1 = 0;
    }flags;

    static constexpr ivec3 Extents = { CHUNK_XWIDTH, CHUNK_HEIGHT, CHUNK_ZWIDTH };


    MD_ACCESS_MACRO(Block,data)

    inline void setBlock(BlockType t, i16 x, i16 y, i16 z) {
        span()[x, y, z] = Block( t );
    }
    inline void setBlock(BlockType t, ChunkBlockPos pos) {
        return setBlock(t, pos.x, pos.y, pos.z); 
    }

    inline void setBlocks(BlockType t, ChunkBlockPos pos1, ChunkBlockPos pos2) {
        ChunkBlockPos min = glm::min(pos1,pos2);
        ChunkBlockPos max = glm::max(pos1,pos2);
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

    inline decltype(auto) at(this auto& self, vec3 pos) {
        return self.at(pos.x, pos.y, pos.z); 
    }

    inline void fillChunk(BlockType t) {
        for (auto& block : data) {
            block = Block(t);
        }
    }
};

// chunkBlockPos->      worldChunkCoord = deleted
// worldChunkCoord->    chunkBlockPos = deleted

// max heap (prio queue) containing block writes
// since we are placing based on priority anyway, im not sure this being a prio queue is even necessary
using PendingWriteQueue = std::priority_queue<PendingBlockWrite>;

// unordered, heap allocated list of block writes
using PendingWriteList = std::vector<PendingBlockWrite>;

//
//

struct ChunkSpan{
    public:
    ChunkSpan(Block* _data):
        _span(_data){}
    ~ChunkSpan() = default;
    bool tryWrite(PendingBlockWrite write);
    using Extents = std::extents<i64, CHUNK_XWIDTH,CHUNK_HEIGHT,CHUNK_ZWIDTH>;
        std::mdspan<Block, Extents>_span;

    auto span(this auto& self){
        return self._span;
    }
    inline decltype(auto) at(this auto& self, i16 x, i16 y, i16 z) {
        return self.span()[x,y,z];
    }
    inline decltype(auto) at(this auto& self, ChunkBlockPos p) {
        return self.span()[p.x,p.y,p.z];
    }

    inline decltype(auto) operator[](this auto& self, i16 x, i16 y, i16 z) {
        return self.span()[x,y,z];
    }
    inline decltype(auto) operator[](this auto& self, ChunkBlockPos p) {
        return self.span()[p.x,p.y,p.z];
    }
};

// [x,y,z] accessible NON OWNING VIEW of chunk block storage.
struct ChunkStore{
private:
    std::vector<Block> buf={};
public:
    ChunkStore(const Chunk& chunk){
        buf.resize(CHUNK_SIZE);
        std::ranges::copy(chunk.data,buf.begin());
    }
    ChunkStore(){
        buf.resize(CHUNK_SIZE);
    }
    ~ChunkStore() = default;
    inline auto data(this auto& self){
        return self.buf.data(); 
    }
    inline decltype(auto) span(this auto& self){
        return std::mdspan(self.data(), CHUNK_XWIDTH, CHUNK_HEIGHT ,CHUNK_ZWIDTH);
    }

    inline decltype(auto) operator[](this auto& self, i16 x, i16 y, i16 z) {
        return self.span()[x, y, z];
    }
    inline decltype(auto) operator[](this auto& self, ChunkBlockPos p) {
        return self.span()[p.x, p.y, p.z];
    }

    inline decltype(auto) at(this auto& self, i16 x, i16 y, i16 z) {
        return self.span()[x, y, z];
    }
    inline decltype(auto) at(this auto& self, ChunkBlockPos p) {
        return self.span()[p.x, p.y, p.z];
    }

    inline auto begin(this auto& self){
        return self.buf.begin(); 
    }
    inline auto end(this auto& self){
        return self.buf.end(); 
    }
    auto& buffer(){ return buf; }

    // implicit conversion to a ChunkSpan. 
    operator ChunkSpan (){
        return {data()};
    }
};


struct GenConfig{
    constexpr static u32 SEA_LEVEL = 64;
};

extern std::atomic<u32> id_counter;
struct ChunkTaskHeader{
    WorldChunkCoord chunkCoord;
    u32 id;
    ChunkTaskHeader(WorldChunkCoord _worldOffset) :
        chunkCoord(_worldOffset),
        id(id_counter.fetch_add(1)){}
};
// QUEUE: GenJobQueue
// PRODUCER: Main Thread
// CONSUMER: Generator Thread
struct GenJob{
    ALL_CTORS_DEFAULT(GenJob);
    GenJob(WorldChunkCoord _chunkCoord, u64 _worldSeed, GenConfig _cfg) :
        head(_chunkCoord),
        worldSeed(_worldSeed),
        cfg(_cfg){}
    ChunkTaskHeader head;
    u64 worldSeed;
    GenConfig cfg;
};

// QUEUE: GenResultQueue
// PRODUCER: Generator Thread
// CONSUMER: Main Thread
struct GenResult{
    GenResult(WorldChunkCoord _chunkCoord) :
        head(_chunkCoord) {}
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
        WorldChunkCoord _chunkCoord,
        const Chunk* _chunk_ptr, 
        std::array<const Chunk*, 6> _surroundingChunkPtrs,
        const ChunkMetadata* _meta,
        const TextureAtlas* _atlas)
        :
        head(_chunkCoord),
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



