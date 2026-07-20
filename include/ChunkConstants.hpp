#pragma once 
#include "Types.h"
#include "cpp23_ranges.hpp"
#include "GeometryConstants.hpp"
inline constexpr const i32 WORLD_YMAX = 386;                                        // x/y/z
inline constexpr const i32 WORLD_YMIN = 0;                                        // x/y/z

inline constexpr const size_t N_NEIGHBOURS = 4;  // up, down, left, right (column chunks)
inline constexpr const i32 CHUNK_XWIDTH = 16;                                        // x/y/z
inline constexpr const i32 CHUNK_ZWIDTH = 16;                                        // x/y/z
inline constexpr const i32 CHUNK_HEIGHT = 386;                                        // x/y/z
inline constexpr const i32 CHUNK_SIZE = CHUNK_XWIDTH * CHUNK_ZWIDTH * CHUNK_HEIGHT;  // x/y/z


inline constexpr size_t MAX_VERTICES_PER_CHUNK = CHUNK_SIZE * MAX_QUADS_PER_BLOCK;
inline constexpr size_t MAX_INDICES_PER_CHUNK = CHUNK_SIZE * MAX_QUADS_PER_BLOCK;

inline const auto CHUNK_XRANGE = views::iota(0, CHUNK_XWIDTH);  // x/y/z
inline const auto CHUNK_YRANGE = views::iota(0, CHUNK_HEIGHT);  // x/y/z
inline const auto CHUNK_ZRANGE = views::iota(0, CHUNK_ZWIDTH);  // x/y/z

using ChunkExtent = std::extents<size_t, CHUNK_XWIDTH, CHUNK_HEIGHT, CHUNK_ZWIDTH>;
