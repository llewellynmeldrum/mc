
#include <print>
#include <queue>

#include "CoordTypes.hpp"
#include "CoordIteration.hpp"

#include "Block.hpp"
#include "ChunkConstants.hpp"
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

    ArrayList2D<i32> heightmap(ChunkInfo::Extents2D);
    ForEachInRangeEx({0,0},ChunkInfo::Extents2D,[&](i32 cx, i32 cz){
        using LM::unlerp;

        auto [wx,wz] = to_world(cx,cz);
        auto biome = biomes[cx,cz];
        f32 cont_noise = noise[cx,cz].cont;
        i32 base = remap_curve<i32>(
            cont_noise,
            {
                // unsure as to how large to make the coast.
                // I think it makes sense for the coast to extend INTO the ocean a little.
                {-1.00f,                    cfg.sea_level-70},
                {+COAST.min - .50f,         cfg.sea_level-40},
                {+COAST.min - .30f,         cfg.sea_level-20},
                {+COAST.min - .10f,         cfg.sea_level-6},
                { COAST.min,                cfg.sea_level-5},
                {COAST.mid()                ,cfg.sea_level-2},
                { COAST.max,                cfg.sea_level+3},
                {+0.66f,                    cfg.sea_level+6},
                {+0.80f,                    cfg.sea_level+10},
                {+1.00f,                    cfg.sea_level+15},
            }
        );
        f32 hill_weight = remap_curve<f32>(
            cont_noise,
            {
                {-1.00f,            0.00f},
                { COAST.min - .30f, 0.00f},
                { COAST.min - .10f, 0.00f},
                { COAST.min,        0.10f},
                { COAST.mid(),      0.30f},
                { COAST.max,        0.40f},
                { COAST.max + .10f, 0.70f},
                { COAST.max + .20f, 0.80f},
                { COAST.max + .30f, 0.90f},
                {+1.00f,            1.00f}, 
            }
        );
        f32 hill_noise = noise[cx,cz].hill;
        i32 hill_height = remap_curve<i32>(
            hill_noise,
            {
                {-1.00f,                    0},
                { COAST.mid(),              0},
                { COAST.mid() + 0.10f,      0},
                { COAST.mid() + 0.45f,      90},
                {+1.00f,                    95}, 
            }
        );

        f32 gradient_noise = noise[cx,cz].grad;
        // 1. work out the base noise via a spline remap
        // 2. work out the hill_weight via a spline remap of cont


        i32 height = 
            base 
            + hill_noise  * hill_height * hill_weight;
        ;
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
    constexpr glm::ivec2 chunk_local_max = {ChunkInfo::Extents3D.x, ChunkInfo::Extents3D.z};

    const auto world_block_origin = toWorldOrigin(job.chunkCoord);
    const auto& world_block_lo = world_block_origin;
    const auto& world_block_hi = toWorldOrigin(job.chunkCoord)+BlockOffset{ChunkInfo::Extents3D};


    GenContext ctx{world_block_origin,cfg};
    auto noise_map = generate_chunk_terrain_noise(cfg,world_block_origin);
    auto biome_map = classify_chunk_biomes(noise_map);
    auto height_map = ctx.gen_heightmap(cfg,noise_map,biome_map);
    auto block_writer = BlockWriter{block_store.view(),pendingWrites,chunk_coord };

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
            auto [wx, wz] = ctx.to_world(cx,cz);
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
            block_store.at(cx,y,cz)=brush;
        }
    });
    ForEachInRangeEx(chunk_local_min,chunk_local_max,[&](i32 cx, i32 cz){
        auto terrain_height = height_map[cx,cz];
        const auto& biome = biome_map[cx,cz];
        const auto& palette = biome_palettes[biome];
        const auto& features = biome_features[biome];


        constexpr static f32 tree_inland_bias_max = 1.6f; 
        f32 tree_inland_bias = remap_curve<f32>(
            noise_map[cx,cz].cont,
            {
                // cont     tree_inland_bias
                {-1.0f,     1.0f},
                {+0.25f,    1.0f},
                {+0.75f,    tree_inland_bias_max},
            }
        );
        const f32 tree_density = noise_map[cx,cz].rain;// * tree_inland_bias;
        const f32 grass_density = tree_density;
        const f32 multi_seg_density = tree_density;
        const f32 single_block_density = tree_density;

        auto origin = toWorldBlockPos(chunk_coord, ChunkBlockPos{cx,terrain_height+1,cz});

        if (features.tree.should_place(origin, tree_density, block_writer)){
            features.tree.place(origin, tree_density, block_writer);
        }
        if (features.grass.should_place(origin, grass_density, block_writer)){
            features.grass.place(origin, grass_density, block_writer);
        }
        if (features.multi_seg.should_place(origin, multi_seg_density, block_writer)){
            features.multi_seg.place(origin, multi_seg_density, block_writer);
        }
        for (const auto& feature: features.single_blocks){
            if (feature.should_place(origin, single_block_density, block_writer)){
                feature.place(origin, single_block_density, block_writer);
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
        GenResult res = generate_chunk(job);
        output_queue.wait_enqueue(res);
    }
    
}

