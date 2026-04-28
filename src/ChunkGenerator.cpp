
#include "World.hpp"
#include "Logger.hpp"

#include <FastNoise/FastNoise.h>
static Chunk gen3D(ivec3 chunk_coord);
static Chunk gen2D(ivec3 chunk_coord);
Chunk
ChunkGenerator::gen(ivec3 chunk_coord) {
    return gen2D(chunk_coord);
}
static Chunk
gen3D(ivec3 chunk_coord) {
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
static Chunk
gen2D(ivec3 chunk_coord) {
    constexpr const f32 AMPLITUDE = 48;
    constexpr const i32 BASE_HEIGHT = -48;
    auto                c = Chunk();

    auto generator = FastNoise::New<FastNoise::Perlin>();

    vec3 chunk_offset = World::chunkToWorldPos(chunk_coord);
    std::array<float, CHUNK_XWIDTH * CHUNK_ZWIDTH> noise1;
    auto                                           minMax = generator->GenUniformGrid2D(
        noise1.data(), chunk_offset.x,
        chunk_offset.z,              // xOffset, yOffset - starting position in world space
        CHUNK_XWIDTH, CHUNK_ZWIDTH,  // xCount, yCount - number of samples per axis
        1.0, 1.0,
        1339  // seed
    );
    std::array<float, CHUNK_XWIDTH * CHUNK_ZWIDTH> noise2;
    auto                                           minMax2 = generator->GenUniformGrid2D(
        noise2.data(), chunk_offset.x,
        chunk_offset.z,              // xOffset, yOffset - starting position in world space
        CHUNK_XWIDTH, CHUNK_ZWIDTH,  // xCount, yCount - number of samples per axis
        1.0, 1.0,
        1337  // seed
    );
    for (i32 cx = 0; cx < CHUNK_XWIDTH; cx++) {
        for (i32 cz = 0; cz < CHUNK_ZWIDTH; cz++) {

            i32 x = cx + chunk_offset.x;
            i32 z = cx + chunk_offset.x;
            f32 noise_height = ((noise1[(cz * CHUNK_XWIDTH + cx)] + 1.0f) * 0.5f) +
                               ((noise2[(cz * CHUNK_XWIDTH + cx)] + 1.0f) * 0.5f);
            noise_height = pow(noise_height, 4.5f);
            i32 world_height = BASE_HEIGHT + noise_height * AMPLITUDE;
            i32 cy = max(world_height - chunk_offset.y, (f32)0);
            cy = min(cy, (i32)CHUNK_HEIGHT);
            if (cy == 0)
                continue;
            c.setBlocks(BlockType::DIRT_BLOCK, { cx, 0, cz }, { cx + 1, cy - 1, cz + 1 });
            c.setBlocks(BlockType::GRASS_BLOCK, { cx, cy - 1, cz }, { cx + 1, cy, cz + 1 });
        }
    }
    return c;
}
