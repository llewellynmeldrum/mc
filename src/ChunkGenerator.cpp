
#include "Block.hpp"
#include "ChunkHelpers.hpp"
#include "DebugFormat.hpp"
#include "DebugFormatSpecializations.hpp"
#include "Assertion.hpp"

#include "NoiseSystem.hpp"
#include "World.hpp"
#include "Logger.hpp"
#include "Types.h"

#include "lmath.hpp"
#include <queue>

//          |?         swamp       jungle
// humidity |spruce    plains      acacia
//          |snow      mesa?       desert
//          |--------------------------->
//          temperature

constexpr i32 SEA_LEVEL = 64;
constexpr i32 HEIGHT_LIMIT = 256;
constexpr i32 WORLD_SEED = 1337;

using namespace glm;

//ChunkTaskHeader:
//    ivec3 worldOffset;
//    u32 id;
//GenJob:
//    ChunkTaskHeader head;
//    u64 worldSeed;
//    GenConfig cfg;

//GenResult:
//    ChunkTaskHeader head;
//    ChunkStore chunkBlocks;
//    ChunkMetadata meta;
//    PendingWriteList deferredWrites;
//



void ChunkGenerator::genChunks(std::stop_token stopToken, 
                      Queue<GenJob>& input_queue, Queue<GenResult>& output_queue){
    
    struct BlockPalette {
        BlockType topsoil = BlockType::GRASS_BLOCK;     // i.e grass for plains, NULL for desert (none)
        BlockType soil = BlockType::DIRT_BLOCK;         // i.e dirt for plains, sand for desert.
        BlockType crust = BlockType::STONE_BLOCK;       // i.e stone for plains, sandstone for desert.
        BlockType deep_crust = BlockType::STONE_BLOCK;  // i.e stone for plains, stone for desert.
    } palette;

    // 0. configure everything

    while (!stopToken.stop_requested()){
        GenJob job = input_queue.wait_dequeue();
        GenResult res{job.chunkCoord};

        Noise2D heightNoise(NoiseType::OpenSimplex2);

        auto& cfg = job.cfg;
        const auto& worldBlockOffset = toWorldBlockPos(job.chunkCoord);
        struct Heightmap{
            std::array<u32, CHUNK_XWIDTH*CHUNK_ZWIDTH> buf;
            auto span(){
                return std::mdspan(buf.data(),CHUNK_XWIDTH, CHUNK_ZWIDTH);
            }
            u32& operator[](u32 x, u32 z){
                return span()[x,z];
            }
        }worldHeightmap;

        if (worldBlockOffset.y < 64){
            for (auto [x,y,z]: EachBlockInChunk()){
                res.chunkBlocks.at(x,y,z) = BlockType::STONE_BLOCK;
            }
        }
        /*
        // 1. create heightmap
        u32 maxVerticalDelta = +64; // above sea level, also the depth of the deepest valley
        for (const auto [x,z] : EachInRange(0, CHUNK_XWIDTH, 0, CHUNK_ZWIDTH)){
            const ivec2 blockWorldPos = {x+worldBlockOffset.x, z+worldBlockOffset.z};
            double n = heightNoise.sample(x,z); // -1 to 1 range
            worldHeightmap[x,z] = cfg.SEA_LEVEL + n*maxVerticalDelta;
        }

        // 2. apply heightmap
        for (const auto [x,z]: EachInRange(0,CHUNK_XWIDTH,0,CHUNK_ZWIDTH)){
            const auto& worldHeight = worldHeightmap[x,z];
            auto height = worldHeight-worldBlockOffset.y;
            for (const auto dy: EachInRange(0, CHUNK_HEIGHT)){
                // BUG: SEGFAULT HERE!
                // create assert less than, assert gthan

                auto y = dy+height;
                y = glm::clamp((i64)y, (i64)0, (i64)CHUNK_HEIGHT-1);
                ASSERT_LT(x,Chunk::Extents.x);
                ASSERT_LT(y,Chunk::Extents.y);
                ASSERT_LT(z,Chunk::Extents.z);
                assert(x>=0);
                assert(y>=0);
                assert(z>=0);
                res.chunkBlocks.at(x,y,z) = BlockType::STONE_BLOCK;

            }
        }
        */
        output_queue.wait_enqueue(res);
    }
    
    // 1. use 2d noise for heightmap, place Palette.crust from 0->height
    // 2. use 3d noise and thresholding to paint air blocks under certain Y (caves) 
    // 3. Paint blocks (soil on top N blocks, grass on top 1 block.)
    // 4. Paint ores in stone blocks below certain point
    // 5. Create trees.
}

