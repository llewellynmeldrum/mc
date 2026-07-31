#pragma once
#include "CoordTypes.hpp"
#include "Direction.hpp"
#include "cppslop.hpp"
#include "glm/vec3.hpp"
#include "ChunkConstants.hpp"
#include "cpp23_ranges.hpp"
FORWARD_DECL_STRUCT(Chunk);  // forward decl for using defs
using MeshRevisionID = u64;
using GenRevisionID = u64;





inline constexpr bool is_in_chunk(ChunkBlockPos p){
    return LM::isVecInBounds(p, ChunkBlockPos{0,0,0}, ChunkBlockPos{ChunkInfo::Extents3D});
}

constexpr inline const auto EachBlockInChunk() {
    return ranges::views::cartesian_product(CHUNK_XRANGE, CHUNK_YRANGE, CHUNK_ZRANGE);
}

// only accept blocks which satisfy pred
template<typename Pred>
constexpr inline auto EachBlockInChunk(Pred&& pred) {
    return ranges::views::cartesian_product(CHUNK_XRANGE, CHUNK_YRANGE, CHUNK_ZRANGE) |
    views::filter(std::forward<Pred>(pred));
}

// given some chunk block position, which direction does it overflow in?
Direction get_cpos_overflow_direction(ChunkBlockPos p);
inline constexpr auto neighbour_block_coords (ChunkBlockPos p){ 
    return Direction_offset | views::transform([p](auto const& o){ return ChunkBlockPos{p.raw()+o};});
};
