#pragma once 
#include "Chunk.hpp"
#include "ChunkConcurrency.hpp"
#include "ChunkMap.hpp"
#include "CommonUtils.hpp"
#include "CoordTypes.hpp"
#include "glmWrapper.hpp"
#include <algorithm>
#include <memory>
#include <print>

#include "Camera.hpp"

struct World {
    World() = default;
    ~World() = default;
    World(World const&) = delete;
    World& operator=(World const&) = delete;
    World(World&&) = delete;
    World& operator=(World&&) = delete;

    ChunkMap chunkMap;
    GenConfig genConfig;
    inline void setup(){
        chunkMap.launchGenerator();
    }
    ChunkEntry* make_chunk_entry(WorldChunkCoord key);
    decltype(auto) try_get_chunk_entry(WorldChunkCoord key){
        return chunkMap.entries.try_get(key);
    }


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

    Block getBlock(WorldBlockPos world_pos) const;
};
