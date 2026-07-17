#pragma once
#include "ChunkConstants.hpp"
#include "glm/ext/vector_int3.hpp"

struct ChunkInfo{
    static constexpr glm::ivec3 Extents = { CHUNK_XWIDTH, CHUNK_HEIGHT, CHUNK_ZWIDTH };
};

