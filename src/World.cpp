
#include "DebugFormatSpecializations.hpp"
#include "World.hpp"

#include "Chunk.hpp"
#include "DEBUG.hpp"
#include "Logger.hpp"

// returns the origin (0,0,0) of whichever chunk *worldPos* is in.
ivec3 World::worldToChunkCoord(vec3 worldPos) {
    f32   x = std::floor(worldPos.x / CHUNK_XWIDTH);
    f32   y = std::floor(worldPos.y / CHUNK_HEIGHT);
    f32   z = std::floor(worldPos.z / CHUNK_ZWIDTH);
    ivec3 res = { x, y, z };
    return res;
}

// returns the origin (0,0,0) of the chunk at *chunkPos*
vec3 World::chunkToWorldPos(ivec3 chunkPos) {
    f32  x = chunkPos.x * CHUNK_XWIDTH;
    f32  y = chunkPos.y * CHUNK_HEIGHT;
    f32  z = chunkPos.z * CHUNK_ZWIDTH;
    vec3 res = { x, y, z };
    return res;
}

Block World::getBlock(ivec3 world_pos) const {
    ivec3 chunk_origin = World::worldToChunkCoord(world_pos);
    auto  chunk_ptr =
        chunkMap.chunks.contains(chunk_origin) ? &chunkMap.chunks.at(chunk_origin) : nullptr;
    if (chunk_ptr) {
        ivec3 chunk_local = world_pos - chunk_origin * (int)CHUNK_ZWIDTH;
        return chunk_ptr->get()->getBlock(chunk_local);
    } else {
        return Block::Empty();
    }
}

const Chunk* World::ch_getChunk(ivec3 chunk_coord) const {
    if (!chunkMap.chunks.contains(chunk_coord)) {
        LOG_ERROR("Tried to access chunk which doesnt exist!");
        LOG_ERROR("Chunk:({}) does not exist!", dbg_fmt(chunk_coord));
        LOG_ERROR("Generated chunks: (n={})", chunkMap.chunks.size());
        for (const auto& [cpos, chunk] : chunkMap.chunks) {
            std::println("\t{} : {}", dbg_fmt(cpos), (void*)&chunk);
        }
        DEBUG_BREAKPOINT();
    }
    return chunkMap.chunks.at(chunk_coord).get();
}

Chunk& World::ch_getMutableChunk(ivec3 chunk_coord) {
    if (!chunkMap.chunks.contains(chunk_coord)) {
        LOG_ERROR("Tried to access chunk which doesnt exist!");
        LOG_ERROR("Chunk:({}) does not exist!", dbg_fmt(chunk_coord));
        LOG_ERROR("Generated chunks: (n={})", chunkMap.chunks.size());
        for (const auto& [cpos, chunk] : chunkMap.chunks) {
            std::println("\t{} : {}", dbg_fmt(cpos), (void*)&chunk);
        }
        DEBUG_BREAKPOINT();
    }
    return *chunkMap.chunks.at(chunk_coord);
}

const Chunk* World::getChunk(vec3 world_pos) const {
    ivec3 chunk_coord = World::worldToChunkCoord(world_pos);
    return ch_getChunk(chunk_coord);
}
Chunk& World::getMutableChunk(vec3 world_pos) {
    ivec3 cpos = World::worldToChunkCoord(world_pos);
    return ch_getMutableChunk(cpos);
}
