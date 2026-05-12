#include "Chunk.hpp"
#include "Geometry.hpp"
#include "ChunkGenerator.hpp"

#include <memory>

enum struct ChunkStatus{
    NOT_GENERATED,           // not generated yet
    ENQUEUED_FOR_MESHING,   // i.e do not add it to the queue (unless a sooner mesh has appeared?)
    MESHED_CLEAN,             // i.e no need to mesh
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

    // chunk metadata map
    std::unordered_map<ivec3, std::unique_ptr<ChunkMetadata>> metadata;
    const ChunkMetadata* getMetadata(ivec3 pos)const ;

    // chunk neighbour map
    std::unordered_map<ivec3, std::array<const Chunk*, NUM_NEIGHBOURS>> neighbours;
    std::array<const Chunk*, NUM_NEIGHBOURS> getSurroundingChunks(ivec3 pos) const;

    // whether or not a chunk needs to be remeshed
    std::unordered_map<ivec3, bool> is_dirty;
    bool isDirty(ivec3 pos) const;
    void makeDirty(ivec3 pos);
    void makeClean(ivec3 pos);

    // world space bounding boxes for chunks (used in frustum culling)
    std::unordered_map<ivec3, std::unique_ptr<AABB>> boundingBoxes;
    const AABB*   getBoundingBox(ivec3 chunk_offset) const;

    ChunkGenerator generator;
    
    void           generate(ivec3 pos);
    const Chunk*   getNeighbourByOffset(ivec3 chunk_offset, ivec3 local_offset) const;
private:
    void           updateNeighbourMap(ivec3 pos);
    void           updateBoundingBoxesMap(ivec3 chunk_coord);


};
