#pragma once 
#include "Types.h"
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
static inline constexpr glm::ivec3 Chunk_Extents = { CHUNK_XWIDTH, CHUNK_HEIGHT, CHUNK_ZWIDTH };
