#include "Chunk.hpp"
#include "FastNoiseLite.h"
using ChunkDataPair = std::pair<Chunk, ChunkMetadata>;
struct Biome;
// 2d Z major array (indexed via [x + z*CHUNK_ZWIDTH])
using HeightMap = std::array<i64, CHUNK_ZWIDTH * CHUNK_XWIDTH>;

using NoiseMap = FastNoiseLite;

using BiomeMap = std::vector<const Biome*>;
struct TerrainNoise {
    NoiseMap base;
    NoiseMap hills;
    NoiseMap detail;
    NoiseMap valleys;
    TerrainNoise();
};
struct ChunkGenerator {
    ChunkGenerator() = default;
    ~ChunkGenerator() = default;
    // has noise maps, etc

    BiomeMap  genChunkBiomeMap(ivec3 chunk_coord, ivec3 chunk_offset);
    HeightMap genChunkHeightmap(const BiomeMap& biomes, ivec3 chunk_coord, ivec3 chunk_offset);

    TerrainNoise terrain_noise;

    ChunkDataPair gen(ivec3 pos);
};
