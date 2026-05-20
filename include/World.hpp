#include "Chunk.hpp"
#include "ChunkConcurrency.hpp"
#include "ChunkMap.hpp"
#include "CommonUtils.hpp"
#include "glmWrapper.hpp"
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

    inline std::vector<ChunkView> chunksInRadius(WorldBlockPos chunkCoord, u32 radiusChunks) {
        std::vector<ChunkView> out;
        for (auto const& [coord, uptr]: chunkMap.chunks) {
            const auto dist = glm::distance((glm::vec3)chunkCoord,(glm::vec3)coord);
            dist_sum+=dist;
            distcount++;
            if (dist <= radiusChunks){
                out.emplace_back(coord, uptr.get());
            }
        }
        std::println("Chunks in radius:{}",out.size());

        if (distcount % 100 == 0){
            std::println("Dist avg:{}",dist_sum/distcount);
        }
        return out;
    }

    std::vector<std::pair<Block, Direction>> getNeighbourBlocks(WorldBlockPos world_pos) const;

    Block getBlock(WorldBlockPos world_pos) const;
};
