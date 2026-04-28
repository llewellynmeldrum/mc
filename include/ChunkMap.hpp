#include "Chunk.hpp"
#include "ChunkGenerator.hpp"
#include "Types.hpp"

using HASH = hashChunkCoord;

constexpr const size_t NUM_NEIGHBOURS = 6;  // up, down, left, right, front, back (3d chunks)
struct ChunkMap {
    HashMap<ivec3, Chunk, HASH>                                    data;
    HashMap<ivec3, std::array<const Chunk*, NUM_NEIGHBOURS>, HASH> neighbours;
    HashMap<ivec3, bool, HASH>                                     is_dirty;

    ChunkMap() = default;
    ~ChunkMap() = default;
    ChunkGenerator generator;
    void           generate(ivec3 pos);
    void           assignNeighbours(ivec3 pos);

    const Chunk* getData(ivec3 pos);

    std::span<const Chunk* const, NUM_NEIGHBOURS> getNeighbours(ivec3 pos) const;

    bool isDirty(ivec3 pos);
    void makeDirty(ivec3 pos);
    void makeClean(ivec3 pos);
};
