#pragma once 

#include <queue>
#include <atomic>

#include "ChunkInvariants.hpp"
#include "ChunkEntry.hpp"
#include "ChunkNoiseDebug.hpp"
#include "WorldGen_NoiseGeneration.hpp"
#include "cppslop.hpp"
#include "CoordTypes.hpp"

#include "Chunk.hpp"
#include "ChunkStorage.hpp"
#include "PendingBlockWrites.hpp"
#include "glm/gtx/hash.hpp"
#include "cppslop.hpp"
#include "LM.hpp"

#include "WorldGen_Config.hpp"





// QUEUE: GenJobQueue
// PRODUCER: Main Thread
// CONSUMER: Generator Thread
struct GenJob{
    WorldChunkCoord chunkCoord;
    const GenConfig& cfg;
};

// QUEUE: GenResultQueue
// PRODUCER: Generator Thread
// CONSUMER: Main Thread
struct GenResult{
    WorldChunkCoord chunkCoord;
    ChunkStore chunkBlocks;
    PendingWriteList deferredWrites; // for if a leaf from a tree in chunk generates outside the chunk.
#ifdef CHUNK_NOISE_DEBUG
    PerColumnDebugStore<NoiseParams> noise{};
#endif 
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
    const_span<TextureAtlas*> atlas_map;
#ifdef CHUNK_NOISE_DEBUG
    PerColumnDebugStore<NoiseParams> noise;
#endif 


    MeshJob(
        WorldChunkCoord key, 
        const_span<TextureAtlas*> _atlas_list, 
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

struct BlendedMeshData{
    std::vector<Vertex> vertices{};
    std::vector<u32> indices{};
};

struct CutoutMeshData{
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
    BlendedMeshData blended;
    CutoutMeshData cutout;
};


