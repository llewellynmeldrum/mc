
#include "Block.hpp"
#include "ChunkHelpers.hpp"
#include "FormatSpecs.hpp"
#include "World.hpp"

#include "Chunk.hpp"
#include "Breakpoints.hpp"
#include "Logger.hpp"

using namespace glm;
// returns the origin (0,0,0) of whichever chunk *worldPos* is in.

ChunkEntry* World::make_chunk_entry(WorldChunkCoord key){
    auto* res = chunkMap.entries.try_emplace(return_policy::return_ptr{}, key,key);
    assert(res);
    return res;
}


