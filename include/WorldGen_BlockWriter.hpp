#include "ChunkStorage.hpp"
#include "CoordTypes.hpp"
#include <stdexcept>
struct BlockWriter{
    ChunkStore&             src_block_store;
    PendingWriteList&       pending_writes;
    const WorldChunkCoord   src_chunk;
    const WorldBlockPos     world_block_lo;
    const WorldBlockPos     world_block_hi;
    BlockWriter(
        ChunkStore& _src_block_store, 
        PendingWriteList& _pending_writes,
        WorldChunkCoord _source_chunk
    ):
        src_block_store(_src_block_store),
        pending_writes(_pending_writes),
        src_chunk(_source_chunk),
        world_block_lo(toWorldOrigin(_source_chunk)),
        world_block_hi(toWorldOrigin(_source_chunk) + BlockOffset{Chunk_Extents})
    {}
    decltype(auto) at(this auto& self, ChunkBlockPos cpos){
        if (!self.src_block_store.contains(cpos)){
            throw std::out_of_range("Outside chunk radius");
        }else{
            return self.src_block_store.at(cpos);
        }
    }
    void try_place(OverwritePolicy policy, WorldBlockPos wpos, BlockType bt);
    std::pair<i32,i32> c2w(ChunkBlockPos pos){
        auto chunk_origin_xz = src_chunk.raw() * Chunk_Extents2;
        return {
            chunk_origin_xz[0] + pos.x,
            chunk_origin_xz[1] + pos.z,
        };
    }
};
