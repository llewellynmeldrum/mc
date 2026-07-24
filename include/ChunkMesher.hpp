#pragma once
#include <stop_token>

#include "ChunkConcurrency.hpp"
#include "Concurrency.hpp"
#include "Mesh.hpp"
#include "Chunk.hpp"
#include "TextureAtlas.hpp"
#include "cppslop.hpp"
#include "Assertion.hpp"

FORWARD_DECL_STRUCT(World)
struct ChunkMesher {
    ChunkMesher() = default;
    ~ChunkMesher() = default;
    void launch(){
        mesherThreads.launch(
            mesh_chunks,
            std::ref(meshJobQueue),
            std::ref(meshResultQueue)
        );
    }

    Queue<MeshJob> meshJobQueue;
    Queue<MeshResult> meshResultQueue;
private:
    ThreadPool mesherThreads{7};
    static void mesh_chunks(std::stop_token stopToken, Queue<MeshJob>& input_queue, Queue<MeshResult>& output_queue);
};
