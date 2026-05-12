#pragma once
#include "Concurrency.hpp"
#include "Mesh.hpp"
#include "Chunk.hpp"
#include "TextureAtlas.hpp"

struct World;
struct ChunkMesher {
    ChunkMesher() = default;
    ~ChunkMesher() = default;
    void setupChunkMesher(TextureAtlas& atlas){
        mesherThreads.launch(
            meshChunks,
            std::ref(toBeMeshed),
            std::ref(chunkMeshData),
            std::ref(atlas)
        );
    }

    Queue<ChunkSnapshot> toBeMeshed;
    Queue<ChunkMeshData> chunkMeshData;
private:
    ThreadPool mesherThreads{6};
    static void meshChunks( std::stop_token stopToken, Queue<ChunkSnapshot>& input_queue, Queue<ChunkMeshData>& output_queue, TextureAtlas& atlas);
};
