#include "WorldGen_BlockWriter.hpp"
void BlockWriter::try_place(OverwritePolicy policy, WorldBlockPos wpos, BlockType bt){
    bool writeIsWithinChunkBounds = LM::isVecInBounds(wpos, world_block_lo, world_block_hi);
    if (writeIsWithinChunkBounds){
        // do it immediately
        i32 cx = wpos.x - world_block_lo.x;
        i32 cy = wpos.y - world_block_lo.y;
        i32 cz = wpos.z - world_block_lo.z;
        if (canMakeWrite(policy, src_block_store.at(cx,cy,cz))){
            src_block_store.set(cx,cy,cz, bt);
        }
    }else{
        pending_writes.emplace_back(policy, src_chunk, wpos, bt);
    }
}
