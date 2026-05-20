#include "Chunk.hpp"
#include "ChunkConcurrency.hpp"
#include "ChunkMap.hpp"
#include "CommonUtils.hpp"
#include "glmWrapper.hpp"
#include <memory>

#include "Camera.hpp"
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
    [[deprecated]]    static glm::ivec3         worldToChunkCoord(glm::vec3 worldPos);
    [[deprecated]]    static glm::vec3          chunkToWorldPos(glm::ivec3 chunkPos);

    inline std::vector<ChunkView> chunksInRadius(WorldBlockPos chunkCoord, u32 radiusChunks) {
        std::vector<ChunkView> out;
        for (auto const& [coord, uptr]: chunkMap.chunks) {
            if (glm::distance((glm::vec3)chunkCoord,(glm::vec3)coord) < radiusChunks){
                out.emplace_back(coord, uptr.get());
            }
        }

        return out;
    }

    std::vector<std::pair<Block, Direction>> getNeighbourBlocks(WorldBlockPos world_pos) const;

    Block getBlock(WorldBlockPos world_pos) const;
};
