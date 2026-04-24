#include "Chunk.hpp"
struct ChunkGenerator {
    ChunkGenerator() = default;
    ~ChunkGenerator() = default;
    // has noise maps, etc

    Chunk gen(ivec3 pos);
};
