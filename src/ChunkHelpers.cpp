#include "ChunkHelpers.hpp"
#include "FormatSpecs.hpp"

Direction get_cpos_overflow_direction(ChunkBlockPos p){

    constexpr auto const& ext = ChunkInfo::Extents3D;
    static constexpr auto lo = ChunkBlockPos{-1,0,-1};
    static constexpr auto hi = ChunkBlockPos{ext.x+1, ext.y, ext.z+1};
    if (!LM::isVecInBounds(p, lo, hi)){
        std::println(stderr, "oob: {}",p);
        BREAKPOINT();
    }
    // given a chunk block pos which is out of the bounds of a chunk,
    // return the direction of the chunk, from the center, that this overflow is in.
    if (p.x <= -1){ assert(p.z >=0 && p.z<ext.z); return Direction::LEFT; }
    if (p.x >= ext.x){ assert(p.z >=0 && p.z<ext.z);return Direction::RIGHT; }
    if (p.z <= -1){ assert(p.x >=0 && p.x<ext.x);return Direction::FORWARD; }
    if (p.z >= ext.x){assert(p.x >=0 && p.x<ext.x); return Direction::BACKWARD; }
    std::abort();
}
