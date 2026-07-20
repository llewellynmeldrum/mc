#include "WorldGen_BlockWriter.hpp"
#include "ChunkEntry.hpp"
BlockWriter::BlockWriter(
        ChunkView _src_block_store, 
        PendingWriteList& _pending_writes,
        WorldChunkCoord _source_chunk
    ):
        src_block_store(_src_block_store),
        pending_writes(_pending_writes),
        src_chunk(_source_chunk),
        world_block_lo(toWorldOrigin(_source_chunk)),
        world_block_hi(toWorldOrigin(_source_chunk) + BlockOffset{ChunkInfo::Extents3D})
    {}
void BlockWriter::try_place(OverwritePolicy policy, WorldBlockPos wpos, BlockType bt){
    bool writeIsWithinChunkBounds = LM::isVecInBounds(wpos, world_block_lo, world_block_hi);
    auto write = PendingBlockWrite{policy, src_chunk, wpos, bt};
    if (writeIsWithinChunkBounds){
        // do it immediately
        auto cpos = toChunkBlockPos(wpos);
        tryWrite(write,src_block_store);
    }else{
        pending_writes.emplace_back(write);
    }
}
