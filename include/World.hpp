#pragma once 
#include <algorithm>
#include <memory>
#include <print>

#include "Camera.hpp"
#include "Chunk.hpp"
#include "ChunkConcurrency.hpp"
#include "ChunkMap.hpp"
#include "CommonUtils.hpp"
#include "CoordTypes.hpp"
#include "GlobalDebugLog.hpp"
#include "glmWrapper.hpp"
#include "WorldGen_NoiseGeneration.hpp"
#include "cppslop.hpp"

#include "glm_math_extensions.hpp"

#include "WorldGen_Defaults.hpp"

void generate_chunks(std::stop_token stopToken, Queue<GenJob>& input_queue, Queue<GenResult>& output_queue);
void mesh_chunks (std::stop_token stopToken, Queue<MeshJob>& in_queue, Queue<MeshResult>& out_queue);

struct World {
    #define CLASS_NAME World
    World(i32 _world_seed):
        world_seed(_world_seed),
        active_cfg(noise_config_defaults(),remap_table_defaults()),
        editable_cfg(noise_config_defaults(),remap_table_defaults())
    {}
    ~World() = default;
    MOVE_CTOR(delete);
    COPY_CTOR(delete);
    MOVE_ASSN(delete);
    COPY_ASSN(delete);

    inline void setup(){
        generators.launch_threads(generate_chunks);
    }

    inline void regenerate(){
        chunkMap.clear();
        worldgen_epoch++; // all new genjobs will have targetRevision incremented 
        LOG_DEBUG("{}->{}",active_cfg.cont_cfg.seed_offset, editable_cfg.world_seed);
        GenConfig::copy(active_cfg,editable_cfg);
    }
    // stuff
    inline void tick(){
        tick_count++;
    }
    i32 tick_count;

    // Mutable state which gets fed to gen workers
    i32 world_seed;
    GenConfig active_cfg;
    GenConfig editable_cfg;
    i32 worldgen_epoch {0}; // aka. global target_gen_revision 

    // Owner of the gen workers
    JobProcessor<GenJob, GenResult> generators;

    // Chunk data store
    ChunkMap chunkMap;

    ChunkEntry* make_chunk_entry(WorldChunkCoord key);


    std::vector<std::pair<bool, WorldChunkCoord>> chunksStatesInRadius(WorldChunkCoord chunkCoord, i32 dist);

    std::vector<std::pair<Block, Direction>> getNeighbourBlocks(WorldBlockPos world_pos) const;

    #undef CLASS_NAME
};
