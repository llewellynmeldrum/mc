#include "cpp23_ranges.hpp"

#include "ChunkStorage.hpp"
#include "Chunk.hpp"
#include "CoordIteration.hpp"


ChunkSlice2D::ChunkSlice2D(Chunk* src, SliceType _slice_type, ChunkBlockPos pos1, ChunkBlockPos pos2) : slice_type(_slice_type) {
        if (slice_type == SliceType::X){
            buf.resize(CHUNK_HEIGHT * CHUNK_ZWIDTH);
            locked_axis_val = pos1.x;
            assert_eq(pos1.x+1,pos2.x);
        }else if (slice_type == SliceType::Y){
            buf.resize(CHUNK_XWIDTH * CHUNK_ZWIDTH);
            locked_axis_val = pos1.y;
            assert_eq(pos1.y+1,pos2.y);
        }else if (slice_type == SliceType::Z){
            buf.resize(CHUNK_XWIDTH * CHUNK_HEIGHT);
            locked_axis_val = pos1.z;
            assert_eq(pos1.z+1,pos2.z);
        }
        // copy all of them in 
        ForEachInRangeEx(pos1, pos2,[&](i32 cx, i32 cy, i32 cz){
            this->at(cx,cy,cz) = src->at(cx,cy,cz);
        });
}
ChunkStore::ChunkStore(const Chunk& chunk){
    buf.resize(CHUNK_SIZE);
    ranges::copy(chunk.data,buf.begin());
}
ChunkStore::ChunkStore(const Chunk* chunk){
    buf.resize(CHUNK_SIZE);
    ranges::copy(chunk->data,buf.begin());
}
ChunkStore::ChunkStore(){
    buf.resize(CHUNK_SIZE);
}

