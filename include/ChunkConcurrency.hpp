#pragma once 

#include <queue>
#include <atomic>

#include "ChunkInvariants.hpp"
#include "ChunkEntry.hpp"
#include "cppslop.hpp"
#include "CoordTypes.hpp"

#include "Chunk.hpp"
#include "ChunkStorage.hpp"
#include "PendingBlockWrites.hpp"
#include "glm/gtx/hash.hpp"
#include "cppslop.hpp"




struct GenConfig{
    i32 SEA_LEVEL = 140;
    f32 cave_air_threshold = -0.55;
    i32 cave_y_threshold = 150;
    i32 MAX_ELEVATION_DELTA = 32;
    f32 tree_distribution_threshold=0.0f;
    f32 tree_place_threshold=1.2f;
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
    PendingWriteList deferredWrites; // for if a leaf from a tree in chunk generates outside the chunk.
};

// QUEUE: MeshJobQueue
// PRODUCER: Main Thread
// CONSUMER: Mesher thread.
FORWARD_DECL_STRUCT(TextureAtlas)
FORWARD_DECL_STRUCT(ChunkMap)
struct MeshJob{
    std::size_t meshRevisionID;
    WorldChunkCoord chunkCoord;
    ChunkStore blocks;
    std::vector<std::optional<ChunkSlice2D>> surroundingChunks;
    const TextureAtlas* atlas;

    MeshJob(
        WorldChunkCoord key, 
        const TextureAtlas* _atlas, 
        ChunkMap* chunk_map,
        const ChunkEntry* entry
    );
    // TODO: to 4-5x reduce the size of a mesh jobs allocation, 
    // i can reduce the surrounding Chunks block storage to only contain the boundary blocks,
    // i.e the ones bordering the actual chunk in question.
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
    std::size_t revisionID;
    WorldChunkCoord chunkCoord;
    OpaqueMeshData opaque;
    TransparentMeshData transparent;
};


