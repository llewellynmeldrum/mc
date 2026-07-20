#pragma once 
// Shared quad stuff (i.e per face)
inline constexpr size_t VTX_PER_QUAD = 4; // aka per face
inline constexpr size_t INDICES_PER_QUAD = 6; // aka per face

// BlockShape::CUBE
inline constexpr size_t QUADS_PER_CUBE = 6;
inline constexpr size_t MAX_VERTICES_PER_CUBE = VTX_PER_QUAD * QUADS_PER_CUBE;
inline constexpr size_t MAX_INDICES_PER_CUBE = INDICES_PER_QUAD * QUADS_PER_CUBE;


// BlockShape::CROSS
inline constexpr size_t QUADS_PER_CROSS= 4 ;
inline constexpr size_t MAX_VERTICES_PER_CROSS = VTX_PER_QUAD * QUADS_PER_CROSS;
inline constexpr size_t MAX_INDICES_PER_CROSS = INDICES_PER_QUAD * QUADS_PER_CROSS;


// a block can be either a cube or a cross (at this stage)
inline constexpr size_t MAX_QUADS_PER_BLOCK = std::max(MAX_VERTICES_PER_CUBE, MAX_VERTICES_PER_CROSS);
