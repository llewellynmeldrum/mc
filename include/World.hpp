#include "Chunk.hpp"
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

    ChunkMap chunkMap;
    GenConfig genConfig;

    static constexpr i64 NUM_VERTICAL_CHUNKS = 16;
    static ivec3         worldToChunkCoord(vec3 worldPos);
    static vec3          chunkToWorldPos(ivec3 chunkPos);

    inline std::vector<ChunkView> chunksInRadius(ivec3 origin, u32 radius) {
        std::vector<ChunkView> out;
        for (auto const& [coord, uptr]: chunkMap.chunks) {
            if (glm::distance((vec3)origin,(vec3)coord) < radius){
                out.emplace_back(coord, uptr.get());
            }
        }

        return out;
    }

    std::vector<std::pair<Block, Direction>> getNeighbourBlocks(vec3 world_pos) const;

    Chunk&       getMutableChunk(vec3 world_pos);
    const Chunk* getChunk(vec3 world_pos) const;

    Chunk&       ch_getMutableChunk(ivec3 chunk_pos);
    const Chunk* ch_getChunk(ivec3 chunk_pos) const;

    Block getBlock(ivec3 world_pos) const;
};
