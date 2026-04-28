#pragma once
#include "Mesh.hpp"
#include "Chunk.hpp"
#include "TextureAtlas.hpp"

struct World;
struct ChunkMesher {
    ChunkMesher() = default;
    ~ChunkMesher() = default;

    Mesh mesh(const World* world_ptr, const Chunk& chunk, const ivec3 chunk_offset,
              const TextureAtlas& atlas);
};
