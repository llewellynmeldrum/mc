
#include "World.hpp"

#include "Chunk.hpp"
#include "DEBUG.hpp"
#include "DebugFormatSpecializations.hpp"
#include "Logger.hpp"
// returns the origin (0,0,0) of whichever chunk *worldPos* is in.
ivec3
World::worldToChunkCoord(vec3 worldPos) {
    f32   x = std::floor(worldPos.x / CHUNK_XWIDTH);
    f32   y = std::floor(worldPos.y / CHUNK_HEIGHT);
    f32   z = std::floor(worldPos.z / CHUNK_ZWIDTH);
    ivec3 res = { x, y, z };
    return res;
}

// returns the origin (0,0,0) of the chunk at *chunkPos*
vec3
World::chunkToWorldPos(ivec3 chunkPos) {
    f32  x = chunkPos.x * CHUNK_XWIDTH;
    f32  y = chunkPos.y * CHUNK_HEIGHT;
    f32  z = chunkPos.z * CHUNK_ZWIDTH;
    vec3 res = { x, y, z };
    return res;
}

Block
World::getBlock(vec3 world_pos) const {
    ivec3 chunk_origin = World::worldToChunkCoord(world_pos);
    auto* chunk = chunks.data.contains(chunk_origin) ? &chunks.data.at(chunk_origin) : nullptr;
    if (chunk) {
        // draw this out, i think its like trying to find the local position in the chunk and then
        // idk, need a book for thijs one
        vec3 chunk_local = world_pos - (static_cast<vec3>(chunk_origin) * (float)CHUNK_ZWIDTH);
        return chunk->getBlock(chunk_local);
    } else {
        return Block::Empty();
    }
}

const Chunk*
World::getChunk(vec3 world_pos) const {
    vec3 chunk_origin = World::worldToChunkCoord(world_pos);
    if (chunks.data.contains(chunk_origin)) {
        return &chunks.data.at(chunk_origin);
    } else {
        return nullptr;
    }
}
const Chunk*
World::getChunk(ivec3 cpos) const {
    if (!chunks.data.contains(cpos)) {
        LOG_ERROR("Tried to access chunk which doesnt exist!");
        LOG_ERROR("Chunk:({}) does not exist!", dbg_fmt(cpos));
        LOG_ERROR("Generated chunks: (n={})", chunks.data.size());
        for (auto [cpos, chunk] : chunks.data) {
            std::println("\t{} : {}", dbg_fmt(cpos), (void*)&chunk);
        }
        DEBUG_BREAKPOINT();
    }
    return &chunks.data.at(cpos);
}

Chunk&
World::getMutableChunk(ivec3 cpos) {
    if (!chunks.data.contains(cpos)) {
        LOG_ERROR("Tried to access chunk which doesnt exist!");
        LOG_ERROR("Chunk:({}) does not exist!", dbg_fmt(cpos));
        LOG_ERROR("Generated chunks: (n={})", chunks.data.size());
        for (auto [cpos, chunk] : chunks.data) {
            std::println("\t{} : {}", dbg_fmt(cpos), (void*)&chunk);
        }
        DEBUG_BREAKPOINT();
    }
    return chunks.data.at(cpos);
}
Chunk&
World::getMutableChunk(vec3 world_pos) {
    ivec3 cpos = World::worldToChunkCoord(world_pos);
    if (!chunks.data.contains(cpos)) {
        LOG_ERROR("Tried to access chunk which doesnt exist!");
        LOG_ERROR("Chunk at {} chunk:({}) does not exist!", dbg_fmt(world_pos), dbg_fmt(cpos));
        LOG_ERROR("Generated chunks: (n={})", chunks.data.size());
        for (auto [cpos, chunk] : chunks.data) {
            std::println("\t{} : {}", dbg_fmt(cpos), (void*)&chunk);
        }
        DEBUG_BREAKPOINT();
    }
    return chunks.data.at(cpos);
}
