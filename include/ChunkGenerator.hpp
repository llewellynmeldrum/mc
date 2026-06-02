#pragma once 
#include "Chunk.hpp"
#include "ChunkConcurrency.hpp"
#include "Concurrency.hpp"
FORWARD_DECL_STRUCT(Biome)



struct ChunkGenerator {
    ChunkGenerator() = default;
    ~ChunkGenerator() = default;
    // has noise maps, etc


    inline void launchGenThreads(){
        genThreads.launch(
            genChunks,
            std::ref(genJobQueue),
            std::ref(genResultQueue)
        );
    }

    Queue<GenJob> genJobQueue;
    Queue<GenResult> genResultQueue;
    ThreadPool genThreads{1};
    static void genChunks(std::stop_token stopToken, Queue<GenJob>& input_queue, Queue<GenResult>& output_queue);
};
