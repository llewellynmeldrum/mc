#pragma once 
#include "Mesh.hpp"
#include "Chunk.hpp"
#include "TextureAtlas.hpp"

struct ChunkMesher{
    ChunkMesher()=default;
    ~ChunkMesher()=default;

    Mesh mesh(const Chunk& chunk,const TextureAtlas& atlas);
};


