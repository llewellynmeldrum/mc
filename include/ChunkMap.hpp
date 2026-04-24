#include "Chunk.hpp"
#include "ChunkGenerator.hpp"
#include "Types.hpp"

using HASH = hashChunkCoord;

struct ChunkMap {
    HashMap<ivec3, Chunk, HASH> data;
    HashMap<ivec3, bool, HASH>  is_dirty;

    ChunkMap() = default;
    ~ChunkMap() = default;
    ChunkGenerator generator;
    void           generate(ivec3 pos);

    const Chunk* getData(ivec3 pos);
    bool         isDirty(ivec3 pos);
    void         makeDirty(ivec3 pos);
    void         makeClean(ivec3 pos);
};
