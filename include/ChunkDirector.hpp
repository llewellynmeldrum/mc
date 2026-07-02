#pragma once 

// facilitates the searching aa
#include "CoordTypes.hpp"
#include "LM.hpp"
struct ChunkDirector{
    WorldChunkCoord prev_chunk_pos{};
    WorldChunkCoord cur_chunk_pos{};
    void init(WorldFloatPos player_pos){
        start_frame(player_pos);
        end_frame();
    }
    bool did_player_cross_chunk_boundary(WorldFloatPos new_pos)const {
        return prev_chunk_pos != toWorldChunkCoord(new_pos);
    }
    bool is_chunk_inside_cull_distance(WorldChunkCoord coord, i32 cull_dist){
        auto lo = cur_chunk_pos + ChunkOffset{-cull_dist};
        auto hi = cur_chunk_pos + ChunkOffset{cull_dist};
        return LM::isVecInBounds(coord,lo, hi);
    }
    bool is_chunk_outside_cull_distance(WorldChunkCoord coord, i32 cull_dist){
        return !is_chunk_inside_cull_distance(coord,cull_dist);
    }
    void start_frame(WorldFloatPos player_pos){
        cur_chunk_pos = toWorldChunkCoord(player_pos);
    }
    void end_frame(){
        prev_chunk_pos = cur_chunk_pos;
    }
};
