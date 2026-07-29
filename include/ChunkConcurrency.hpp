#pragma once 

#include <queue>
#include <atomic>

#include "BenchmarkMap.hpp"
#include "ChunkConstants.hpp"
#include "ChunkEntry.hpp"
#include "ChunkNoiseDebug.hpp"
#include "Concurrency.hpp"
#include "Vertex.hpp"
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


struct ChunkBenchContext{
    ConcurrentChunkBenchmarker& work;
    ConcurrentChunkBenchmarker& job_idle;
    ConcurrentChunkBenchmarker& res_idle;
};

FORWARD_DECL_STRUCT(ChunkMap)
struct LightingJob{
    WorldChunkCoord coord;
    RevisionState::ID rev;
    std::vector<ChunkLightSlice> neighbour_light_slices;
    std::vector<ChunkBlockSlice> neighbour_block_slices;
    ChunkLightStore light_data;
    ChunkBlockStore block_data;
    LightingJob(
        WorldChunkCoord _coord, 
        ChunkMap const* chunk_map,
        ChunkEntry const* entry
    );
};
struct LightingResult{
    RevisionState::ID rev;
    ChunkLightStore lights;
};


// QUEUE: GenJobQueue
// PRODUCER: Main Thread
// CONSUMER: Generator Thread
struct GenJob{
    ChunkBenchContext bench;
    size_t genRevisionID;
    WorldChunkCoord chunkCoord;
    const GenConfig cfg;
};

// QUEUE: GenResultQueue
// PRODUCER: Generator Thread
// CONSUMER: Main Thread
struct GenResult{
    size_t genRevisionID;
    WorldChunkCoord chunkCoord;
    ChunkBlockStore chunkBlocks;
    PendingWriteList deferredWrites; // for if a leaf from a tree in chunk generates outside the chunk.
};

// QUEUE: MeshJobQueue
// PRODUCER: Main Thread
// CONSUMER: Mesher thread.
FORWARD_DECL_STRUCT(TextureAtlas)
struct MeshJob{
    using SurroundingChunkStore = std::vector<std::optional<ChunkBlockSlice>>;
    ChunkBenchContext bench;
    size_t meshRevisionID;
    WorldChunkCoord chunkCoord;
    ChunkBlockStore blocks;
    ChunkLightStore light_data;
    std::vector<ChunkBlockSlice> surrounding_chunks_block_slices;
    std::vector<ChunkLightSlice> surrounding_chunks_light_slices;
    const_span<TextureAtlas*> atlas_map;


    MeshJob(
        ChunkBenchContext bench,
        WorldChunkCoord key, 
        const_span<TextureAtlas*> _atlas_list, 
        const ChunkMap* chunk_map,
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
    size_t revisionID;
    WorldChunkCoord chunkCoord;
    OpaqueMeshData opaque;
    BlendedMeshData blended;
    CutoutMeshData cutout;
};



template<typename JobType, typename ResType, size_t thread_count=1>
struct JobProcessor{
    JobProcessor() = default;
    ~JobProcessor() = default;

    template<typename Fn, typename ...Args>
        requires callable_with<Fn, std::stop_token, Queue<JobType>&, Queue<ResType>&>
    inline void launch_threads(Fn&& work_fn){
        threads.launch(
            std::forward<Fn>(work_fn),
            std::ref(job_queue),
            std::ref(res_queue)
        );
    }

    Queue<JobType> job_queue;
    Queue<ResType> res_queue;
    ThreadPool threads{thread_count};
};
