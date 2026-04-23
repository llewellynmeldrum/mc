#include "Chunk.hpp"
#include "DebugFormatSpecializations.hpp"
#include "Logger.hpp"
#include <concepts>
#include <type_traits>
#include "DEBUG.hpp"
// returns the origin (0,0,0) of whichever chunk *worldPos* is in.
ChunkCoordinate Chunk::worldToChunkPos(const vec3& worldPos){
    f32 x = std::floor(worldPos.x/CHUNK_XWIDTH);
    f32 z = std::floor(worldPos.z/CHUNK_ZWIDTH);
    ChunkCoordinate res = {
        x,z
    };
    return res;
}

// returns the origin (0,0,0) of the chunk at *chunkPos*
vec3 Chunk::chunkToWorldPos(const ChunkCoordinate& chunkPos){
    f32 x = chunkPos.x*CHUNK_XWIDTH;
    f32 z = chunkPos.y*CHUNK_ZWIDTH;
    vec3 res = {x,0.0,z};
    return res;
}
