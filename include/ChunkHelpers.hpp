#pragma once
#include "cppslop.hpp"
#include "glm/vec3.hpp"
#include "ChunkConstants.hpp"
FORWARD_DECL_STRUCT(Chunk);  // forward decl for using defs

// A ChunkView is a non owning view of a sequence of chunks, paired with their coordinates
using ChunkView = std::pair<glm::ivec3, const Chunk*>;

namespace std{

    template<>
    struct hash<glm::ivec3>{
        std::size_t operator()(const glm::ivec3& v) const {
            return (std::hash<i32>{}(v.x) << 1) ^  // NOLINT
                   (std::hash<i32>{}(v.y) << 2) ^  // NOLINT
                   (std::hash<i32>{}(v.z));
        }
    };
}// namespace: std


constexpr inline const auto EachBlockInChunk() {
    return std::views::cartesian_product(CHUNK_XRANGE, CHUNK_YRANGE, CHUNK_ZRANGE);
}

constexpr inline const auto EachInRange(
    glm::ivec2 min, glm::ivec2 max){
    return EachInRange(min.x,max.x, min.y, max.y);
}
constexpr inline const auto EachInRange(
    glm::ivec3 min, glm::ivec3 max){
    return EachInRange(min.x,max.x, min.y, max.y, min.z,max.z);
}


[[deprecated]]constexpr inline glm::ivec3 worldToChunkCoord(glm::ivec3 worldPos) {
    f32   x = std::floor(worldPos.x / CHUNK_XWIDTH);
    f32   y = std::floor(worldPos.y / CHUNK_HEIGHT);
    f32   z = std::floor(worldPos.z / CHUNK_ZWIDTH);
    glm::ivec3 res = { x, y, z };
    return res;
}


