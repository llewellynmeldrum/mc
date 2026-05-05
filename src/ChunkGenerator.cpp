
#include "DebugFormat.hpp"
#include "DebugFormatSpecializations.hpp"

#include "World.hpp"
#include "Logger.hpp"
#include "Types.h"

#include <FastNoise/FastNoise.h>
#include "lmath.hpp"
using NoiseSamples2D = std::array<f32, CHUNK_XWIDTH * CHUNK_ZWIDTH>;

//         /\
//          |?         swamp       jungle
// humidity |spruce    plains      acacia
//          |snow      mesa?       desert
//          |--------------------------->
//          temperature

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
        const i32 max_mountain_height = 72;
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

BiomeMap PLAINS_ONLY(CHUNK_ZWIDTH* CHUNK_XWIDTH, &plains);

TerrainNoise::TerrainNoise() {
    base = FastNoise::New<FastNoise::Simplex>();
    hills = FastNoise::New<FastNoise::Simplex>();
    valleys = FastNoise::New<FastNoise::Simplex>();
    detail = FastNoise::New<FastNoise::Simplex>();

    base->SetScale(200.0f);
    hills->SetScale(100.0f);
    valleys->SetScale(100.0f);
    detail->SetScale(25.0f);

    hills->SetSeedOffset(100.0f);
    valleys->SetSeedOffset(400.0f);
    detail->SetSeedOffset(200.0f);
}
template <typename T>
auto random_range(T min, T max) {
    f32 s = rand() / (f32)RAND_MAX;
    T   range = max - min;
    return min + static_cast<T>(s * range);
}

ChunkDataPair ChunkGenerator::gen(ivec3 chunk_coord) {
    Chunk res;

    const ivec3 chunk_offset = World::chunkToWorldPos(chunk_coord);
    // |||||||||||||||||
    // step 0: biome selection
    // |||||||||||||||||
    const auto& biomes = genChunkBiomeMap(chunk_coord, chunk_offset);

    // |||||||||||||||||
    // step 1: heightmap
    // |||||||||||||||||
    // -> based on biome for each block, a heightmap is created,
    //    representing the highest point for each xz in the chunk
    const i32  chunk_min_world_y = chunk_coord.y * CHUNK_HEIGHT;
    const i32  chunk_max_world_y = chunk_min_world_y + CHUNK_HEIGHT;
    const auto heightmap = genChunkHeightmap(biomes, chunk_coord, chunk_offset);

    for (i32 cx = 0; cx < CHUNK_XWIDTH; cx++) {
        for (i32 cz = 0; cz < CHUNK_ZWIDTH; cz++) {
            i32         idx = cx + CHUNK_XWIDTH * cz;
            const auto  world_height = heightmap[idx];
            const auto* biome = biomes[idx];
            if (chunk_min_world_y > world_height)
                continue;
            const i32 local_height = std::min(CHUNK_HEIGHT, world_height - chunk_offset.y);
            res.setColumn(biome->palette.crust, { cx, 0, cz }, local_height);
        }
    }
    // TODO: when biomes have the noisemaps and shit for temp/humidity apply them here as well
    ChunkMetadata meta;
    for (i32 x = 0; x < CHUNK_XWIDTH; x++) {
        for (i32 y = 0; y < CHUNK_HEIGHT; y++) {
            for (i32 z = 0; z < CHUNK_ZWIDTH; z++) {
                meta.blockTemperature[x, y, z] = 1.0;
            }
        }
    }
    // |||||||||||||||||
    // step 3: Caves ( TODO:)
    // |||||||||||||||||

    // |||||||||||||||||
    // step 4: Paint blocks( TODO:)
    // |||||||||||||||||
    return { res, meta };
    for (i32 cx = 0; cx < CHUNK_XWIDTH; cx++) {
        for (i32 cz = 0; cz < CHUNK_ZWIDTH; cz++) {
            i32 idx = cx + CHUNK_XWIDTH * cz;
            // chunk paintin is broken
            const auto& palette = biomes[idx]->palette;
            const auto  world_height = heightmap[idx];
            // only care about the topmost chunk
            if (chunk_min_world_y > world_height || chunk_max_world_y < world_height)
                continue;
            const i32 local_height = std::min(CHUNK_HEIGHT, world_height - chunk_offset.y);

            // top 10-12 blocks = crust
            // top 4-5 blocks  = soil
            i32 crust_start = std::max(0, local_height - random_range<i32>(10, 12));
            for (i32 y = crust_start; y >= 0; y--) {
                res.setBlock(palette.crust, cx, cx, y);
            }
            i32 soil_start = std::max(0, local_height - random_range<i32>(4, 5));
            for (i32 y = soil_start; y >= 0; y--) {
                res.setBlock(palette.soil, cx, cx, y);
            }
            i32 topsoil_y = local_height;
            res.setBlock(palette.topsoil, cx, cx, topsoil_y);
            // top 1 block  = grass
        }
    }

    return { res, meta };
}

BiomeMap ChunkGenerator::genChunkBiomeMap(ivec3 chunk_coord, ivec3 chunk_offset) {
    return PLAINS_ONLY;
}

NoiseSamples2D sampleNoiseMap(const auto& noise_map, ivec3 chunk_offset) {
    NoiseSamples2D res;
    noise_map->GenUniformGrid2D(res.data(), chunk_offset.x, chunk_offset.z, CHUNK_XWIDTH,
                                CHUNK_ZWIDTH, 1.0, 1.0, WORLD_SEED);
    return res;
}
HeightMap ChunkGenerator::genChunkHeightmap(const BiomeMap& block_biomes, ivec3 chunk_coord,
                                            ivec3 chunk_offset) {
    auto base_map = sampleNoiseMap(terrain_noise.base, chunk_offset);
    auto hills_map = sampleNoiseMap(terrain_noise.hills, chunk_offset);
    auto valleys_map = sampleNoiseMap(terrain_noise.valleys, chunk_offset);
    auto detail_map = sampleNoiseMap(terrain_noise.detail, chunk_offset);

    HeightMap res;
    for (i32 cx = 0; cx < CHUNK_XWIDTH; cx++) {
        for (i32 cz = 0; cz < CHUNK_ZWIDTH; cz++) {
            i32         idx = cx * CHUNK_ZWIDTH + cz;
            const auto* biome = block_biomes[idx];

            f32 base = lmath::unlerp(-1.0f, 1.0f, base_map[idx]);
            f32 hills = lmath::unlerp(-1.0f, 1.0f, hills_map[idx]);
            f32 detail = lmath::unlerp(-1.0f, 1.0f, detail_map[idx]);
            f32 rivers = lmath::unlerp(-1.0f, 1.0f, valleys_map[idx]);

            f32  land = smoothstep(0.1f, 0.8f, base);
            auto hills_mask = smoothstep(0.6f, 0.8f, base);
            auto rivers_mask = smoothstep(0.6f, 0.8f, base);
            // clang-format off
            f32  height =                  //
                +   SEA_LEVEL                 //
                +   land * 10.0f              //
                +   hills * hills_mask * 10.0f  //
                -   rivers * rivers_mask * 20.0f
                +    detail * 2.0f;           //

            // clang-format on
            res[idx] = static_cast<decltype(res)::value_type>(std::floor(height));
        }
    }
    return res;
}
