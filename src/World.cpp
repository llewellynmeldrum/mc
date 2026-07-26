
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
    auto* res = chunkMap.entries.try_emplace(return_policy::return_ptr{}, key,key, worldgen_epoch);
    assert(res);
    return res;
}

std::vector<std::pair<bool, WorldChunkCoord>> World::chunksStatesInRadius(WorldChunkCoord chunkCoord, i32 dist) {
    const size_t nChunksInRadius = std::pow(2*dist+1,2);
    std::vector<std::pair<bool,WorldChunkCoord>> candidates;
    candidates.reserve(nChunksInRadius);

    auto add = [this, &candidates](i32 x, i32 z){
        const auto key = WorldChunkCoord{x,z}; // dont you have to 
        const auto state = chunkMap.entries.try_get(key);
        candidates.emplace_back(static_cast<bool>(state),key);
    };


    i32 x{chunkCoord.x}, z{chunkCoord.z};
    add(x,z); // center point
    for (i32 r = 1; r<= dist; r++){
        const i32 r2 = 2*r;
        add(--x,z); // move out of the centre point
        for (int j = 0; j<r2 - 1;j++)    add(x,++z); // traverse the remaining (-X) edge
        for (int j = 0; j<r2 ; j++)     add(++x,z);  // traverse the whole     (Z+) edge
        for (int j = 0; j<r2 ; j++)     add(x,--z);  // traverse the whole     (+X) edge
        for (int j = 0; j<r2 ; j++)     add(--x,z);  // traverse the whole     (+X) edge
    }
    return candidates;
}


