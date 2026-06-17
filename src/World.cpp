
#include "ChunkHelpers.hpp"
#include "DebugFormatSpecializations.hpp"
#include "World.hpp"

#include "Chunk.hpp"
#include "Breakpoints.hpp"
#include "Logger.hpp"

using namespace glm;
// returns the origin (0,0,0) of whichever chunk *worldPos* is in.

Block World::getBlock(WorldBlockPos worldPos) const {
    WorldChunkCoord chunkCoord = toWorldChunkCoord(worldPos);
    auto  chunk_ptr =
        chunkMap.chunk_entries.contains(chunkCoord) ? &chunkMap.chunk_entries.at(chunkCoord) : nullptr;
    if (chunk_ptr) {
        auto worldChunkOffset = BlockOffset{toWorldBlockPos(chunkCoord,{0,0,0}).raw()};
        auto chunkLocal = worldPos - worldChunkOffset;
        return chunk_ptr->get()->block_data.at(ChunkBlockPos{chunkLocal.raw()});
    } else {
        return Block::Empty();
    }
    // TODO: 
    // currently midway through refactoring all ambiguous uses of ivec3 
    // to the WorldBlockPos, WorldChunkCoord, and ChunkBlockPos types.
    // I might consider making them custom types that cant implicitly 
    // convert into eachother, but CAN implicitly conver to ivec3.
    // ---
    // But,  the first stage is a typedef and changing all uses to whatever they are supposed to be
    // This is also helping me identify bugs in the implementation
    // ---
    // We have a segfault right now that I believe is occuring inside setBlock or smth
    // the function which uploads generated chunks is almost certainly broken, 
    // anyways good luck
}

