#pragma once 
#include <exception>
#include "ChunkConcurrency.hpp"
#include "Concurrency.hpp"
#include "CoordTypes.hpp"
enum struct ChunkType{
    NeedsGen,
    NeedsMesh,
};

struct GenScheduler {
    GenScheduler() = default;
    ~GenScheduler() = default;
    // has noise maps, etc


    inline void launch(){
        genThreads.launch(
            genChunks,
            std::ref(job_q),
            std::ref(res_q)
        );
    }

    Queue<GenJob> job_q;
    Queue<GenResult> res_q;
    ThreadPool<1> genThreads{"GEN"};
    static void genChunks(std::stop_token stopToken, ThreadTracker& tracker,Queue<GenJob>& input_queue, Queue<GenResult>& output_queue);
};

struct MeshScheduler {
    MeshScheduler() = default;
    ~MeshScheduler() = default;
    void launch(){
        mesherThreads.launch(
            meshChunks,
            std::ref(meshJobQueue),
            std::ref(meshResultQueue)
        );
    }

    Queue<MeshJob> meshJobQueue;
    Queue<MeshResult> meshResultQueue;
    ThreadPool<6> mesherThreads{"MESH"};
    static void meshChunks(std::stop_token stopToken,ThreadTracker& tracker, Queue<MeshJob>& input_queue, Queue<MeshResult>& output_queue);
};

FORWARD_DECL_STRUCT(World)
struct ChunkScheduler{
    std::size_t trim_empty_meshes(std::unordered_set<WorldChunkCoord>& set);
    World& world; 

    void launch(){
        gen.launch();
        mesh.launch();
    }
    GenScheduler gen;
    MeshScheduler mesh;

    bool submit_gen_request(WorldChunkCoord coord);
    bool submit_mesh_request(WorldChunkCoord coord);
    std::vector<WorldChunkCoord> find_gen_ready_chunks(WorldChunkCoord center, glm::ivec3 extents, std::size_t max);
    std::vector<WorldChunkCoord> find_mesh_ready_chunks(WorldChunkCoord center, glm::ivec3 extents, std::size_t max);

    MeshJob construct_mesh_job(WorldChunkCoord coord);
    GenJob construct_gen_job(WorldChunkCoord coord);
    
    template<ChunkType chunk_type>
    std::vector<WorldChunkCoord> find_chunks(WorldChunkCoord center, glm::ivec3 extents, size_t max){
        if constexpr (chunk_type == ChunkType::NeedsGen){
            return find_gen_ready_chunks(center, extents, max);
        }else if constexpr(chunk_type == ChunkType::NeedsMesh){
            return find_mesh_ready_chunks(center, extents, max);
        }else{
            std::terminate();
            return {};
        }
    }
};
