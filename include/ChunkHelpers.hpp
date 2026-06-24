#pragma once
#include "cppslop.hpp"
#include "glm/vec3.hpp"
#include "ChunkConstants.hpp"
FORWARD_DECL_STRUCT(Chunk);  // forward decl for using defs
using MeshRevisionID = u64;
using GenRevisionID = u64;

// A ChunkView is a non owning view of a sequence of chunks, paired with their coordinates
using ChunkView = std::pair<glm::ivec3, const Chunk*>;





constexpr inline const auto EachBlockInChunk() {
    return std::views::cartesian_product(CHUNK_XRANGE, CHUNK_YRANGE, CHUNK_ZRANGE);
}

// only accept blocks which satisfy pred
template<typename Pred>
constexpr inline auto EachBlockInChunk(Pred&& pred) {
    return std::views::cartesian_product(CHUNK_XRANGE, CHUNK_YRANGE, CHUNK_ZRANGE)|
    std::views::filter(std::forward<Pred>(pred));
}

