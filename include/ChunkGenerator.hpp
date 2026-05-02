#include "Chunk.hpp"
using ChunkDataPair = std::pair<Chunk, ChunkMetadata>;
struct ChunkGenerator {
    ChunkGenerator() = default;
    ~ChunkGenerator() = default;
    // has noise maps, etc

    ChunkDataPair gen(ivec3 pos);
};
