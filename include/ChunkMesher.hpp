#pragma once
#include "Mesh.hpp"
#include "Chunk.hpp"
#include "TextureAtlas.hpp"

struct World;
struct ChunkMesher {
    ChunkMesher() = default;
    ~ChunkMesher() = default;

    std::size_t emit_chunk_vertex_data(std::vector<Vertex>& out_vertices,
                                       std::vector<u32>& out_indices, const World* world_ptr,
                                       const Chunk* chunk, const ChunkMetadata* chunk_meta,
                                       const ivec3 chunk_offset, const TextureAtlas& atlas);
};
