#include "Chunk.hpp"
#include "ChunkHelpers.hpp"
#include "CoordTypes.hpp"
#include "DebugChunkLog.hpp"
// disable colors
#include "FormatSpecs.hpp"

std::atomic<u32> id_counter{0};
static constexpr BlockType overwrite_block = BlockType::STONE_BLOCK;

static void log_deferred_write(PendingBlockWrite write, Block old_block, bool success){
    auto chunk_coord = toWorldChunkCoord(write.targetWorldBlockPos);
//    auto targetChunkLocal = toChunkBlockPos(write.targetWorldBlockPos);
    //  Investigate why all deferred writes are failing.
    //  The logging is telling me that the destination blocks match the source block every time. very odd
    log_to_chunk("pending_write",chunk_coord,"Attempted ({}:{}) write @W{} | {}->{}",
                 write.overwritePolicy,
                 success ? "Y" : "N",
                 write.targetWorldBlockPos,
                 old_block,
                 write.new_block
    );
}
bool Chunk::tryWrite(PendingBlockWrite write){
    auto targetChunkLocal = toChunkBlockPos(write.targetWorldBlockPos);
    Block old_block = this->at(targetChunkLocal);
    bool success{};
    if (canMakeWrite(write,old_block)){
        this->at(targetChunkLocal) = overwrite_block;
        success = true;
    }else{
        success = false;
    }
    log_deferred_write(write, old_block, success);
    //TODO:  add all other invocations of deferred write system to per chunk log
    return success;
}
bool ChunkSpan::tryWrite(PendingBlockWrite write){
    // turn the targetChunkCoord and write.WorldBlockPos to targetChunkBlockPos
    auto targetChunkLocal = toChunkBlockPos(write.targetWorldBlockPos);
    Block old_block = this->at(targetChunkLocal);
    bool success{};
    // the higher 'priority' block wins.
    if (canMakeWrite(write,old_block)){
        // HACK: in order to see pending writes easier:
        //
        this->at(targetChunkLocal) = overwrite_block;
        //targetBlock = write.source_block;
        success = true;
    }else{
        success = false;
    }
    log_deferred_write(write, old_block, success);
    return success;
}
