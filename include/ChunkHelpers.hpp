#pragma once
#include "cppslop.hpp"
#include "glm/vec3.hpp"
#include "ChunkConstants.hpp"
FORWARD_DECL_STRUCT(Chunk);  // forward decl for using defs
using MeshRevisionID = u64;

// A ChunkView is a non owning view of a sequence of chunks, paired with their coordinates
using ChunkView = std::pair<glm::ivec3, const Chunk*>;



STD_HASH_SPECIALIZATION(glm::ivec3, obj,
    return (std::hash<i32>{}(obj.x) << 1) ^  // NOLINT
           (std::hash<i32>{}(obj.y) << 2) ^  // NOLINT
           (std::hash<i32>{}(obj.z));
)



constexpr inline const auto EachBlockInChunk() {
    return std::views::cartesian_product(CHUNK_XRANGE, CHUNK_YRANGE, CHUNK_ZRANGE);
}

