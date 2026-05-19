#pragma once
#include "Concurrency.hpp"
#include "Mesh.hpp"
#include "Chunk.hpp"
#include "TextureAtlas.hpp"
#include "cppslop.hpp"

FORWARD_DECL_STRUCT(World)
struct ChunkMesher {
    ChunkMesher() = default;
    ~ChunkMesher() = default;
    void setupChunkMesher(){
        mesherThreads.launch(
            meshChunks,
            std::ref(meshJobQueue),
            std::ref(meshResultQueue)
        );
    }

    Queue<MeshJob> meshJobQueue;
    Queue<MeshResult> meshResultQueue;
private:
    ThreadPool mesherThreads{6};
    static void meshChunks(std::stop_token stopToken, Queue<MeshJob>& input_queue, Queue<MeshResult>& output_queue);
};
