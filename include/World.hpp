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
    inline std::size_t distcount{0};
    inline f32 dist_sum{0};
struct World {
    World() = default;
    ~World() = default;
    World(World const&) = delete;
    World& operator=(World const&) = delete;
    World(World&&) = delete;
    World& operator=(World&&) = delete;

    inline void setupWorld(){
        chunkMap.setupChunkMap();
    }
    ChunkMap chunkMap;
    GenConfig genConfig;

    static constexpr i64 NUM_VERTICAL_CHUNKS = 16;

    // returns generated chunks within a radius from source, sorted in ascending order of distance to source
    inline std::vector<WorldChunkCoord> generatedChunkCoordsInRadius(WorldChunkCoord source, u32 radiusChunks, u32 maxChunks) {
        const size_t nChunks = std::pow(radiusChunks*2,3);
        std::vector<WorldChunkCoord> candidateList;
        candidateList.reserve(nChunks);

        glm::ivec3 lo(-radiusChunks);
        glm::ivec3 hi(radiusChunks);
        std::size_t count = 0;
        for (auto [x,y,z]: EachInRange(lo,hi)){
            glm::vec3 candidate = {x,y,z};
            if (!chunkMap.hasEntry(candidate)) continue;
            if (chunkMap.isMeshing(candidate)) continue;

            candidateList.emplace_back(candidate);
            if (++count >= maxChunks) break;
            // skip meshing entirely if chunk is not generated yet
        }

        auto sqdist = [](const auto& a, const auto& b) {
            auto sq = [](i32 v){ return v*v;};
            return sq(b.x-a.x) + sq(b.y-a.y) + sq(b.z-a.z);
        };

//        std::ranges::sort(candidateList, [sqdist,source](const auto& a, const auto& b){
//            // sort ascending 
//            return sqdist(source,a) < sqdist(source,b);
//        });
        return candidateList;
    }

    std::vector<std::pair<Block, Direction>> getNeighbourBlocks(WorldBlockPos world_pos) const;

    Block getBlock(WorldBlockPos world_pos) const;
};
