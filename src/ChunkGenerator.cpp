#include "ChunkGenerator.hpp"

Chunk ChunkGenerator::gen(ivec3 pos) {
    // TODO: implement generation
    auto c = Chunk();
    c.setBlocks(BlockType::DIRT_BLOCK, { 0, 0, 0 }, { CHUNK_XWIDTH, 5, CHUNK_ZWIDTH });
    c.setBlocks(BlockType::GRASS_BLOCK, { 0, 6, 0 }, { CHUNK_XWIDTH, 6, CHUNK_ZWIDTH });
    return c;
}
