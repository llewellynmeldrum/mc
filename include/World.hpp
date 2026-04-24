#include "Chunk.hpp"
#include "ChunkMap.hpp"
#include "glmWrapper.hpp"
struct World {
    static ivec3 worldToChunkPos(const vec3& worldPos);
    static vec3  chunkToWorldPos(const ivec3& chunkPos);
    World() = default;
    ~World() = default;
    // make chunkMap itself use unique ptrs
    ChunkMap chunks;

    Chunk& getChunk(vec3 world_pos);

    inline std::vector<ChunkView> getDirtyChunksInRadius(ivec3 origin, u32 radius) {
        std::vector<ChunkView> res;
        for (const auto& [chunk_pos, chunk] : chunks.data) {
            if (chunks.isDirty(chunk_pos)) {
                res.emplace_back(chunk_pos, &chunk);
            }
        }
        return res;
    }

    Chunk& getChunk(ivec3 chunk_pos);

    inline void generateChunk(const ivec3& chunk_pos) {
        chunks.generate(chunk_pos);
        chunks.makeDirty(chunk_pos);
    }
};
