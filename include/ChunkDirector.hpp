#pragma once 

// facilitates the searching aa
#include "CoordTypes.hpp"
struct ChunkDirector{
    WorldChunkCoord prev_chunk_pos{};
    bool did_player_cross_chunk_boundary(WorldFloatPos new_pos)const {
        return prev_chunk_pos != toWorldChunkCoord(new_pos);
    }
    void end_frame(WorldFloatPos player_pos){
        prev_chunk_pos = toWorldChunkCoord(player_pos);
    }
};
