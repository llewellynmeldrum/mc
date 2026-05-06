#include "Chunk.hpp"
#include "ChunkMap.hpp"
#include "CommonUtils.hpp"
#include "glmWrapper.hpp"
#include <memory>

struct World {
    static constexpr i64 NUM_VERTICAL_CHUNKS = 16;
    static ivec3         worldToChunkCoord(vec3 worldPos);
    static vec3          chunkToWorldPos(ivec3 chunkPos);
    World() = default;
    ~World() = default;
    // make chunkMap itself use unique ptrs
    ChunkMap chunkMap;

    inline std::vector<ChunkView> filterChunksWithinFrustum(std::span<ChunkView> input_chunks,
                                                            const Frustum&       frustum) {
        std::vector<ChunkView> res;
        for (const auto& [chunk_pos, chunk] : chunkMap.data) {
            if (frustum.isAABBInside(*chunkMap.boundingBoxes[chunk_pos])) {
                res.emplace_back(chunk_pos, chunk.get());
            }
        }
        return res;
    }
    inline std::vector<ChunkView> filterChunksOutsideFrustum(std::span<ChunkView> input_chunks,
                                                             const Frustum&       frustum) {
        std::vector<ChunkView> res;
        for (const auto& [chunk_pos, chunk] : chunkMap.data) {
            if (!frustum.isAABBInside(*chunkMap.boundingBoxes[chunk_pos])) {
                res.emplace_back(chunk_pos, chunk.get());
            }
        }
        return res;
    }
    inline std::vector<ChunkView> getDirtyChunksInRadius(ivec3 origin, u32 radius) {
        std::vector<ChunkView> res;
        for (const auto& [chunk_pos, chunk] : chunkMap.data) {
            if (chunkMap.isDirty(chunk_pos)) {
                res.emplace_back(chunk_pos, chunk.get());
            }
        }
        return res;
    }

    std::vector<std::pair<Block, Direction>> getNeighbourBlocks(vec3 world_pos) const;

    Chunk&       getMutableChunk(vec3 world_pos);
    const Chunk* getChunk(vec3 world_pos) const;

    Chunk&       ch_getMutableChunk(ivec3 chunk_pos);
    const Chunk* ch_getChunk(ivec3 chunk_pos) const;

    Block getBlock(ivec3 world_pos) const;

    inline void generateChunk(const ivec3& chunk_pos) {
        chunkMap.generate(chunk_pos);
        chunkMap.makeDirty(chunk_pos);
    }
};
