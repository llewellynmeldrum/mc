
#include "Block.hpp"
#include "DebugFormat.hpp"
#include "DebugFormatSpecializations.hpp"

#include "FastNoiseLite.h"
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
static void genChunks(std::stop_token stopToken, 
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
        Noise2D<FastNoiseLite::NoiseType::OpenSimplex2> heightNoise(job.worldSeed);


        GenResult res{job.head.worldOffset};
        auto& cfg = job.cfg;
        struct Heightmap{
            std::array<u32, CHUNK_XWIDTH*CHUNK_ZWIDTH> buf;
            u32& operator[](u32 x, u32 z){
                    return std::mdspan(buf.data(), CHUNK_XWIDTH, CHUNK_ZWIDTH)[x, z];
            }
        }heightmap;

        // 1. create heightmap
        u32 maxVerticalDelta = +64; // above sea level, also the depth of the deepest valley
        for (const auto [x,z] : EachInRange(0, CHUNK_XWIDTH, 0, CHUNK_ZWIDTH)){
            double n = heightNoise.sample(x,z); // -1 to 1 range
            heightmap[x,z] = cfg.SEA_LEVEL + n*maxVerticalDelta;
        }
        // 2. apply heightmap
        for (const auto [x,z]: EachInRange(0,CHUNK_XWIDTH,0,CHUNK_ZWIDTH)){
            const auto& height = heightmap[x,z];
            for (const auto y: EachInRange(0, height)){
                res.chunkBlocks[x,y,z] = BlockType::STONE_BLOCK;
            }
        }
    }
    
    // 1. use 2d noise for heightmap, place Palette.crust from 0->height
    // 2. use 3d noise and thresholding to paint air blocks under certain Y (caves) 
    // 3. Paint blocks (soil on top N blocks, grass on top 1 block.)
    // 4. Paint ores in stone blocks below certain point
    // 5. Create trees.
}

