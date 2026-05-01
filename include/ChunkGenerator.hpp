#include "Chunk.hpp"
struct ChunkGenerator {
    ChunkGenerator() = default;
    ~ChunkGenerator() = default;
    // has noise maps, etc

    std::unique_ptr<Chunk> gen(ivec3 pos);
};
