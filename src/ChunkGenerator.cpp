
#include "Block.hpp"
#include "ChunkConstants.hpp"
#include "ChunkHelpers.hpp"
#include "CoordTypes.hpp"
#include "DebugFormat.hpp"
#include "FormatSpecs.hpp"
#include "Assertion.hpp"
#include "CoordIteration.hpp"
#include "LM.hpp"

#include "NoiseSystem.hpp"
#include "PendingBlockWrites.hpp"
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


using namespace glm;

struct BlockPalette {
    BlockType topsoil = BlockType::GRASS_BLOCK;     // i.e grass for plains, NULL for desert (none)
    BlockType soil = BlockType::DIRT_BLOCK;         // i.e dirt for plains, sand for desert.
    BlockType crust = BlockType::STONE_BLOCK;       // i.e stone for plains, sandstone for desert.
    BlockType deep_crust = BlockType::STONE_BLOCK;  // i.e stone for plains, stone for desert.
    BlockType river_bed = BlockType::DIRT_BLOCK;  // i.e stone for plains, stone for desert.
    BlockType wood_log = BlockType::OAK_LOG;
    BlockType leaves = BlockType::OAK_LEAF;
} palette;



template<typename value_type, std::size_t extentX, std::size_t extentZ>
struct Array2D{
    std::array<value_type, extentX*extentZ> buf;

    decltype(auto) span(this auto& self){
        return std::mdspan(self.buf.data(),CHUNK_XWIDTH, CHUNK_ZWIDTH);
    }
    decltype(auto) operator[](this auto& self, size_t x, size_t z){
        return self.span()[x,z]; 
    }
};

struct HeightMap{
private:
    Array2D<i32, Chunk::Extents.x, Chunk::Extents.z> chunk_buf;
    Array2D<i32, Chunk::Extents.x, Chunk::Extents.z> world_buf;
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
    const f32 layer1_influence = heightNoise1.scale;
    const f32 layer2_influence = heightNoise2.scale;

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
                i32 signed_elevation_delta =cfg.MAX_ELEVATION_DELTA*norm;
                i32 raw_world_height = (cfg.TERRAIN_HEIGHT() + signed_elevation_delta);

                // constrain to world space chunk bounds
                i32 world_height = LM::constrain(chunk_wy_min, chunk_wy_max, raw_world_height); 

                i32 chunk_local_height = world_height - chunk_wy_offset;
                chunk_buf[lx,lz] = chunk_local_height;
                world_buf[lx,lz] = raw_world_height;

            }
        }
    }
    i32 at_chunk(i32 cx, i32 cz)const {
        return chunk_buf[cx,cz];
    }
    i32 at_world(i32 wx, i32 wz)const {
        return world_buf[wx,wz];
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
    auto& [chunkCoord, chunk, meta, pendingWrites] = res;

    constexpr glm::ivec2 chunkLocalMin2 = {0,0};
    constexpr  glm::ivec2 chunkLocalMax2 =ivec2{Chunk::Extents.x, Chunk::Extents.z};
    //GenConfig;
    const GenConfig& gen_cfg = job.cfg;

    Noise2D heightNoise1{NoiseType::Perlin};
    heightNoise1.setFractalType(FractalType::FBm);
    heightNoise1.setFreq(0.001);
    heightNoise1.setScale(1.2f);
    heightNoise1.setFractalOctaves(2);

    Noise2D heightNoise2{NoiseType::Perlin};
    heightNoise2.setFractalType(FractalType::FBm);
    heightNoise2.setFreq(0.005);
    heightNoise2.setScale(0.8f);
    heightNoise2.setFractalOctaves(3);

    Noise3D caveNoise{NoiseType::Perlin};
    caveNoise.setSeed(job.worldSeed+1000);
    caveNoise.setFractalType(FractalType::FBm);
    caveNoise.setFractalOctaves(3);


    const auto world_block_origin = toWorldOrigin(job.chunkCoord);
    const auto& world_block_lo = world_block_origin;
    const auto& world_block_hi = toWorldOrigin(job.chunkCoord)+BlockOffset{Chunk::Extents};

    // BUG: This is broken, pending writes dont apply properly.
    auto tryBlockWrite = [&chunk, world_block_lo, world_block_hi, chunkCoord, &pendingWrites]
        (OverwritePolicy policy, WorldBlockPos wpos, BlockType bt){
        bool writeIsWithinChunkBounds = LM::isVecInBounds(wpos, world_block_lo, world_block_hi);
        if (writeIsWithinChunkBounds){
            // do it immediately
            i32 cx = wpos.x - world_block_lo.x;
            i32 cy = wpos.y - world_block_lo.y;
            i32 cz = wpos.z - world_block_lo.z;
            if (canMakeWrite(policy, chunk.at(cx,cy,cz))){
                chunk.set(cx,cy,cz, bt);
            }
        }else{
            pendingWrites.emplace_back(policy, chunkCoord, wpos, bt);
        }
    };




    const HeightMap chunk_heightmap{heightNoise1, heightNoise2, gen_cfg, chunkCoord, job.worldSeed};



    // 1. Generate heightmap and set max height per xz column.
    {
        Array2D<i32, Chunk::Extents.x, Chunk::Extents.y> heightAt;
        const glm::ivec2 chunkLocalMin = {0,0};
        const glm::ivec2 chunkLocalMax =ivec2{Chunk::Extents.x, Chunk::Extents.z};
        ForEachInRangeEx(chunkLocalMin,chunkLocalMax,[&](i32 cx, i32 cz){
            chunk.setColumn({cx,0,cz},chunk_heightmap.at_chunk(cx,cz), palette.crust);
        });
    }

    // 2. paint blocks based on their position
    {
        const glm::ivec3 chunkLocalMin = {0,0,0};
        const glm::ivec3 chunkLocalMax = ivec3{Chunk::Extents};
        ForEachInRangeEx(chunkLocalMin,chunkLocalMax,[&](i32 cx, i32 cy, i32 cz){
            i32 wx = cx+world_block_origin.x;
            i32 wz = cz+world_block_origin.z;
            i32 w_top_block_y = chunk_heightmap.at_world(cx,cz);
            i32 c_top_block_y = chunk_heightmap.at_chunk(cx,cz);
            i32 wy = cy+world_block_origin.y;

            i32 dist_to_top_block = w_top_block_y - wy;

            if (dist_to_top_block<0){ // we are above the top block
                if (wy < gen_cfg.SEA_LEVEL){
                    if(chunk.at(cx,cy,cz)==BlockType::AIR){
                        chunk.set(cx,cy,cz,BlockType::WATER_BLOCK);
                        auto wpos_below = WorldBlockPos{wx,wy-1,wz};
                        tryBlockWrite(
                            OverwritePolicy::OnlyGrass, 
                            wpos_below, 
                            palette.river_bed
                        );
                    }
                }
            } else if (dist_to_top_block <= 1){
                chunk.set(cx,cy,cz,palette.topsoil);
            } else if (dist_to_top_block <= 5){
                chunk.set(cx,cy,cz,palette.soil);
            }
        });
    }

    
    // 3. generate 3d noise  for caves, threshhold out low vals
    const glm::ivec3 chunkLocalMin = {0,0,0};
    const glm::ivec3 chunkLocalMax = ivec3{Chunk::Extents.x, Chunk::Extents.y, Chunk::Extents.z};
    ForEachInRangeEx(chunkLocalMin,chunkLocalMax,[&](i32 cx, i32 cy, i32 cz){
        i32 wx = world_block_origin.x+cx;
        i32 wy = world_block_origin.y+cy;
        i32 wz = world_block_origin.z+cz;
        if (wy<gen_cfg.cave_y_threshold){ 
            f32 noise_val = caveNoise.sample(wx,wy,wz);
            f32 height_factor = wy / -150.0f;
            noise_val/=height_factor;
            // at -150 blocks, 2
            // at 150 blocks, 0.5
            if (chunk.at(cx,cy,cz) == BlockType::STONE_BLOCK){
                if (noise_val < gen_cfg.cave_air_threshold){
                    // only replace stone blocks with caves
                    chunk.set(cx,cy,cz, BlockType::AIR);
                }else{
                    // TODO: If we reach here, the block has not been nuked by cave noise
                    // therefore put some ore here with some very low threshold, high octave 3d noise
                }
            }
        }
    });

    
    // NOTE: 
    // PLACE TREES
    auto tree_height = [&job](i32 cx, i32 cz){
        srand(job.worldSeed+cx+cz);
        return static_cast<i32>(std::round(randf(4,6)));
    };
    Noise2D treeDensityNoise(NoiseType::Perlin,job.worldSeed);
    treeDensityNoise.setFractalType(FractalType::FBm);
    treeDensityNoise.setFreq(0.005);
    treeDensityNoise.setScale(0.8f);
    treeDensityNoise.setFractalOctaves(3);
    auto placeTree = [chunkCoord, &chunk, tryBlockWrite, tree_height](f32 noise, i32 cx, i32 cy, i32 cz){
        const auto wpos = WorldBlockPos{chunkCoord.raw()*Chunk::Extents} +BlockOffset{cx,cy,cz};
        WorldBlockPos iwpos = wpos;
        for (i32 dy = 0; dy<tree_height(cx,cz); dy++){
            iwpos.y = wpos.y + dy;
            tryBlockWrite(
                OverwritePolicy::OnlyAir, 
                iwpos, 
                palette.wood_log
            );
        }
        ForEachInRangeEx(ivec3{-1,0,-1},ivec3{2,2,2},[&](i32 ix, i32 iy, i32 iz){
            tryBlockWrite(
                OverwritePolicy::OnlyAir, 
                WorldBlockPos{iwpos.raw()+ivec3{ix,iy,iz}},
                palette.leaves
            );

        });
        tryBlockWrite(
            OverwritePolicy::OnlyAir, 
            WorldBlockPos{iwpos.raw()+ivec3{-1,2,0}},
            palette.leaves
        );
        tryBlockWrite(
            OverwritePolicy::OnlyAir, 
            WorldBlockPos{iwpos.raw()+ivec3{1,2,0}},
            palette.leaves
        );
        tryBlockWrite(
            OverwritePolicy::OnlyAir, 
            WorldBlockPos{iwpos.raw()+ivec3{0,2,1}},
            palette.leaves
        );
        tryBlockWrite(
            OverwritePolicy::OnlyAir, 
            WorldBlockPos{iwpos.raw()+ivec3{0,2,-1}},
            palette.leaves
        );
        tryBlockWrite(
            OverwritePolicy::OnlyAir, 
            WorldBlockPos{iwpos.raw()+ivec3{0,2,0}},
            palette.leaves
        );
    };

//    treeDensityNoise.setScale(1.5);

    ForEachInRangeEx(chunkLocalMin2,chunkLocalMax2,[&](i32 cx, i32 cz){
        i32 cy = chunk_heightmap.at_chunk(cx,cz);
        i32 wx{chunkCoord.x*Chunk::Extents.x + cx};
        i32 wz{chunkCoord.z*Chunk::Extents.z + cz};
        i32 w_top_block_y = chunk_heightmap.at_world(cx,cz);
        i32 wy = chunkCoord.y*Chunk::Extents.y;
        srand(job.worldSeed+wx+wz);
        i32 dist_to_top_block = w_top_block_y - wy;
        if (cy>=1 && chunk.at(cx,cy-1,cz)==BlockType::GRASS_BLOCK){
            f32 noise = treeDensityNoise.sample(wx,wz);

            if (noise > gen_cfg.tree_distribution_threshold){
                if (noise > gen_cfg.tree_place_threshold){
                    placeTree(noise, cx,cy+1,cz);
                }
            }

        }

    });

    return res;
    // 4. Paint ores in stone blocks below certain point
    // 5. Create trees.
}

void ChunkGenerator::genChunks(std::stop_token stopToken, 
                      Queue<GenJob>& input_queue, Queue<GenResult>& output_queue){
    while (!stopToken.stop_requested()){
        GenJob job = input_queue.wait_dequeue();
        GenResult res = generateChunk(job);
        output_queue.wait_enqueue(res);
    }
    
}

