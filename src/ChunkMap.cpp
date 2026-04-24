#include "ChunkMap.hpp"
#include "Logger.hpp"

const Chunk* ChunkMap::getData(ivec3 pos) {
    if (data.contains(pos)) {
        return &data[pos];
    } else {
        return nullptr;
    }
}
void ChunkMap::generate(ivec3 pos) {
    if (data.contains(pos)) {
        LOG_WARN("Chunk generation requested on chunk that already exists.");
    } else {
        data.insert({ pos, generator.gen(pos) });
    }
}

bool ChunkMap::isDirty(ivec3 pos) {
    auto it = is_dirty.find(pos);
    if (it != is_dirty.end()) {
        return (*it).second;
    }
    return false;  // no point in meshing a chunk if it does not exist
}

void ChunkMap::makeDirty(ivec3 pos) {
    is_dirty.insert_or_assign(pos, true);
}
void ChunkMap::makeClean(ivec3 pos) {
    is_dirty.insert_or_assign(pos, false);
}
