#include "Chunk.hpp"
#include "ChunkMap.hpp"
#include "CommonUtils.hpp"
#include "glmWrapper.hpp"
struct World {
    static ivec3 worldToChunkCoord(vec3 worldPos);
    static vec3  chunkToWorldPos(ivec3 chunkPos);
    World() = default;
    ~World() = default;
    // make chunkMap itself use unique ptrs
    ChunkMap chunks;

    inline std::vector<ChunkView>
    getDirtyChunksInRadius(ivec3 origin, u32 radius) {
        std::vector<ChunkView> res;
        for (const auto& [chunk_pos, chunk] : chunks.data) {
            if (chunks.isDirty(chunk_pos)) {
                res.emplace_back(chunk_pos, &chunk);
            }
        }
        return res;
    }

    std::vector<std::pair<Block, Direction>> getNeighbourBlocks(vec3 world_pos) const;

    Chunk&       getMutableChunk(vec3 world_pos);
    const Chunk* getChunk(vec3 world_pos) const;

    Chunk&       getMutableChunk(ivec3 chunk_pos);
    const Chunk* getChunk(ivec3 chunk_pos) const;

    Block getBlock(vec3 world_pos) const;

    inline void
    generateChunk(const ivec3& chunk_pos) {
        chunks.generate(chunk_pos);
        chunks.makeDirty(chunk_pos);
    }
};
