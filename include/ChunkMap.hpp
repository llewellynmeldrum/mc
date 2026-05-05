#include "Chunk.hpp"
#include "ChunkGenerator.hpp"
#include "Types.hpp"

constexpr const size_t NUM_NEIGHBOURS = 6;  // up, down, left, right, front, back (3d chunks)

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
    HashMap<ivec3, bool, HASH>                                     is_dirty;

    const Chunk*   getNeighbourByOffset(ivec3 chunk_offset, ivec3 local_offset) const;
    ChunkGenerator generator;
    void           generate(ivec3 pos);
    void           updateNeighbourMap(ivec3 pos);

    std::array<const Chunk*, NUM_NEIGHBOURS> getSurroundingChunks(ivec3 pos) const;

    bool isDirty(ivec3 pos) const;
    void makeDirty(ivec3 pos);
    void makeClean(ivec3 pos);
};
