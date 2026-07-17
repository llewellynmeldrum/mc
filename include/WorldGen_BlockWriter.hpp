#include "ChunkStorage.hpp"
#include "CoordTypes.hpp"

#include <stdexcept>
FORWARD_DECL_STRUCT(ChunkEntry)
struct BlockWriter{
    ChunkView             src_block_store;
    PendingWriteList&       pending_writes;
    const WorldChunkCoord   src_chunk;
    const WorldBlockPos     world_block_lo;
    const WorldBlockPos     world_block_hi;
    BlockWriter( ChunkView _src_block_store, PendingWriteList& _pending_writes, WorldChunkCoord _source_chunk);

    decltype(auto) at(this auto& self, ChunkBlockPos cpos){
        return self.src_block_store.at(cpos);
    }
    void try_place(OverwritePolicy policy, WorldBlockPos wpos, BlockType bt);
};
