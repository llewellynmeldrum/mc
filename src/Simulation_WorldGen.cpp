#include "Engine.hpp"



std::size_t Engine::enqueueGenerationJobs(std::span<WorldChunkCoord> candidates){
    profiler.bench_start("enqueueGen");
    std::size_t count{};
    for (const auto& candidate_coord: candidates){
        count += scheduler.submit_gen_request(candidate_coord) ? 1 : 0;

    }
    profiler.bench_end("enqueueGen");
    return count;
}
std::size_t Engine::drainAndUploadGenResults(std::size_t maxUploads){
    profiler.bench_start("drainGen");

    auto genResults = drain_queue(world.chunkMap.generator.genResultQueue,maxUploads);
    
    for (const auto& newGen : genResults){
        world.chunkMap.entries.if_contains(
            newGen.chunkCoord,
            [&](ChunkEntry& entry){
                if (entry.qualifies_for_gen_dequeue()){
                    entry.state_transition(gen_dequeue);
                    world.chunkMap.uploadGeneratedChunk(newGen);
                    world.chunkMap.mark_mesh_dirty(newGen.chunkCoord); // allow for meshing
                    return true;
                }else{
                    LOG_DEBUG("(gen_upload): Discarded homeless chunk gen data @{}.", newGen.chunkCoord);
                    return false;
                }
            }
        );
    }
    profiler.bench_end("drainGen");
    return genResults.size();
}
