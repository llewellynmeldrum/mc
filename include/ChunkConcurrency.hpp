#pragma once 

#include <queue>
#include <atomic>
#include <type_traits>
#include <functional>

#include "ChunkEntryEnums.hpp"
#include "BenchmarkMap.hpp"
#include "ChunkConstants.hpp"
#include "ChunkEntry.hpp"
#include "ChunkNoiseDebug.hpp"
#include "Concurrency.hpp"
#include "Vertex.hpp"
#include "WorldGen_NoiseGeneration.hpp"
#include "cppslop.hpp"
#include "CoordTypes.hpp"

#include "ThreadTracker.hpp"
#include "Chunk.hpp"
#include "ChunkStorage.hpp"
#include "ChunkNeighbourhood.hpp"
#include "PendingBlockWrites.hpp"
#include "glm/gtx/hash.hpp"
#include "cppslop.hpp"
#include "LM.hpp"
#include "CopyOnWrite.hpp"

#include "WorldGen_Config.hpp"



struct ChunkBenchContext{
    ConcurrentChunkBenchmarker& work;
    ConcurrentChunkBenchmarker& job_idle;
    ConcurrentChunkBenchmarker& res_idle;
};

FORWARD_DECL_STRUCT(ChunkMap)
struct LightingJob{
    ChunkBenchContext bench;
    WorldChunkCoord coord;
    RevisionState::ID rev;
    BlockNeighbourhoodSnapshot blocks;
    LightNeighbourhoodSnapshot lights;
    LightingJob(
        ChunkBenchContext bench,
        WorldChunkCoord _coord, 
        ChunkMap const* chunk_map,
        ChunkEntry const* entry
    );
};
struct LightingResult{
    WorldChunkCoord coord;
    RevisionState::ID rev;
    ChunkLightStore lights;
};


// QUEUE: GenJobQueue
// PRODUCER: Main Thread
// CONSUMER: Generator Thread
struct GenJob{
    ChunkBenchContext bench;
    WorldChunkCoord coord;
    RevisionState::ID rev;
    const GenConfig cfg;
};

// QUEUE: GenResultQueue
// PRODUCER: Generator Thread
// CONSUMER: Main Thread
struct GenResult{
    WorldChunkCoord coord;
    RevisionState::ID rev;
    ChunkBlockStore chunk_blocks;
    PendingWriteList deferred_writes; // for if a leaf from a tree in chunk generates outside the chunk.
};

// QUEUE: MeshJobQueue
// PRODUCER: Main Thread
// CONSUMER: Mesher thread.
FORWARD_DECL_STRUCT(TextureAtlas)
struct MeshJob{
    using SurroundingChunkStore = std::vector<std::optional<ChunkBlockSlice>>;
    ChunkBenchContext bench;
    WorldChunkCoord coord;
    RevisionState::ID rev;
    BlockNeighbourhoodSnapshot blocks;
    LightNeighbourhoodSnapshot lights;
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
struct OpaqueMeshDataTag;
struct CutoutMeshDataTag;
struct BlendedMeshDataTag;
template<typename Tag>
struct MeshData{
    MeshData() = default;
    MeshData(std::span<const Vertex> v, std::span<const u32>i )
        :vertices(std::from_range, v)
        ,indices(std::from_range, i)
    {}
    MeshData(MeshData&&) = default;
    MeshData&    operator=(MeshData&&) = default;
    MeshData(const MeshData&) = delete;
    MeshData&    operator=(const MeshData&) = delete;

    std::vector<Vertex> vertices{};
    std::vector<u32> indices{};
};
using OpaqueMeshData = MeshData<OpaqueMeshDataTag>;
using BlendedMeshData = MeshData<BlendedMeshDataTag>;
using CutoutMeshData = MeshData<CutoutMeshDataTag>;


// QUEUE: MeshResultQueue
// PRODUCER: Mesher Thread
// CONSUMER: Main thread.
struct MeshResult{
    RevisionState::ID rev;
    WorldChunkCoord coord;
    OpaqueMeshData opaque;
    BlendedMeshData blended;
    CutoutMeshData cutout;
};



template<JobType jt, typename job_struct, typename res_struct, size_t n_threads=1>
struct JobProcessor{
    template<typename Fn>
        requires callable_with<Fn, std::stop_token, Queue<job_struct>&, Queue<res_struct>&>
    JobProcessor(Fn&& work) :work_function(std::forward<Fn>(work)) {}
    ~JobProcessor() = default;

    constexpr static auto JT = jt;
    constexpr static size_t thread_count = n_threads;
    using work_func_t = std::function<void(std::stop_token, Queue<job_struct>&, Queue<res_struct>&)>;


    work_func_t work_function;

    Queue<job_struct> job_queue;
    Queue<res_struct> res_queue;
    ThreadPool threads{n_threads};

    inline void launch_threads(){
        std::println("launching {} {} threads",threads.count, JT);
        threads.launch(
            work_function,
            std::ref(job_queue),
            std::ref(res_queue)
        );
    }

};

template<JobType JT>
struct job_type_traits{
    using job_t = std::false_type;
    using res_t = std::false_type;
    using queue_t = Queue<job_t>;
};

template<>
struct job_type_traits<JobType::Gen>{
    using job_t = GenJob;
    using res_t = GenResult;
};
template<>
struct job_type_traits<JobType::Mesh>{
    using job_t = MeshJob;
    using res_t = MeshResult;
};
template<>
struct job_type_traits<JobType::Light>{
    using job_t = LightingJob;
    using res_t = LightingResult;
};

template<JobType JT>
using job_struct_t = job_type_traits<JT>::job_t;

template<JobType JT>
using res_struct_t = job_type_traits<JT>::res_t;

