#include "Chunk.hpp"
#include "Concurrency.hpp"
using ChunkDataPair = std::pair<Chunk, ChunkMetadata>;
struct Biome;
// 2d Z major array (indexed via [x + z*CHUNK_ZWIDTH])
using HeightMap = std::array<i64, CHUNK_ZWIDTH * CHUNK_XWIDTH>;


using BiomeMap = std::vector<const Biome*>;
struct TerrainNoise {
};
struct ChunkGenerator {
    ChunkGenerator() = default;
    ~ChunkGenerator() = default;
    // has noise maps, etc

    BiomeMap  genChunkBiomeMap(ivec3 chunk_coord, ivec3 chunk_offset);
    HeightMap genChunkHeightmap(const BiomeMap& biomes, ivec3 chunk_coord, ivec3 chunk_offset);
    TerrainNoise terrain_noise;
    ChunkDataPair gen(ivec3 pos);

    inline void setupChunkGenerator(){
        genThreads.launch(
            genChunks,
            std::ref(genJobQueue),
            std::ref(genResultQueue)
        );
    }

    Queue<GenJob> genJobQueue;
    Queue<GenResult> genResultQueue;
    ThreadPool genThreads{4};
    static void genChunks(std::stop_token stopToken, Queue<GenJob>& input_queue, Queue<GenResult>& output_queue);
};
