#include <print>
#include <queue>

#include "FastNoiseLite.h"

#include "CoordTypes.hpp"
#include "CoordIteration.hpp"
#include "Block.hpp"
#include "ChunkConstants.hpp"
#include "ChunkHelpers.hpp"
#include "DebugFormat.hpp"
#include "FormatSpecs.hpp"
#include "Assertion.hpp"
#include "LM.hpp"
#include "PendingBlockWrites.hpp"
#include "World.hpp"
#include "Logger.hpp"
#include "Types.h"
#include "Array2D.hpp"
#include "ChunkNoiseDebug.hpp"
#include "glm_math_extensions.hpp"
#include "ThreadTracker.hpp"

#include "Biomes.hpp"
#include "WorldGen_BiomeClassification.hpp"
#include "WorldGen_NoiseGeneration.hpp"
#include "WorldGen_BiomeBlockPalettes.hpp"
#include "WorldGen_Config.hpp"
#include "WorldGen_Utils.hpp"

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
        f32 hill_noise = noise[cx,cz].hill;

        i32 base = cfg.sea_level + cfg.cont_noise_to_base.remap<i32>(cont_noise);
        f32 hill_weight = cfg.cont_noise_to_hill_weight.remap<f32>(cont_noise);
        i32 hill_height = cfg.hill_noise_to_hill_height.remap<i32>( hill_noise);


        i32 height = 
            base 
            + (hill_noise  * hill_height * hill_weight);
        ;
        heightmap[cx,cz] = height; 
    });
    return heightmap;
};

static GenResult generate_chunk(GenJob job){
    GenResult res{
        .genRevisionID = job.genRevisionID,
        .chunkCoord=job.chunkCoord,
        .chunkBlocks = {},
        .deferredWrites = {}
    };
    const auto& chunk_coord = res.chunkCoord;
    auto& block_store = res.chunkBlocks;
    auto& pendingWrites = res.deferredWrites;

    const GenConfig& cfg = job.cfg;



    constexpr static f32 tree_inland_bias_max = 1.6f; 
    RemapTable cont_to_tree_inland_bias{
                // cont     tree_inland_bias
                {-1.0f,     1.0f},
                {+0.25f,    1.0f},
                {+0.75f,    tree_inland_bias_max},
    };


    const auto world_block_origin = toWorldOrigin(job.chunkCoord);
    const auto& world_block_lo = world_block_origin;
    const auto& world_block_hi = toWorldOrigin(job.chunkCoord)+BlockOffset{ChunkInfo::Extents3D};


    GenContext ctx{world_block_origin,cfg};
    auto noise_map = generate_chunk_terrain_noise(cfg,world_block_origin);
    auto biome_map = classify_chunk_biomes(noise_map);
    auto terrain_height_map = ctx.gen_heightmap(cfg,noise_map,biome_map);
    auto block_writer = BlockWriter{block_store.view(),pendingWrites,chunk_coord };

#ifdef CHUNK_NOISE_DEBUG
    std::ranges::copy(noise_map, res.noise.begin());
#endif


    for_each_xz_in_chunk([&](i32 cx, i32 cz){
        auto terrain_height = terrain_height_map[cx,cz];
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
    for_each_xz_in_chunk([&](i32 cx, i32 cz){
        auto terrain_height = terrain_height_map[cx,cz];
        auto stone_height = terrain_height - 3;
        const auto& biome = biome_map[cx,cz];
        if (biome == BiomeID::IceBeach || biome == BiomeID::FrozenOcean){
            const auto& palette = biome_palettes[biome];
            i32 y = cfg.sea_level-1;
            auto [wx, wz] = ctx.to_world(cx,cz);
            BlockType brush = BlockType::AIR;
            if (block_store.at(cx,y,cz) == BlockType::WATER_BLOCK){
                block_store.at(cx,y,cz) = palette.ice;
            }
        }
    });
    for_each_xz_in_chunk([&](i32 cx, i32 cz){
        auto terrain_height = terrain_height_map[cx,cz];
        const auto& biome = biome_map[cx,cz];
        const auto& palette = biome_palettes[biome];
        const auto& features = biome_features[biome];

        f32 tree_inland_bias = cont_to_tree_inland_bias.remap<f32>(noise_map[cx,cz].cont);
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
    for_each_xz_in_chunk([&](i32 cx, i32 cz){
        auto terrain_height = terrain_height_map[cx,cz];
        const auto& biome = biome_map[cx,cz];
        const f32 snow_density = noise_map[cx,cz].heat;
        const auto& features = biome_features[biome];
        for (i32 y = WORLD_YMAX-2; y>=terrain_height; y--){
            if (block_store.at(cx,y,cz) != BlockType::AIR){
                auto snow_origin = toWorldBlockPos(chunk_coord, ChunkBlockPos{cx,y+1,cz});
                if (features.snow.should_place(snow_origin, snow_density, block_writer)){
                    features.snow.place(snow_origin, snow_density, block_writer);
                }
                break;
            }
        }
    });


    return res;
    // 4. Paint ores in stone blocks below certain point
    // 5. Create trees.
}

void ChunkGenerator::genChunks(std::stop_token stopToken, 
                      Queue<GenJob>& input_queue, Queue<GenResult>& output_queue){

    ThreadTracker::assign_my_thread_type(ThreadType::gen);
    while (!stopToken.stop_requested()){
        GenJob job = input_queue.wait_dequeue();
        job.bench.job_idle.bench_end(job.chunkCoord,job.genRevisionID);
        job.bench.work.bench_start(job.chunkCoord,job.genRevisionID);

        GenResult res = generate_chunk(job);

        job.bench.work.bench_end(job.chunkCoord,job.genRevisionID);
        job.bench.res_idle.bench_start(job.chunkCoord,job.genRevisionID);
        output_queue.wait_enqueue(res);
    }
    
}

