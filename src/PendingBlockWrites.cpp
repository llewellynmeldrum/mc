#include "PendingBlockWrites.hpp"
#include "ChunkView.hpp"
#include "ChunkHelpers.hpp"
#include "CoordTypes.hpp"
#include "DebugChunkLog.hpp"
// disable colors
#include "FormatSpecs.hpp"

std::atomic<u32> id_counter{0};

static void log_deferred_write(PendingBlockWrite write, Block old_block, bool success){
    auto chunk_coord = toWorldChunkCoord(write.target_world);
//    auto targetChunkLocal = toChunkBlockPos(write.targetWorldBlockPos);
    //  Investigate why all deferred writes are failing.
    //  The logging is telling me that the destination blocks match the source block every time. very odd
    log_to_chunk("pending_write",chunk_coord,"Attempted ({}:{}) write @W{} | {}->{}",
                 write.policy,
                 success ? "Y" : "N",
                 write.target_world,
                 old_block,
                 write.new_block
    );
}

bool tryWrite(PendingBlockWrite write, ChunkView view){
    auto targetChunkLocal = toChunkBlockPos(write.target_world);
    Block old_block = view.at(targetChunkLocal);
    bool success{};
    if (canMakeWrite(write,old_block)){
        view.at(targetChunkLocal) = write.new_block;
        success = true;
    }else{
        success = false;
    }
    log_deferred_write(write, old_block, success);
    return success;
}
