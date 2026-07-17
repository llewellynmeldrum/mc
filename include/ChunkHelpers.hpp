#pragma once
#include "cppslop.hpp"
#include "glm/vec3.hpp"
#include "ChunkConstants.hpp"
#include "cpp23_ranges.hpp"
FORWARD_DECL_STRUCT(Chunk);  // forward decl for using defs
using MeshRevisionID = u64;
using GenRevisionID = u64;






constexpr inline const auto EachBlockInChunk() {
    return ranges::views::cartesian_product(CHUNK_XRANGE, CHUNK_YRANGE, CHUNK_ZRANGE);
}

// only accept blocks which satisfy pred
template<typename Pred>
constexpr inline auto EachBlockInChunk(Pred&& pred) {
    return ranges::views::cartesian_product(CHUNK_XRANGE, CHUNK_YRANGE, CHUNK_ZRANGE) |
    views::filter(std::forward<Pred>(pred));
}

