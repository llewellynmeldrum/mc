
#include "DebugFormat.hpp"
#include "DebugFormatSpecializations.hpp"

#include "World.hpp"
#include "Logger.hpp"
#include "Types.h"

#include <FastNoise/FastNoise.h>
#include "lmath.hpp"

//         /\
//          |?         swamp       jungle
// humidity |spruce    plains      acacia
//          |snow      mesa?       desert
//          |--------------------------->
//          temperature

// clang-format off
constexpr i32 SEA_LEVEL = 64;
constexpr i32 HEIGHT_LIMIT = 256;
constexpr i32 WORLD_SEED = 1337;


struct Biome {
    std::string_view name;

    // @brief
    // Describes the block palette used to paint in block types during stage III of world gen.
    struct BlockPalette {
        BlockType topsoil;     // i.e grass for plains, NULL for desert (none)
        BlockType soil;        // i.e dirt for plains, sand for desert.
        BlockType crust;       // i.e stone for plains, sandstone for desert.
        BlockType deep_crust;  // i.e stone for plains, stone for desert.
    } palette;

    // @brief
    // Contains information used to generate the heightmap
    struct HeightInfo {
        const i32 base = SEA_LEVEL;
        const i32 max_valley_depth = 32;
        const i32 max_mountain_height = 128;
    } heightinfo;

    f32 temp_min = F32_MIN;
    f32 temp_max = F32_MAX;
    f32 humid_min = F32_MIN;
    f32 humid_max = F32_MAX;
};

// NOTE: for now, plains is the only biome.
static const Biome plains{
    .name = "Plains",
    .palette = {
        .topsoil = BlockType::GRASS_BLOCK,
        .soil = BlockType::DIRT_BLOCK,
        .crust = BlockType::STONE_BLOCK,
        .deep_crust = BlockType::STONE_BLOCK,
    },
};


using std::array;
// 2d Z major array (indexed via [x + z*CHUNK_ZWIDTH])
using HeightMap = array<i64, CHUNK_ZWIDTH * CHUNK_XWIDTH>;
using BiomeMap = std::vector<const Biome*>;

BiomeMap PLAINS_ONLY(CHUNK_ZWIDTH*CHUNK_XWIDTH, &plains);
static HeightMap genChunkHeightmap(const BiomeMap& biomes, ivec3 chunk_coord, ivec3 chunk_offset);
Chunk ChunkGenerator::gen(ivec3 chunk_coord) {
    Chunk       res;

    const ivec3 chunk_offset = World::chunkToWorldPos(chunk_coord);
    // |||||||||||||||||
    // step 0: biome selection 
    // |||||||||||||||||
    const auto& biomes = PLAINS_ONLY;  // replace with genBiomeMap for the chunk later


    // |||||||||||||||||
    // step 1: heightmap
    // |||||||||||||||||
    // -> based on biome for each block, a heightmap is created, 
    //    representing the highest point for each xz in the chunk
    const i32 chunk_min_world_y = chunk_coord.y * CHUNK_HEIGHT;
    const i32 chunk_max_world_y = chunk_min_world_y + CHUNK_HEIGHT;
    const auto  heightmap = genChunkHeightmap(biomes, chunk_coord, chunk_offset);

    for (i32 cx = 0; cx < CHUNK_XWIDTH; cx++) {
        for (i32 cz = 0; cz < CHUNK_ZWIDTH; cz++) {
            const auto world_height = heightmap[cx + CHUNK_XWIDTH * cz];
            const auto* biome = biomes[cx+CHUNK_XWIDTH *cz];
            if (chunk_min_world_y > world_height)
                continue;
            const i32 local_height = std::min(CHUNK_HEIGHT, world_height - chunk_offset.y);
//            std::print("{}, ",local_height);
            res.setColumn(biome->palette.crust, { cx, 0, cz }, local_height);
        }
//        std::println();
    }
    return res;
}

static HeightMap genChunkHeightmap(const BiomeMap& biomes, ivec3 chunk_coord,
                                                            ivec3 chunk_offset) {

    using noiseMap2D = std::array<f32, CHUNK_XWIDTH * CHUNK_ZWIDTH>;
    noiseMap2D noiseMapA;
    auto perlinNoise = FastNoise::New<FastNoise::Perlin>();

    auto minMax = perlinNoise->GenUniformGrid2D(
        noiseMapA.data(),                // yuh
        chunk_offset.x, chunk_offset.z,  // Offset into the noise map
        CHUNK_XWIDTH, CHUNK_ZWIDTH,      // Samples per axis
        1.0, 1.0,                        // step size -> this acts weird for me
        WORLD_SEED);

    LOG_EXPR(minMax.max);
    LOG_EXPR(minMax.min);
    HeightMap res;

    for (i32 cx = 0; cx < CHUNK_XWIDTH; cx++) {
        for (i32 cz = 0; cz < CHUNK_ZWIDTH; cz++) {
            const auto* biome = biomes[cx+CHUNK_XWIDTH *cz];
            const f32 min_y = biome->heightinfo.base - biome->heightinfo.max_valley_depth;
            const f32 max_y = biome->heightinfo.base + biome->heightinfo.max_mountain_height;
            f32 raw_noiseA = noiseMapA[(cz * CHUNK_XWIDTH + cx)];

            // adjust noise to have more aggresive peaks
//            raw_noiseA = pow(raw_noiseA, 4.5f); 
            f32 t = lmath::unlerp(-1.0f,1.0f, raw_noiseA);
            i32 height = std::lerp(min_y,max_y, t);

            std::print("{}-",height);
            std::print("({}), ",raw_noiseA);
            res[cx + cz*CHUNK_XWIDTH] = height;
        }
        std::println();
    }
    return res;
}
static Chunk gen2D(ivec3 chunk_coord) {
    // |||||||||||||||||
    // step 0: biome gen
    // |||||||||||||||||

    // TODO:

    auto c = Chunk();

    return c;
}
static Chunk gen3D(ivec3 chunk_coord) {
    constexpr const f32 STEP_SIZE = 1.2f;
    constexpr const f32 AMPLITUDE = 16;
    constexpr const i32 BASE_HEIGHT = 0;
    auto                c = Chunk();

    auto generator = FastNoise::New<FastNoise::Perlin>();

    std::array<float, CHUNK_XWIDTH * CHUNK_ZWIDTH * CHUNK_HEIGHT> noise;

    vec3 chunk_offset = World::chunkToWorldPos(chunk_coord);
    auto minMax = generator->GenUniformGrid3D(
        noise.data(), chunk_offset.x, chunk_offset.y,
        chunk_offset.z,  // xOffset, yOffset - starting position in world space
        CHUNK_XWIDTH, CHUNK_HEIGHT, CHUNK_ZWIDTH,  // xCount, yCount - number of samples per axis
        STEP_SIZE, STEP_SIZE, STEP_SIZE,  // xStepSize, yStepSize - distance between samples
        1337                              // seed
    );
    for (i32 x = 0; x < CHUNK_XWIDTH; x++) {
        for (i32 z = 0; z < CHUNK_ZWIDTH; z++) {
            for (i32 y = 0; y < CHUNK_ZWIDTH; y++) {

                f32  noise_ = noise[(z * CHUNK_HEIGHT + y) * CHUNK_XWIDTH + x];
                auto bt = BlockType::AIR;
                if (noise_ > 0.0f) {
                    bt = BlockType::GRASS_BLOCK;
                }
                c.setBlock(bt, { x, y, z });
            }
        }
    }
    return c;
}
