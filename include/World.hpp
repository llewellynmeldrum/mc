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
#include "glmWrapper.hpp"
#include "WorldGen_NoiseGeneration.hpp"
#include "cppslop.hpp"

struct World {
    World(i32 _world_seed):
        world_seed(_world_seed),
        noise_gen(world_seed),
        genConfig(noise_gen)
    {}
    ~World() = default;
    NO_MOVE(World);
    NO_COPY(World);

    inline void setup(){
        generators.launchGenThreads();
    }

    inline void regenerate(){
        chunkMap.clear();
        worldgen_epoch++; // all new genjobs will have targetRevision incremented 
    }

    // Mutable state which gets fed to gen workers
    i32 world_seed;
    NoiseGenerator noise_gen{default_world_seed};
    GenConfig genConfig;

    i32 worldgen_epoch {0}; // aka. global target_gen_revision 

    // Owner of the gen workers
    ChunkGenerator generators;

    // Chunk data store
    ChunkMap chunkMap;


    void set_block(WorldBlockPos wpos, BlockType block);

    ChunkEntry* make_chunk_entry(WorldChunkCoord key);


    inline std::vector<std::pair<bool, WorldChunkCoord>> chunksStatesInRadius(WorldChunkCoord chunkCoord, i32 dist) {
        const size_t nChunksInRadius = std::pow(2*dist+1,2);
        std::vector<std::pair<bool,WorldChunkCoord>> candidates;
        candidates.reserve(nChunksInRadius);

        auto add = [this, &candidates](i32 x, i32 z){
            const auto key = WorldChunkCoord{x,z}; // dont you have to 
            const auto state = chunkMap.entries.try_get(key);
            candidates.emplace_back(static_cast<bool>(state),key);
        };


        i32 x{chunkCoord.x}, z{chunkCoord.z};
        add(x,z); // center point
        for (i32 r = 1; r<= dist; r++){
            const i32 r2 = 2*r;
            add(--x,z); // move out of the centre point
            for (int j = 0; j<r2 - 1;j++)    add(x,++z); // traverse the remaining (-X) edge
            for (int j = 0; j<r2 ; j++)     add(++x,z);  // traverse the whole     (Z+) edge
            for (int j = 0; j<r2 ; j++)     add(x,--z);  // traverse the whole     (+X) edge
            for (int j = 0; j<r2 ; j++)     add(--x,z);  // traverse the whole     (+X) edge
        }
        return candidates;
    }


    std::vector<std::pair<Block, Direction>> getNeighbourBlocks(WorldBlockPos world_pos) const;

};
