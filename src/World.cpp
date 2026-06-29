
#include "Block.hpp"
#include "ChunkHelpers.hpp"
#include "FormatSpecs.hpp"
#include "World.hpp"

#include "Chunk.hpp"
#include "Breakpoints.hpp"
#include "Logger.hpp"

using namespace glm;
// returns the origin (0,0,0) of whichever chunk *worldPos* is in.

void World::make_chunk_entry(WorldChunkCoord key){
    bool inserted = chunkMap.entries.try_emplace(key,key);
    if (!inserted){
        std::println(stderr,"Tried to make duplicate chunk_entry for chunk @{}",key);
        std::terminate();
    }
}

