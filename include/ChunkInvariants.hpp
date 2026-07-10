#pragma once 
#include "Types.h"
inline constexpr const i32 WORLD_YMAX = 256;                                        // x/y/z
inline constexpr const i32 WORLD_YMIN = 0;                                        // x/y/z

inline constexpr const size_t N_NEIGHBOURS = 4;  // up, down, left, right (column chunks)
inline constexpr const i32 CHUNK_XWIDTH = 16;                                        // x/y/z
inline constexpr const i32 CHUNK_ZWIDTH = 16;                                        // x/y/z
inline constexpr const i32 CHUNK_HEIGHT = 256;                                        // x/y/z
inline constexpr const i32 CHUNK_SIZE = CHUNK_XWIDTH * CHUNK_ZWIDTH * CHUNK_HEIGHT;  // x/y/z

// Shared quad stuff (i.e per face)
inline constexpr std::size_t VTX_PER_QUAD = 4; // aka per face
inline constexpr std::size_t INDICES_PER_QUAD = 6; // aka per face

// BlockShape::CUBE
inline constexpr std::size_t QUADS_PER_CUBE = 6;
inline constexpr std::size_t MAX_VERTICES_PER_CUBE = VTX_PER_QUAD * QUADS_PER_CUBE;
inline constexpr std::size_t MAX_INDICES_PER_CUBE = INDICES_PER_QUAD * QUADS_PER_CUBE;


// BlockShape::CROSS
inline constexpr std::size_t QUADS_PER_CROSS= 2 ;
inline constexpr std::size_t MAX_VERTICES_PER_CROSS = VTX_PER_QUAD * QUADS_PER_CROSS;
inline constexpr std::size_t MAX_INDICES_PER_CROSS = INDICES_PER_QUAD * QUADS_PER_CROSS;


inline constexpr std::size_t MAX_QUADS_PER_BLOCK = std::max(MAX_VERTICES_PER_CUBE, MAX_VERTICES_PER_CROSS);

inline constexpr std::size_t MAX_VERTICES_PER_CHUNK = CHUNK_SIZE * MAX_QUADS_PER_BLOCK;
inline constexpr std::size_t MAX_INDICES_PER_CHUNK = CHUNK_SIZE * MAX_QUADS_PER_BLOCK;






inline constexpr const auto CHUNK_XRANGE = std::views::iota(0, CHUNK_XWIDTH);  // x/y/z
inline constexpr const auto CHUNK_YRANGE = std::views::iota(0, CHUNK_HEIGHT);  // x/y/z
inline constexpr const auto CHUNK_ZRANGE = std::views::iota(0, CHUNK_ZWIDTH);  // x/y/z
