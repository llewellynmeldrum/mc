#include "ChunkConstants.hpp"
#include "lmath.hpp"
static inline constexpr glm::ivec3 Chunk_Extents = { CHUNK_XWIDTH, CHUNK_HEIGHT, CHUNK_ZWIDTH };
// Identifies a CHUNK in the WORLD.
using WorldChunkCoord = glm::ivec3;      
// Identifies a BLOCK in the WORLD.
using WorldBlockPos = glm::ivec3;        
 // Identifies a BLOCK in a CHUNK.
using ChunkBlockPos = glm::ivec3;       
// worldChunkCoord -> worldBlockPos 
// Returns the world pos of the block at [0,0,0] in the chunk identified by wChunkCoord
inline auto toWorldBlockPos(WorldChunkCoord chunk) -> WorldBlockPos{
    return chunk * Chunk_Extents;
}

// chunkBlockPos->worldBlockPos
// Returns the world pos of the block at [cBlockPos] in the chunk identified by wChunkCoord
inline auto toWorldBlockPos(WorldChunkCoord chunk,ChunkBlockPos local) -> WorldBlockPos{
    return local + toWorldBlockPos(chunk);
}

// worldBlockPos->      chunkBlockPos
// returns the chunk local pos, in the chunk within wBlockPos, of the block at [wBlockPos] in the world
inline auto toChunkBlockPos(WorldBlockPos block) -> ChunkBlockPos{
    return static_cast<ChunkBlockPos>(lmath::euclid_mod(block, Chunk_Extents));
}

// worldBlockPos->      worldChunkCoord
inline auto toWorldChunkCoord(WorldBlockPos block) -> WorldChunkCoord {
    return static_cast<WorldChunkCoord>(lmath::floor_div(block , Chunk_Extents));
}
