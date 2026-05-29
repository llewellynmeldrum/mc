#pragma once 

#include <queue>
#include <atomic>

#include "ChunkConstants.hpp"
#include "ChunkEntry.hpp"
#include "cppslop.hpp"
#include "CoordTypes.hpp"

#include "Chunk.hpp"
#include "ChunkStorage.hpp"
#include "PendingBlockWrites.hpp"
#include "cppslop.hpp"




struct GenConfig{
    i32 SEA_LEVEL = 140;
    f32 cave_air_threshold = -0.55;
    i32 cave_y_threshold = 150;
    i32 MAX_ELEVATION_DELTA = 32;
    f32 tree_place_threshold=0.01;
    i32 TERRAIN_HEIGHT(){return SEA_LEVEL+16;}
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
    std::vector<ChunkStore> surroundingChunks{6,ChunkStore{}};
    ChunkMetadata meta;
    const TextureAtlas* atlas;

    MeshJob(WorldChunkCoord key, const TextureAtlas* _atlas, const ChunkEntry* entry):

        meshGeneration(entry->latest_mesh_revision),
        chunkCoord(key),
        blocks(&entry->block_data),
        meta(entry->metadata),
        atlas(_atlas)
    {
        assert(surroundingChunks.size()==N_NEIGHBOURS);
        for (const auto [i,neighbour_ptr] : std::views::enumerate(entry->neighbours)){
            if (!neighbour_ptr) continue;

            surroundingChunks[i] = *neighbour_ptr;
        }
    }
};

struct OpaqueMeshData{
    std::vector<Vertex> vertices{};
    std::vector<u32> indices{};
};

struct TransparentMeshData{
    std::vector<Vertex> vertices{};
    std::vector<u32> indices{};
};

// QUEUE: MeshResultQueue
// PRODUCER: Mesher Thread
// CONSUMER: Main thread.
struct MeshResult{
    std::size_t meshGeneration;
    WorldChunkCoord chunkCoord;
    OpaqueMeshData opaque;
    TransparentMeshData transparent;
};



STD_HASH_SPECIALIZATION(MeshJob, job, 
    return std::hash<glm::ivec3>{}(job.chunkCoord.raw());
)
STD_HASH_SPECIALIZATION(MeshResult, res, 
    return std::hash<glm::ivec3>{}(res.chunkCoord.raw());
)
STD_HASH_SPECIALIZATION(GenJob, job, 
    return std::hash<glm::ivec3>{}(job.chunkCoord.raw());
)
STD_HASH_SPECIALIZATION(GenResult, res, 
    return std::hash<glm::ivec3>{}(res.chunkCoord.raw());
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
