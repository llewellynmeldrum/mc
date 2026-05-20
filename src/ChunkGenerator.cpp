
#include "Block.hpp"
#include "DebugFormat.hpp"
#include "DebugFormatSpecializations.hpp"
#include "DEBUG.hpp"

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
//
struct AssertionPassthrough{
    int line;
    std::string_view pretty_fn;
};
#define ASSERT_LT(a,b) assert_lt(a,b,#a,#b,__FILE_NAME__, __FUNCTION__, __PRETTY_FUNCTION__, __LINE__)

template<typename ...Args>
constexpr inline bool assert_lt(auto a, auto b,
                                std::string_view str_a, std::string_view str_b,
                                std::string_view file_name, std::string_view fn, std::string_view pretty_fn, int line, Args... vargs){
    if (!(a<b)){
        println( stderr, "\e[31;1;4m"
            "Assertion FAILED in {"/*FILE*/"}:{"/*LINE*/"} !\n"     "\e[0m\e[31;1m"
             "Expected:{} {}{}{} < {}{}{},"                 "\n"
              "{}Where{} {}{}{}={}{}{}, {}{}{}={}{}{}."                         "\n"
               "{}in function:{} {}",
            file_name, line,
             fmt::clear, fmt::blue, str_a, fmt::clear, fmt::blue,  str_b, fmt::clear,
              fmt::red,fmt::clear, fmt::blue, str_a, fmt::clear, fmt::orange, a, fmt::clear,
                    fmt::blue, str_b, fmt::clear, fmt::orange, b, fmt::clear,
               fmt::red, pretty_fn, fmt::clear);                                         

        DEBUG_BREAKPOINT_QUIET();
        return false;
    }
    return true;
}
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
        GenResult res{job.head.chunkCoord};

        Noise2D heightNoise(NoiseType::OpenSimplex2);

        auto& cfg = job.cfg;
        const auto& worldBlockOffset = toWorldBlockPos(job.head.chunkCoord);
        struct Heightmap{
            std::array<u32, CHUNK_XWIDTH*CHUNK_ZWIDTH> buf;
            auto span(){
                return std::mdspan(buf.data(),CHUNK_XWIDTH, CHUNK_ZWIDTH);
            }
            u32& operator[](u32 x, u32 z){
                return span()[x,z];
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
            const auto& worldHeight = heightmap[x,z];
            auto height = worldHeight-worldBlockOffset.y;
            for (const auto y: EachInRange(0, height)){
                // BUG: SEGFAULT HERE!
                height = glm::clamp((i64)height, (i64)0, (i64)CHUNK_HEIGHT);
                // create assert less than, assert gthan

                ASSERT_LT(x,Chunk::Extents.x);
                ASSERT_LT(y,Chunk::Extents.y);
                ASSERT_LT(z,Chunk::Extents.z);
                assert(x>=0);
                assert(y>=0);
                assert(z>=0);
                res.chunkBlocks.at(x,y,z) = BlockType::STONE_BLOCK;

            }
        }
        output_queue.wait_enqueue(res);
    }
    
    // 1. use 2d noise for heightmap, place Palette.crust from 0->height
    // 2. use 3d noise and thresholding to paint air blocks under certain Y (caves) 
    // 3. Paint blocks (soil on top N blocks, grass on top 1 block.)
    // 4. Paint ores in stone blocks below certain point
    // 5. Create trees.
}

