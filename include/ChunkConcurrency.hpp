#pragma once 

#include <queue>
#include <atomic>

#include "cppslop.hpp"
#include "CoordTypes.hpp"

#include "Chunk.hpp"
#include "ChunkStorage.hpp"
#include "PendingBlockWrites.hpp"
#include "cppslop.hpp"




struct GenConfig{
    constexpr static u32 SEA_LEVEL = 64;
};


// QUEUE: GenJobQueue
// PRODUCER: Main Thread
// CONSUMER: Generator Thread
struct GenJob{
    WorldChunkCoord chunkCoord;
    u64 worldSeed;
    GenConfig cfg;
};

// QUEUE: GenResultQueue
// PRODUCER: Generator Thread
// CONSUMER: Main Thread
struct GenResult{
    WorldChunkCoord chunkCoord;
    ChunkStore chunkBlocks;
    ChunkMetadata meta;
    PendingWriteList deferredWrites; // for if a leaf from a tree in chunk generates outside the chunk.
};

// QUEUE: MeshJobQueue
// PRODUCER: Main Thread
// CONSUMER: Mesher thread.
FORWARD_DECL_STRUCT(TextureAtlas)
struct MeshJob{
    std::size_t meshGeneration;
    WorldChunkCoord chunkCoord;
    ChunkStore blocks;
    std::array<ChunkStore,6> surroundingChunks;
    ChunkMetadata meta;
    const TextureAtlas* atlas;

    MeshJob(
        std::size_t meshGeneration,
        WorldChunkCoord _chunkCoord,
        const Chunk* _chunk_ptr, 
        std::span<const Chunk*const > _surroundingChunkPtrs,
        const ChunkMetadata* _meta,
        const TextureAtlas* _atlas)
        :
        chunkCoord(_chunkCoord),
        blocks(*_chunk_ptr)
    {
        for (const auto& [i,chunk_ptr]: std::views::enumerate(_surroundingChunkPtrs)){
            if (chunk_ptr){
                surroundingChunks[i] = ChunkStore{*chunk_ptr};
            }else{
                surroundingChunks[i] = ChunkStore{};
            }
        }
        meta = *_meta;
        atlas = _atlas;
    }
};

// QUEUE: MeshResultQueue
// PRODUCER: Mesher Thread
// CONSUMER: Main thread.
struct MeshResult{
    std::size_t meshGeneration;
    WorldChunkCoord chunkCoord;
    std::vector<Vertex> vertices{};
    std::vector<u32> indices{};
};



STD_HASH_SPECIALIZATION(MeshJob, job, 
    return std::hash<glm::ivec3>{}(job.chunkCoord);
)
STD_HASH_SPECIALIZATION(MeshResult, res, 
    return std::hash<glm::ivec3>{}(res.chunkCoord);
)
STD_HASH_SPECIALIZATION(GenJob, job, 
    return std::hash<glm::ivec3>{}(job.chunkCoord);
)
STD_HASH_SPECIALIZATION(GenResult, res, 
    return std::hash<glm::ivec3>{}(res.chunkCoord);
)

inline bool operator==(const MeshJob& a, const MeshJob& b) noexcept {
    return a.chunkCoord == b.chunkCoord;
}
inline bool operator==(const MeshResult& a, const MeshResult& b) noexcept {
    return a.chunkCoord == b.chunkCoord;
}
inline bool operator==(const GenJob& a, const GenJob& b) noexcept {
    return a.chunkCoord == b.chunkCoord;
}
inline bool operator==(const GenResult& a, const GenResult& b) noexcept {
    return a.chunkCoord == b.chunkCoord;
}
