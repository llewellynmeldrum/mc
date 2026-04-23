
#include "World.hpp"

#include "DebugFormatSpecializations.hpp"
#include "Logger.hpp"
#include <print>

Chunk& World::getChunk(vec3 world_pos){
    ChunkCoordinate cpos = Chunk::worldToChunkPos(world_pos);
    if (!chunkMap.contains(cpos)){
        LOG_ERROR("Tried to access chunk which doesnt exist!");
        LOG_ERROR("Chunk at {} chunk:({}) does not exist!",dbg_fmt(world_pos),dbg_fmt(cpos));
        LOG_ERROR("Generated chunks: (n={})",chunkMap.size());
        for (auto [cpos, chunk]: chunkMap){
            std::println("\t{} : {}",dbg_fmt(cpos),(void*)&chunk);
        }
        DEBUG_BREAKPOINT();
    }
    return chunkMap.at(cpos);
}
Chunk& World::getChunk(ChunkCoordinate cpos){
    if (!chunkMap.contains(cpos)){
        LOG_ERROR("Tried to access chunk which doesnt exist!");
        LOG_ERROR("Chunk:({}) does not exist!",dbg_fmt(cpos));
        LOG_ERROR("Generated chunks: (n={})",chunkMap.size());
        for (auto [cpos, chunk]: chunkMap){
            std::println("\t{} : {}",dbg_fmt(cpos),(void*)&chunk);
        }
        DEBUG_BREAKPOINT();
    }
    return chunkMap.at(cpos);
}
