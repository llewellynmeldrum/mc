#include "Chunk.hpp"
#include "FastNoise/Generators/Fractal.h"
#include "FastNoise/Generators/Simplex.h"
#include "FastNoise/Utility/SmartNode.h"
using ChunkDataPair = std::pair<Chunk, ChunkMetadata>;
struct Biome;
// 2d Z major array (indexed via [x + z*CHUNK_ZWIDTH])
using HeightMap = std::array<i64, CHUNK_ZWIDTH * CHUNK_XWIDTH>;

using Simplex = FastNoise::Simplex;

template <typename T>
using NoiseMap = FastNoise::SmartNode<T>;

using BiomeMap = std::vector<const Biome*>;
struct TerrainNoise {
    NoiseMap<Simplex> base;
    NoiseMap<Simplex> hills;
    NoiseMap<Simplex> detail;
    NoiseMap<Simplex> valleys;
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
