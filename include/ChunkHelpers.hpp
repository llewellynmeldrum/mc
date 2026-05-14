#pragma once
#include "Types.h"
//#include <utility>
#include "cppslop.hpp"
#include "glmWrapper.hpp"
FORWARD_DECL_STRUCT(Chunk);  // forward decl for using defs

// A ChunkView is a non owning view of a sequence of chunks, paired with their coordinates
using ChunkView = std::pair<ivec3, const Chunk*>;

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

inline constexpr const size_t NUM_NEIGHBOURS = 6;  // up, down, left, right, front, back (3d chunks)
inline constexpr const i64 CHUNK_XWIDTH = 16;                                        // x/y/z
inline constexpr const i64 CHUNK_ZWIDTH = 16;                                        // x/y/z
inline constexpr const i64 CHUNK_HEIGHT = 16;                                        // x/y/z
inline constexpr const i64 CHUNK_SIZE = CHUNK_XWIDTH * CHUNK_ZWIDTH * CHUNK_HEIGHT;  // x/y/z
//
inline constexpr std::size_t VERTICES_PER_FACE = 4;
inline constexpr std::size_t FACES_PER_BLOCK = 6 ;
inline constexpr std::size_t INDICES_PER_BLOCK = 6 ;
inline constexpr std::size_t MAX_VERTICES_PER_BLOCK = VERTICES_PER_FACE * FACES_PER_BLOCK;
inline constexpr std::size_t MAX_VERTICES_PER_CHUNK = CHUNK_SIZE * MAX_VERTICES_PER_BLOCK;

inline constexpr std::size_t INDICES_PER_FACE = 6;
inline constexpr std::size_t MAX_INDICES_PER_BLOCK = INDICES_PER_FACE * FACES_PER_BLOCK;
inline constexpr std::size_t MAX_INDICES_PER_CHUNK = CHUNK_SIZE * MAX_INDICES_PER_BLOCK;

inline constexpr const auto CHUNK_XRANGE = std::views::iota(0, CHUNK_XWIDTH);  // x/y/z
inline constexpr const auto CHUNK_YRANGE = std::views::iota(0, CHUNK_HEIGHT);  // x/y/z
inline constexpr const auto CHUNK_ZRANGE = std::views::iota(0, CHUNK_ZWIDTH);  // x/y/z

constexpr inline const auto EachBlockInChunk() {
    return std::views::cartesian_product(CHUNK_XRANGE, CHUNK_YRANGE, CHUNK_ZRANGE);
}
constexpr inline const auto EachCoordInRange(
    i64 x0, i64 x1,
    i64 y0, i64 y1,
    i64 z0, i64 z1) {
    using std::views::iota;
    using std::views::cartesian_product;

    return cartesian_product(iota(x0,x1), iota(y0,y1), iota(z0,z1));
}

#define MD_ACCESS_MACRO(T, storage)                                                                         \
    inline T& operator[](i16 x, i16 y, i16 z) {                                                    \
        return std::mdspan(storage.data(), CHUNK_XWIDTH, CHUNK_HEIGHT, CHUNK_ZWIDTH)[x, y, z];        \
    }                                                                                              \
    inline const T& operator[](i16 x, i16 y, i16 z) const {                                        \
        return std::mdspan(storage.data(), CHUNK_XWIDTH, CHUNK_HEIGHT, CHUNK_ZWIDTH)[x, y, z];        \
    }

constexpr inline ivec3 worldToChunkCoord(vec3 worldPos) {
    f32   x = std::floor(worldPos.x / CHUNK_XWIDTH);
    f32   y = std::floor(worldPos.y / CHUNK_HEIGHT);
    f32   z = std::floor(worldPos.z / CHUNK_ZWIDTH);
    ivec3 res = { x, y, z };
    return res;
}
