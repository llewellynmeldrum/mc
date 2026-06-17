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



    inline std::vector<std::pair<bool, WorldChunkCoord>> chunksStatesInRadius(WorldChunkCoord chunkCoord, i32 dist) {
        const size_t nChunksInRadius = std::pow(2*dist+1,3);
        std::vector<std::pair<bool,WorldChunkCoord>> candidates;
        candidates.reserve(nChunksInRadius);

        auto add = [this, &candidates](i32 x, i32 y, i32 z){
            const auto key = WorldChunkCoord{x,y,z}; // dont you have to 
            const auto state = chunkMap.states.try_get(key);
            candidates.emplace_back(static_cast<bool>(state),key);
        };

        const i32& oy = chunkCoord.y;

        i32 minY = oy-dist;
        i32 maxY = oy+dist;
        for (i32 y = maxY; y>=minY; y--){
            i32 x{chunkCoord.x}, z{chunkCoord.z};
            add(x,y,z); // center point
            for (i32 r = 1; r<= dist; r++){
                const i32 r2 = 2*r;
                add(--x,y,z); // move out of the centre point
                for (int j = 0; j<r2 - 1;j++)    add(x,y,++z); // traverse the remaining (-X) edge
                for (int j = 0; j<r2 ; j++)     add(++x,y,z);  // traverse the whole     (Z+) edge
                for (int j = 0; j<r2 ; j++)     add(x,y,--z);  // traverse the whole     (+X) edge
                for (int j = 0; j<r2 ; j++)     add(--x,y,z);  // traverse the whole     (+X) edge
            }
        }
        return candidates;
    }

    inline std::vector<WorldChunkCoord> 
    meshReadyChunksInRad(WorldChunkCoord chunkCoord, glm::ivec3 extents, i32 maxChunks=0) {
        std::vector<WorldChunkCoord> candidates;

        auto add = [this, &candidates](i32 x, i32 y, i32 z){
            const auto key = WorldChunkCoord{x,y,z}; // dont you have to 
            bool added = chunkMap.states.if_contains_else(
                key,
                [&](ChunkState& state){
                    if (state.ready_for_mesh()){
                        candidates.emplace_back(key);
                        return true;
                    }
                    return false;
                },
                [&](){
                    return false;
                }
            );
            return added;
        };
        SpiralIterateRange(maxChunks, chunkCoord,extents.y, extents.x, add);
        return candidates;
    }

    std::vector<std::pair<Block, Direction>> getNeighbourBlocks(WorldBlockPos world_pos) const;

    Block getBlock(WorldBlockPos world_pos) const;
};
