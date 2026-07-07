
#include "Block.hpp"
#include "ChunkInvariants.hpp"
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

#include <tracy/Tracy.hpp>


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
    using mapped_type = value_type;
    using key_type = glm::ivec2;
    auto contains(key_type p) const noexcept{
        return p[0]>=0 && p[0]<static_cast<i32>(extentX)
            && p[1]>=0 && p[1]<static_cast<i32>(extentZ);
    }
    auto size()const noexcept{
        return buf.size();
    }

    decltype(auto) span(this auto& self){
        return std::mdspan(self.buf.data(),CHUNK_XWIDTH, CHUNK_ZWIDTH);
    }
    decltype(auto) at(this auto& self, glm::ivec2 v){
        return self.span()[v.x,v.y]; 
    }
    decltype(auto) operator[](this auto& self, size_t x, size_t z){
        return self.span()[x,z]; 
    }
};

static_assert(map_like<const Array2D<i32, Chunk::Extents.x, Chunk::Extents.z>>);

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
        auto v = glm::ivec2{cx,cz};
        return AT(chunk_buf,v);
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

// from https://www.reedbeta.com/blog/hash-functions-for-gpu-rendering/
u32 pcg_hash(u32 input) {
    u32 state = input * 747796405u + 2891336453u;
    u32 word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
    return (word >> 22u) ^ word;
}

u32 pcg_hash(std::pair<i32, i32> both) {
    return (pcg_hash(both.first) << 4) ^ pcg_hash(both.second);
}
f32 wpos_seeded_rand01(i32 wx, i32 wz){
    static constexpr u32 big_prime = 120'067;
    u32 hash = pcg_hash({wx, wz});
    hash = hash%big_prime;
    f32 zero_to_one = static_cast<f32>(hash)/static_cast<f32>(big_prime);
    assert(zero_to_one >= 0.0f && zero_to_one <= 1.0f);
    return zero_to_one;
}

static GenResult generateChunk(GenJob job){
    GenResult res{
        .chunkCoord=job.chunkCoord,
        .chunkBlocks = {},
        .deferredWrites = {},
    };
    auto& [chunk_coord, chunk, pendingWrites] = res;

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
    heightNoise2.setFractalOctaves(5);

    Noise3D caveNoise{NoiseType::Perlin};
    caveNoise.setSeed(job.worldSeed+1000);
    caveNoise.setFractalType(FractalType::FBm);
    caveNoise.setFractalOctaves(3);

    Noise2D tree_density{NoiseType::Perlin};
    tree_density.setFractalType(FractalType::FBm);
    tree_density.setFreq(0.0020);
    tree_density.setScale(1.0f);
    tree_density.setFractalOctaves(2);


    const auto world_block_origin = toWorldOrigin(job.chunkCoord);
    const auto& world_block_lo = world_block_origin;
    const auto& world_block_hi = toWorldOrigin(job.chunkCoord)+BlockOffset{Chunk::Extents};
    auto chunk_to_world = [world_block_origin](i32 cx, i32 cz){
        const auto& [wx_offset, wy_offset,wz_offset] = world_block_origin;
        return std::make_pair(cx + wx_offset, cz+wz_offset);
    };

    auto try_place_block = [world_block_lo, world_block_hi, &chunk, &pendingWrites, chunk_coord] (OverwritePolicy policy, WorldBlockPos wpos, BlockType bt){
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
            pendingWrites.emplace_back(policy, chunk_coord, wpos, bt);
        }
    };

    auto try_place_blocks = [try_place_block] (OverwritePolicy policy, WorldBlockPos min, WorldBlockPos max, BlockType bt){
        for (i32 y = min.y; y<max.y; y++){
            for (i32 x = min.x; x<max.x; x++){
                for (i32 z = min.z; z<max.z; z++){
                    try_place_block(policy, {x,y,z},bt);
                }
            }
        }
    };




    const HeightMap chunk_heightmap{heightNoise1, heightNoise2, gen_cfg, chunk_coord, job.worldSeed};



    // 1. Generate heightmap and set max height per xz column.
    {
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
                        try_place_block(
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
    // 4. TREES:
    {
        auto place_tree  = [chunk_coord, try_place_blocks](i32 cx,i32 cy, i32 cz){
            auto wpos = toWorldBlockPos(chunk_coord, {cx,cy,cz});
//            log_to_chunk(chunk_coord,"Placing tree @{}",wpos);
            try_place_blocks(
                OverwritePolicy::OnlyAir, 
                wpos, 
                wpos + BlockOffset{1,5,1}, 
                palette.wood_log
            );
            try_place_blocks(
                OverwritePolicy::OnlyAir, 
                wpos + BlockOffset{-1,4,-1}, 
                wpos + BlockOffset{2,6,2}, 
                palette.leaves
            );
        };
        const glm::ivec2 chunk_local_min = {0,0};
        const glm::ivec2 chunk_local_max = ivec2{Chunk::Extents.x, Chunk::Extents.z};
        ForEachInRangeEx(chunk_local_min, chunk_local_max,[&](i32 cx, i32 cz){
            const auto [wx, wz] = chunk_to_world(cx,cz);
            f32 density = tree_density.sample(wx,wz);
            // 1. find the max roll in a 3x3 area to prevent touching trees
            std::array<f32, 3*3> rolls;
            f32 max_adjacent_roll = 0.0f;
            for (i32 x = -1; x<=1; x++){
                for (i32 z = -1; z<=1; z++){
                    max_adjacent_roll = std::max(max_adjacent_roll, wpos_seeded_rand01(wx+x,wz+z));
                }
            }
            f32 roll = wpos_seeded_rand01(wx,wz);
            if (roll < max_adjacent_roll) return;
            if (roll < density * gen_cfg.tree_place_threshold){
                //log_to_chunk(chunk_coord,"{} -> density:{}, roll:{}", WorldChunkCoord{wx,wz},density,roll);
                i32 cy = chunk_heightmap.at_chunk(cx,cz);
                if (cy <= gen_cfg.SEA_LEVEL) return; // skip underwater trees
                place_tree(cx,cy,cz);
            }
        });
    }

    return res;
    // 4. Paint ores in stone blocks below certain point
    // 5. Create trees.
}

void ChunkGenerator::genChunks(std::stop_token stopToken, 
                      Queue<GenJob>& input_queue, Queue<GenResult>& output_queue){

    tracy::SetThreadName("chunk mesher");
    while (!stopToken.stop_requested()){
        GenJob job = input_queue.wait_dequeue();
        GenResult res = generateChunk(job);
        output_queue.wait_enqueue(res);
    }
    
}

