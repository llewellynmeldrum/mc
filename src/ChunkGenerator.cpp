
#include "Block.hpp"
#include "ChunkHelpers.hpp"
#include "DebugFormat.hpp"
#include "DebugFormatSpecializations.hpp"
#include "Assertion.hpp"
#include "CoordIteration.hpp"

#include "NoiseSystem.hpp"
#include "World.hpp"
#include "Logger.hpp"
#include "Types.h"

#include "LM.hpp"
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

struct BlockPalette {
    BlockType topsoil = BlockType::GRASS_BLOCK;     // i.e grass for plains, NULL for desert (none)
    BlockType soil = BlockType::DIRT_BLOCK;         // i.e dirt for plains, sand for desert.
    BlockType crust = BlockType::STONE_BLOCK;       // i.e stone for plains, sandstone for desert.
    BlockType deep_crust = BlockType::STONE_BLOCK;  // i.e stone for plains, stone for desert.
} palette;



struct Heightmap{
private:
    std::array<u32, CHUNK_XWIDTH*CHUNK_ZWIDTH> buf;
    decltype(auto) span(this auto& self){
        return std::mdspan(self.buf.data(),CHUNK_XWIDTH, CHUNK_ZWIDTH);
    }
    decltype(auto) operator[](this auto& self, u32 x, u32 z){
        return self.span()[x,z]; 
    }
    auto* data(this auto& self){return self.buf.data();}
public:
    void height_at(){
    }
};
// 1. make a lambda function which allows for faster iteration of coordinate ranges
// - Make it iterate over a glm vec object temporary which it mutates
//
// Ideally, i would like to adjust generation params and get reloads of all chunks.
//


static GenResult generateChunk(GenJob job){
    GenResult res{
        .chunkCoord=job.chunkCoord,
        .chunkBlocks = {},
        .meta = {},
        .deferredWrites = {},
    };
    auto& [chunkCoord, block, meta, deferredWrites] = res;
    Noise2D heightNoise{NoiseType::OpenSimplex2};

    const auto& gen_cfg = job.cfg;
    const auto& worldBlockPos = toChunkOrigin(job.chunkCoord);

    auto min = ChunkBlockPos{worldBlockPos.raw()};
    auto max = min+toBlockOffset(Chunk::Extents);
    if (worldBlockPos.y < 64){
        ForEachCoordInRangeEx(min,max,[&res](auto v){
            res.chunkBlocks.at(v) = BlockType::STONE_BLOCK;
        });
    }
    return res;
    // 1. use 2d noise for heightmap, place Palette.crust from 0->height
    // 2. use 3d noise and thresholding to paint air blocks under certain Y (caves) 
    // 3. Paint blocks (soil on top N blocks, grass on top 1 block.)
    // 4. Paint ores in stone blocks below certain point
    // 5. Create trees.
}

void ChunkGenerator::genChunks(std::stop_token stopToken, 
                      Queue<GenJob>& input_queue, Queue<GenResult>& output_queue){
    

    // 0. configure everything

    while (!stopToken.stop_requested()){
        GenJob job = input_queue.wait_dequeue();
        GenResult res = generateChunk(job);
        output_queue.wait_enqueue(res);
    }
    
}

