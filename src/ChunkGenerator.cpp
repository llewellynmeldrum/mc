
#include <print>
#include <queue>

#include "CoordTypes.hpp"
#include "CoordIteration.hpp"

#include "Block.hpp"
#include "ChunkInvariants.hpp"
#include "ChunkHelpers.hpp"
#include "DebugFormat.hpp"
#include "FormatSpecs.hpp"
#include "Assertion.hpp"
#include "LM.hpp"

#include "NoiseSystem.hpp"
#include "PendingBlockWrites.hpp"
#include "World.hpp"
#include "Logger.hpp"
#include "Types.h"
#include "Array2D.hpp"
#include "ChunkNoiseDebug.hpp"

#include "Biomes.hpp"
#include "WorldGen_BiomeClassification.hpp"
#include "WorldGen_NoiseGeneration.hpp"
#include "WorldGen_Config.hpp"
#include "WorldGen_Utils.hpp"

#include "glm_math_extensions.hpp"

#include "FastNoiseLite.h"
#include "tracy/Tracy.hpp"

using HeightMap2D = ArrayList2D<i32>;

struct GenContext{
    GenContext(WorldBlockPos _world_block_origin, const GenConfig& _cfg) :
        cfg(_cfg),
        world_block_origin(_world_block_origin) 
    {}
    const GenConfig& cfg;
    WorldBlockPos world_block_origin;

    auto to_world (i32 cx, i32 cz) const {
        const auto& [wx_offset, wy_offset,wz_offset] = world_block_origin;
        return std::make_pair(cx + wx_offset, cz + wz_offset);
    };
    auto gen_heightmap ();
    // base the heightmap mostly off of noise but also biome slightly?
};
//auto GenContext::gen_heightmap (){
//    // TODO: world should probably own these, otherwise changes to world seed will not go through
//    const static NoiseGen hill_noisegen(NoiseConfig{
//        .type = NoiseType::Perlin,
//        .seed = cfg.world_seed + 1234,
//        .freq = 0.005f,
//        .frac_type = FractalType::FBm,
//        .frac_octaves = 5,
//    });
//    const static NoiseGen mountain_noisegen(NoiseConfig{
//        .type = NoiseType::Perlin,
//        .seed = cfg.world_seed + 4321,
//        .freq = 0.002f,
//        .frac_type = FractalType::Ridged,
//        .frac_octaves = 4,
//    });
//
//
//    const glm::ivec2 chunk_local_min = {0,0};
//    const glm::ivec2 chunk_local_max = glm::ivec2{Chunk::Extents.x, Chunk::Extents.z};
//    ForEachInRangeEx(chunk_local_min,chunk_local_max,[&](i32 cx, i32 cz){
//        using LM::unlerp;
//        auto [wx,wz] = to_world(cx,cz);
//        i32 continentalness={}, hill_dy={}, mount_dy={};
//        f32 cont_noise = continental_noisemap[cx,cz];
//        continentalness = remap_curve(
//            cont_noise,
//            {
//                {+0.00f, cfg.sea_level-80},
//                {+0.40f, cfg.sea_level-70},
//                {+0.50f, cfg.sea_level},
//                {+0.80f, cfg.sea_level+10},
//                {+1.00f, cfg.sea_level+15},
//            }
//        );
//        f32 hill_noise = hill_noisegen.sample(wx,wz);
//        hill_dy = remap_curve(
//            hill_noise,
//            {
//                {+0.00f, 0},
//                {+0.40f, 5},
//                {+0.55f, 10},
//                {+0.80f, 20},
//                {+1.00f, 35},
//            }
//        );
//        f32 mountain_noise = mountain_noisegen.sample(wx,wz);
//        mount_dy = remap_curve(
//            mountain_noise,
//            {
//                {+0.00f, 0},
//                {+0.50f, 10},
//                {+0.75f, 45},
//                {+1.00f, 90},
//            }
//        );
//
//        // scale the mountains down dramatically on the shoreline
//        // reduce mountain by the square of the water depth?
//
//        i32 height = continentalness + hill_dy + mount_dy;
//        heightmap[cx,cz] = height; 
//       // noise_export[cx,cz] = cont_noise;
//    });
//};

static GenResult generate_chunk(GenJob job){
    GenResult res{
        .chunkCoord=job.chunkCoord,
        .chunkBlocks = {},
        .deferredWrites = {},
    };
    const auto& chunk_coord = res.chunkCoord;
    auto& block_store = res.chunkBlocks;
    auto& pendingWrites = res.deferredWrites;

    const GenConfig& cfg = job.cfg;



    constexpr glm::ivec2 chunk_local_min = {0,0};
    constexpr glm::ivec2 chunk_local_max = {Chunk::Extents.x, Chunk::Extents.z};

    const auto world_block_origin = toWorldOrigin(job.chunkCoord);
    const auto& world_block_lo = world_block_origin;
    const auto& world_block_hi = toWorldOrigin(job.chunkCoord)+BlockOffset{Chunk::Extents};

    auto try_place_block = [world_block_lo, world_block_hi, &block_store, &pendingWrites, chunk_coord] (
        OverwritePolicy policy, 
        WorldBlockPos wpos, 
        BlockType bt
    ){
        bool writeIsWithinChunkBounds = LM::isVecInBounds(wpos, world_block_lo, world_block_hi);
        if (writeIsWithinChunkBounds){
            // do it immediately
            i32 cx = wpos.x - world_block_lo.x;
            i32 cy = wpos.y - world_block_lo.y;
            i32 cz = wpos.z - world_block_lo.z;
            if (canMakeWrite(policy, block_store.at(cx,cy,cz))){
                block_store.set(cx,cy,cz, bt);
            }
        }else{
            pendingWrites.emplace_back(policy, chunk_coord, wpos, bt);
        }
    };

    GenContext ctx{world_block_origin,cfg};
    auto noise_map = generate_chunk_terrain_noise(cfg,world_block_origin);
    auto biome_map = classify_chunk_biomes(noise_map);

#ifdef CHUNK_NOISE_DEBUG
    std::ranges::copy(noise_map, res.noise.begin());
#endif


    ForEachInRangeEx(chunk_local_min,chunk_local_max,[&](i32 cx, i32 cz){
        auto biome = biome_map[cx,cz];
        auto palette = biome_palette[biome];
        auto block = palette.topsoil;

        block_store.set(cx,1,cz,block);
//        for (i32 cy = WORLD_YMIN; cy<WORLD_YMAX; cy++){
//            auto block = BlockType::AIR;
//            if (cy <= terrain_height){ 
//                block = default_plains_palette.crust;
//            }
//            if ( cy > terrain_height && cy <= cfg.sea_level){
//                block = default_plains_palette.water;
//            }
//            block_store.set(cx,cy,cz, block);
//        }
    });
        
//    // 4. TREES:
//    {
//        auto place_tree  = [chunk_coord, try_place_blocks, try_place_block](i32 cx,i32 cy, i32 cz){
//            auto wpos = toWorldBlockPos(chunk_coord, {cx,cy,cz});
//            f32 log_height01 = wpos_seeded_rand01(wpos, RandOffset::TreeHeight);
//            static constexpr i32 log_height_min = 5;
//            static constexpr i32 log_height_max = 8;
//            i32 log_height = std::round(std::lerp(log_height_min,log_height_max,log_height01));
//            try_place_blocks(
//                OverwritePolicy::OnlyAir, 
//                wpos, 
//                wpos + BlockOffset{1,log_height+1,1}, 
//                palette.wood_log
//            );
//            std::vector<BlockOffset> leaf_offsets;
//
//            static constexpr auto TREE_DECAY_CHANCE = 0.75f;
//            // y= LH -2,LH-1 get a full, 5x5 square. 
//            // the corners have a 25% chance of spawning
//            for (auto y = -2; y<=-1; y++){
//                for (auto x = -2; x<=2; x++){
//                    for (auto z = -2; z<=2; z++){
//                        bool corner = std::abs(x) + std::abs(z) == 4;
//                        bool should_place = true;
//                        if (corner){
//                            auto corner_wpos = wpos + BlockOffset{x,log_height+y,z};
//                            f32 roll = wpos_seeded_rand01(corner_wpos, RandOffset::TreeDecay);
//
//                            should_place = (roll > TREE_DECAY_CHANCE);
//                        }
//                        if (should_place){
//                            leaf_offsets.emplace_back(x,log_height+y,z);
//                        }
//                    }
//                }
//            }
//            for (auto y = 0; y<=1; y++){
//                for (auto x = -1; x<=1; x++){
//                    for (auto z = -1; z<=1; z++){
//                        bool corner = std::abs(x) + std::abs(z) == 2;
//                        bool should_place = true;
//                        if (corner){
//                            auto corner_wpos = wpos + BlockOffset{x,log_height+y,z};
//                            f32 roll = wpos_seeded_rand01(corner_wpos, RandOffset::TreeDecay);
//
//                            should_place = (roll > TREE_DECAY_CHANCE);
//                        }
//                        if (should_place){
//                            leaf_offsets.emplace_back(x,log_height+y,z);
//                        }
//                    }
//                }
//            }
//            for (const auto& offset: leaf_offsets){
//                try_place_block(
//                    OverwritePolicy::OnlyAir,
//                    wpos + offset,
//                    palette.leaves
//                );
//            }
//        };
//        const glm::ivec2 chunk_local_min = {0,0};
//        const glm::ivec2 chunk_local_max = ivec2{Chunk::Extents.x, Chunk::Extents.z};
//        ForEachInRangeEx(chunk_local_min, chunk_local_max,[&](i32 cx, i32 cz){
//            const auto [wx, wz] = to_world(cx,cz);
//            f32 density = tree_density.sample(wx,wz);
//            // 1. find the max roll in a 3x3 area to prevent touching trees
//            std::array<f32, 3*3> rolls;
//            f32 max_adjacent_roll = 0.0f;
//            // 
//            for (i32 x = -1; x<=1; x++){
//                for (i32 z = -1; z<=1; z++){
//                    f32 roll = wpos_seeded_rand01(wx+x,wz+z,RandOffset::TreeJitter);
//                    max_adjacent_roll = std::max(max_adjacent_roll, roll);
//                }
//            }
//            f32 roll = wpos_seeded_rand01(wx,wz,RandOffset::TreeJitter);
//            if (roll < max_adjacent_roll) return;
//            if (roll < density * gen_cfg.tree_place_threshold){
//                //log_to_chunk(chunk_coord,"{} -> density:{}, roll:{}", WorldChunkCoord{wx,wz},density,roll);
//                i32 cy = heightmap.at_chunk(cx,cz);
//                if (cy <= gen_cfg.SEA_LEVEL) return; // skip underwater trees
//                place_tree(cx,cy,cz);
//            }
//        });
//        // paint grass
//        ForEachInRangeEx(chunk_local_min, chunk_local_max,[&](i32 cx, i32 cz){
//            const auto [wx, wz] = to_world(cx,cz);
//            f32 density = grass_height.sample(wx,wz);
//            f32 reject_grass_roll = wpos_seeded_rand01(wx,wz,RandOffset::GrassGrowth);
//            if (reject_grass_roll < gen_cfg.reject_grass_chance){
//                return;
//            }
//
//            f32 trim_grass = wpos_seeded_rand01(wx,wz,RandOffset::GrassGrowth);
//            density -= trim_grass*0.2f;
//            i32 wy = heightmap.at_chunk(cx,cz);
//            auto wpos = WorldBlockPos{wx,wy+1,wz};
//            auto base = -0.5f;
//            if (wy <= gen_cfg.SEA_LEVEL) return; // skip underwater grass
//            if(density > base+0.65){ 
//                try_place_block(
//                    OverwritePolicy::OnlyAir,
//                    wpos,
//                    palette.grass[3]
//                );
//            } else if(density > base+0.15){ 
//                try_place_block(
//                    OverwritePolicy::OnlyAir,
//                    wpos,
//                    palette.grass[2]
//                );
//            } else if(density > base+0.10){ 
//                try_place_block(
//                    OverwritePolicy::OnlyAir,
//                    wpos,
//                    palette.grass[1]
//                );
//            }else if(density > base){ 
//                try_place_block(
//                    OverwritePolicy::OnlyAir,
//                    wpos,
//                    palette.grass[0]
//                );
//            }
//            // 1. find the max roll in a 3x3 area to prevent touching trees
//        });
//    }

    return res;
    // 4. Paint ores in stone blocks below certain point
    // 5. Create trees.
}

void ChunkGenerator::genChunks(std::stop_token stopToken, 
                      Queue<GenJob>& input_queue, Queue<GenResult>& output_queue){

    tracy::SetThreadName("chunk mesher");
    while (!stopToken.stop_requested()){
        GenJob job = input_queue.wait_dequeue();
        GenResult res = generate_chunk(job);
        output_queue.wait_enqueue(res);
    }
    
}

