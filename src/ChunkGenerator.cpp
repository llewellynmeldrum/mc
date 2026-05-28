
#include "Block.hpp"
#include "ChunkConstants.hpp"
#include "ChunkHelpers.hpp"
#include "CoordTypes.hpp"
#include "DebugFormat.hpp"
#include "DebugFormatSpecializations.hpp"
#include "Assertion.hpp"
#include "CoordIteration.hpp"
#include "LM.hpp"

#include "NoiseSystem.hpp"
#include "World.hpp"
#include "Logger.hpp"
#include "Types.h"

#include "LM.hpp"
#include <print>
#include <queue>

//          |?         swamp       jungle
// humidity |spruce    plains      acacia
//          |snow      mesa?       desert
//          |--------------------------->
//          temperature

constexpr i32 WORLD_SEED = 1337;

using namespace glm;

struct BlockPalette {
    BlockType topsoil = BlockType::GRASS_BLOCK;     // i.e grass for plains, NULL for desert (none)
    BlockType soil = BlockType::DIRT_BLOCK;         // i.e dirt for plains, sand for desert.
    BlockType crust = BlockType::STONE_BLOCK;       // i.e stone for plains, sandstone for desert.
    BlockType deep_crust = BlockType::STONE_BLOCK;  // i.e stone for plains, stone for desert.
} palette;



template<std::size_t extentX, std::size_t extentZ>
struct i2D_Array{
    std::array<i32, extentX*extentZ> buf;

    decltype(auto) span(this auto& self){
        return std::mdspan(self.buf.data(),CHUNK_XWIDTH, CHUNK_ZWIDTH);
    }
    decltype(auto) operator[](this auto& self, i32 x, i32 z){
        return self.span()[x,z]; 
    }
};

struct HeightMap{
private:
    i2D_Array<Chunk::Extents.x, Chunk::Extents.z> chunk_buf;
    i2D_Array<Chunk::Extents.x, Chunk::Extents.z> world_buf;
public:
    HeightMap(Noise2D& hn1, Noise2D& hn2, GenConfig cfg, WorldChunkCoord chunkCoord, decltype(GenJob::worldSeed) seed)
    :heightNoise1(hn1),
    heightNoise2(hn2)
    {
        heightNoise1.setSeed(seed);
        heightNoise2.setSeed(seed);
        loadNoiseValues(cfg,chunkCoord);
    }
    ~HeightMap()=default;
    Noise2D& heightNoise1;
    Noise2D& heightNoise2;
    const f32 layer1_influence = 1.0f;
    const f32 layer2_influence = 0.5f;

    inline void loadNoiseValues(GenConfig cfg, WorldChunkCoord chunkCoord){
        WorldBlockPos worldChunkOffset = toWorldOrigin(chunkCoord);
        auto [chunk_wx_offset,chunk_wy_min,chunk_wz_offset] = worldChunkOffset;
        const auto chunk_wy_max = chunk_wy_min + Chunk::Extents.y;
        const auto& chunk_wy_offset = chunk_wy_min; // alias

        for (i32 lx = 0; lx< Chunk::Extents.x; lx++){
            for (i32 lz = 0; lz< Chunk::Extents.z; lz++){
                i32 wx = chunk_wx_offset + lx;
                i32 wz = chunk_wz_offset + lz;
                const f32 sample1 = heightNoise1.sample(wx,wz);
                const f32 sample2 = heightNoise2.sample(wx,wz);
                const f32 norm = sample1*layer1_influence + sample2*layer2_influence;
                i32 signed_elevation_delta = std::abs(cfg.MAX_ELEVATION_DELTA*norm);
                i32 raw_world_height = (cfg.MIN_HEIGHT + cfg.SEA_LEVEL + signed_elevation_delta);

                // constrain to world space chunk bounds
                i32 world_height = LM::constrain(chunk_wy_min, chunk_wy_max, raw_world_height); 

                i32 chunk_local_height = world_height - chunk_wy_offset;
                chunk_buf[lx,lz] = chunk_local_height;
                world_buf[lx,lz] = raw_world_height;

            }
        }
    }
    i32 at_chunk(i32 cx, i32 wz)const {
        return chunk_buf[cx,wz];
    }
    i32 at_world(i32 cx, i32 wz)const {
        return world_buf[cx,wz];
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
    auto& [chunkCoord, blocks, meta, deferredWrites] = res;

    //GenConfig;
    const auto& gen_cfg = job.cfg;

    Noise2D heightNoise1{NoiseType::Perlin};
    heightNoise1.setFractalType(FractalType::FBm);
    heightNoise1.setFreq(0.001);
    heightNoise1.setFractalOctaves(2);

    Noise2D heightNoise2{NoiseType::Perlin};
    heightNoise2.setFractalType(FractalType::FBm);
    heightNoise2.setFreq(0.01);
    heightNoise2.setFractalOctaves(3);

    Noise3D caveNoise{NoiseType::Perlin};
    caveNoise.setFractalType(FractalType::FBm);
    caveNoise.setFractalOctaves(4);




    const HeightMap chunk_heightmap{heightNoise1, heightNoise2, gen_cfg, chunkCoord, job.worldSeed};

    const auto world_block_origin = toWorldOrigin(job.chunkCoord);


    // 1. Generate heightmap and set max height per xz column.
    {
        i2D_Array<Chunk::Extents.x, Chunk::Extents.y> heightAt;
        const glm::ivec2 chunkLocalMin = {0,0};
        const glm::ivec2 chunkLocalMax =ivec2{Chunk::Extents.x, Chunk::Extents.z};
        // 1. Apply heightmap to do something icant rememebr
        ForEachInRangeEx(chunkLocalMin,chunkLocalMax,[&](i32 cx, i32 cz){
            blocks.setColumn({cx,0,cz},chunk_heightmap.at_chunk(cx,cz), palette.crust);
        });
    }

    {
        // 2. generate 3d noise  for caves, threshhold out low vals
        const glm::ivec3 chunkLocalMin = {0,0,0};
        const glm::ivec3 chunkLocalMax = ivec3{Chunk::Extents.x, Chunk::Extents.y, Chunk::Extents.z};
        ForEachInRangeEx(chunkLocalMin,chunkLocalMax,[&](i32 cx, i32 cy, i32 cz){
            i32 wx = world_block_origin.x+cx;
            i32 wy = world_block_origin.y+cy;
            i32 wz = world_block_origin.z+cz;
            if (wy<gen_cfg.cave_y_threshold){ 
                f32 noise_val = caveNoise.sample(wx,wy,wz);
                if (noise_val < gen_cfg.cave_air_threshold){
                    blocks.set(cx,cy,cz, BlockType::AIR);
                }
            }
        });
    }

    // 3. paint blocks
    {
        const glm::ivec2 chunkLocalMin = {0,0};
        const glm::ivec2 chunkLocalMax =ivec2{Chunk::Extents.x, Chunk::Extents.z};
        ForEachInRangeEx(chunkLocalMin,chunkLocalMax,[&](i32 cx, i32 cz){
            i32 w_maxy = chunk_heightmap.at_world(cx,cz);
            i32 c_maxy = chunk_heightmap.at_chunk(cx,cz);
            ForEachInRangeEx(0,c_maxy,[&](i32 cy){
                i32 wy = cy+world_block_origin.y;
                i32 dist_to_top_block = w_maxy - wy;
                if (dist_to_top_block==1){
                    blocks.set(cx,cy,cz,palette.topsoil);
                }else if (dist_to_top_block < 5){
                    blocks.set(cx,cy,cz,palette.soil);
                }
            });
        });
    }
    


    // 1. use 2d noise for heightmap, place Palette.crust from 0->height


    return res;
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

