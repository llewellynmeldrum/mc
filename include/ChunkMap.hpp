#include "Chunk.hpp"
#include "Geometry.hpp"
#include "ChunkGenerator.hpp"

#include <memory>

enum struct ChunkStatus{
    DIRTY,
    CURRENTLY_MESHING,
    CLEAN,
};
// should the mesh queue be a prio queue?
struct ChunkMap {
    ChunkMap() = default;
    ~ChunkMap() = default;
    ChunkMap(ChunkMap const&) = delete;
    ChunkMap& operator=(ChunkMap const&) = delete;

    ChunkMap(ChunkMap&&) = default;
    ChunkMap& operator=(ChunkMap&&) = default;

    // chunk map
    std::unordered_map<ivec3, std::unique_ptr<Chunk>> data;

    // chunk state map
    std::unordered_map<ivec3, ChunkStatus> chunkState;

    // chunk metadata map
    std::unordered_map<ivec3, std::unique_ptr<ChunkMetadata>> metadata;
    const ChunkMetadata* getMetadata(ivec3 pos)const ;

    // chunk neighbour map
    std::unordered_map<ivec3, std::array<const Chunk*, NUM_NEIGHBOURS>> neighbours;
    std::array<const Chunk*, NUM_NEIGHBOURS> getSurroundingChunks(ivec3 pos) const;

    // whether or not a chunk needs to be remeshed
    std::unordered_map<ivec3, bool> is_dirty;
    bool isDirty(ivec3 pos) const;
    bool isClean(ivec3 pos) const;
    bool isMeshing(ivec3 pos) const;
    void markDirty(ivec3 pos);
    void markClean(ivec3 pos);
    void markMeshing(ivec3 pos);

    // world space bounding boxes for chunks (used in frustum culling)
    std::unordered_map<ivec3, std::unique_ptr<AABB>> boundingBoxes;
    const AABB*   getBoundingBox(ivec3 chunk_offset) const;

    ChunkGenerator generator;
    
    void           generate(ivec3 pos);
    ChunkStatus getChunkState(ivec3 chunk_offset) const;
    const Chunk*   getNeighbourByOffset(ivec3 chunk_offset, ivec3 local_offset) const;
private:
    void           updateNeighbourMap(ivec3 pos);
    void           updateBoundingBoxesMap(ivec3 chunk_coord);


};
