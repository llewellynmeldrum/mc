#include "Chunk.hpp"
#include "Geometry.hpp"
#include "ChunkGenerator.hpp"
#include "Types.hpp"

#include <memory>

struct ChunkMap {
    using HASH = hashChunkCoord;
    ChunkMap() = default;
    ~ChunkMap() = default;

    // chunk map
    HashMap<ivec3, std::unique_ptr<Chunk>, HASH> data;

    // chunk metadata map
    HashMap<ivec3, std::unique_ptr<ChunkMetadata>, HASH> metadata;

    // chunk neighbour map
    HashMap<ivec3, std::array<const Chunk*, NUM_NEIGHBOURS>, HASH> neighbours;

    // whether or not a chunk needs to be remeshed
    HashMap<ivec3, bool, HASH> is_dirty;

    // world space bounding boxes for chunks (used in frustum culling)
    HashMap<ivec3, std::unique_ptr<AABB>, HASH> boundingBoxes;

    const ChunkMetadata* getMetadata(ivec3 pos);
    const Chunk*   getNeighbourByOffset(ivec3 chunk_offset, ivec3 local_offset) const;
    ChunkGenerator generator;
    void           generate(ivec3 pos);
    void           updateNeighbourMap(ivec3 pos);
    void           updateBoundingBoxesMap(ivec3 chunk_coord);

    std::array<const Chunk*, NUM_NEIGHBOURS> getSurroundingChunks(ivec3 pos) const;

    bool isDirty(ivec3 pos) const;
    void makeDirty(ivec3 pos);
    void makeClean(ivec3 pos);
};
