
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
#include "WorldGen_BiomeBlockPalettes.hpp"
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
    auto gen_heightmap (const GenConfig& cfg, ArrayList2D<NoiseParams> noise,ArrayList2D<BiomeID> biomes);
    // base the heightmap mostly off of noise but also biome slightly?
};
auto GenContext::gen_heightmap (const GenConfig& cfg, ArrayList2D<NoiseParams> noise,ArrayList2D<BiomeID> biomes){

    ArrayList2D<i32> heightmap(Chunk_Extents2);
    const glm::ivec2 chunk_local_min = {0,0};
    const glm::ivec2 chunk_local_max = glm::ivec2{Chunk::Extents.x, Chunk::Extents.z};
    ForEachInRangeEx(chunk_local_min,chunk_local_max,[&](i32 cx, i32 cz){
        using LM::unlerp;

        auto [wx,wz] = to_world(cx,cz);
        auto biome = biomes[cx,cz];
        i32 continentalness={}, hill_dy={}, mount_dy={};
        f32 cont_noise = noise[cx,cz].cont;
        continentalness = remap_curve(
            cont_noise,
            {
                {+0.00f, cfg.sea_level-80},
                {+0.40f, cfg.sea_level-6},
                {+0.48f, cfg.sea_level-1},
                {+0.60f, cfg.sea_level + 1},   // <- flat shelf spanning 0.48–0.60: the beach
                {+0.66f, cfg.sea_level + 4},
                {+0.80f, cfg.sea_level+10},
                {+1.00f, cfg.sea_level+15},
            }
        );
        f32 hill_noise = noise[cx,cz].hill;
        hill_dy = remap_curve(
            hill_noise,
            {
                {+0.00f, 0},
                {+0.40f, 5},
                {+0.55f, 10},
                {+0.80f, 20},
                {+1.00f, 25},
            }
        );
        f32 mountain_noise = noise[cx,cz].mountain;
        mount_dy = remap_curve(
            mountain_noise,
            {
                {+0.00f, 0},
                {+0.50f, 10},
                {+0.75f, 25},
                {+1.00f, 30},
            }
        );
        f32 landness = glm::smoothstep(0.45f, 0.55f, cont_noise);
        // mountains should only be able to exist at VERY HIGH landedness.
        hill_dy  = i32(hill_dy  * landness);
        mount_dy = i32(mount_dy * landness);


        i32 height = continentalness+ hill_dy + mount_dy;
        heightmap[cx,cz] = height; 
       // noise_export[cx,cz] = cont_noise;
    });
    return heightmap;
};

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


    GenContext ctx{world_block_origin,cfg};
    auto noise_map = generate_chunk_terrain_noise(cfg,world_block_origin);
    auto biome_map = classify_chunk_biomes(noise_map);
    auto height_map = ctx.gen_heightmap(cfg,noise_map,biome_map);
    auto block_writer = BlockWriter{block_store,pendingWrites,chunk_coord };

#ifdef CHUNK_NOISE_DEBUG
    std::ranges::copy(noise_map, res.noise.begin());
#endif


    ForEachInRangeEx(chunk_local_min,chunk_local_max,[&](i32 cx, i32 cz){
        auto terrain_height = height_map[cx,cz];
        auto dirt_y_start = terrain_height - 1;
        auto dirt_y_stop = terrain_height - 4;
        auto stone_height = terrain_height - 3;
        const auto& biome = biome_map[cx,cz];
        const auto& palette = biome_palettes[biome];
        for (i32 y = WORLD_YMAX-1; y>=WORLD_YMIN; y--){
            BlockType brush = BlockType::AIR;

            if (y > terrain_height){
                if ( y < cfg.sea_level){
                    brush = BlockType::WATER_BLOCK;
                }
            }else if (y == terrain_height) {
                brush = palette.topsoil;
            }else if (y < terrain_height){
                if (y <= dirt_y_start && y > dirt_y_stop){
                    brush = palette.soil;
                }else{
                    brush = palette.crust;
                }
            }else{
                continue;
            }
            block_store.set(cx,y,cz,brush);
        }
        const auto& features = biome_features[biome];
        const f32 density = noise_map[cx,cz].density0;

        auto origin = toWorldBlockPos(chunk_coord, ChunkBlockPos{cx,terrain_height+1,cz});
        if (features.should_place_tree(origin, features.tree, density, block_writer)){
            std::println("PLACING TREE {}", origin);
            features.tree.place(origin, density, block_writer);
        }
    });


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

