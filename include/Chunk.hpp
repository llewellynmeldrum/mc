#pragma once
#include "ChunkConstants.hpp"
#include "glm/ext/vector_int3.hpp"

struct ChunkInfo{
    static constexpr i32 XWIDTH = 16;                                        // x/y/z
    static constexpr i32 ZWIDTH = 16;                                        // x/y/z
    static constexpr i32 HEIGHT = 386;                                        // x/y/z
    static constexpr i32 SIZE = XWIDTH * ZWIDTH * HEIGHT;  // x/y/z
    static constexpr glm::ivec3 Extents3D = { XWIDTH, HEIGHT, ZWIDTH };
    static constexpr glm::ivec2 Extents2D = { XWIDTH, ZWIDTH };
};

