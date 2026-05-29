#include "Chunk.hpp"
#include "ChunkHelpers.hpp"
#include "CoordTypes.hpp"

std::atomic<u32> id_counter{0};

bool Chunk::tryWrite(PendingBlockWrite write){
    auto targetChunkLocal = toChunkBlockPos(write.targetWorldBlockPos);
    Block& targetBlock = this->at(targetChunkLocal);
    if (canMakeWrite(write,targetBlock)){
        targetBlock = write.source_block;
        return true;
   }
    return false;
}
bool ChunkSpan::tryWrite(PendingBlockWrite write){
    // turn the targetChunkCoord and write.WorldBlockPos to targetChunkBlockPos
    auto targetChunkLocal = toChunkBlockPos(write.targetWorldBlockPos);
    Block& targetBlock = this->at(targetChunkLocal);
    // the higher 'priority' block wins.
    if (canMakeWrite(write,targetBlock)){
        targetBlock = write.source_block;
        return true;
    }
    return false;
}
