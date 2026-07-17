
#include "Block.hpp"
#include "ChunkHelpers.hpp"
#include "FormatSpecs.hpp"
#include "World.hpp"

#include "Chunk.hpp"
#include "Breakpoints.hpp"
#include "Logger.hpp"

using namespace glm;
// returns the origin (0,0,0) of whichever chunk *worldPos* is in.

void World::set_block(WorldBlockPos wpos, BlockType block){
    auto coord = toWorldChunkCoord(wpos);
    auto* entry = chunkMap.entries.try_get(coord);
    auto* pwq = chunkMap.get_or_emplace_pwq(coord);
    auto chunk_local = toChunkBlockPos(wpos);
    if (entry){
        entry->block_data.at(chunk_local) = block;
        entry->_mark_mesh_dirty();
    }else{
        pwq->emplace_back(
            OverwritePolicy::Everything,
            coord,
            wpos,
            block
        );
    }
}
ChunkEntry* World::make_chunk_entry(WorldChunkCoord key){
    auto* res = chunkMap.entries.try_emplace(return_policy::return_ptr{}, key,key);
    assert(res);
    return res;
}


