
#include "ChunkHelpers.hpp"
#include "DebugFormatSpecializations.hpp"
#include "ChunkMap.hpp"
#include "CommonUtils.hpp"
#include "Logger.hpp"
#include <memory>
#include <ranges>

using namespace glm;
std::array<const Chunk*, NUM_NEIGHBOURS> ChunkMap::getSurroundingChunks(ivec3 pos) const {
    std::array<const Chunk*, NUM_NEIGHBOURS> res{ neighbours.at(pos) };
    return res;
}
std::array<ChunkStore, NUM_NEIGHBOURS> ChunkMap::copySurroundingChunks(ivec3 pos) const {
    std::array<ChunkStore,NUM_NEIGHBOURS>res;
    for (const auto [i,val]: std::views::enumerate(neighbours.at(pos))){
        res[i] = ChunkStore(*val);
    }
    return res;
}
ChunkMetadata* ChunkMap::getMetadata(ivec3 pos)const {
    return metadata.at(pos).get();
}
const AABB*   ChunkMap::getBoundingBox(ivec3 chunk_offset) const{
    return boundingBoxes.at(chunk_offset).get();
}

// assign our neighbours if they exist in the chunkmap,
// also add ourselves  to our neighbours neighbourlist.
void ChunkMap::updateNeighbourMap(ivec3 pos) {
    auto* self_ptr = chunks[pos].get();

    std::array<const Chunk*, NUM_NEIGHBOURS> my_neighbours{};
    for (const auto& dir : eachDirection) {
        const i32   dir_idx = static_cast<i32>(dir);
        const ivec3 neighbourChunkLocal = pos + Direction_offset[dir_idx];
        if (chunks.contains(neighbourChunkLocal)) {
            // assign NEIGHBOUR to ourNeighbours.dir
            const auto* neighbourChunk = chunks[neighbourChunkLocal].get();
            if (!neighbourChunk)
                continue;
            my_neighbours[dir_idx] = neighbourChunk;

            // assign SELF to ourNeighbours.dir.inverseDir
            const auto inverseDir_idx = inverseDirection_n.at(dir_idx);
            neighbours[neighbourChunkLocal][inverseDir_idx] = self_ptr;
        }
    }
    neighbours.emplace(pos, std::move(my_neighbours));
}

void ChunkMap::updateBoundingBoxesMap(ivec3 chunk_coord) {
    const ivec3 min = chunk_coord * Chunk::Extents;
    const ivec3 max = min + Chunk::Extents;
    boundingBoxes.emplace(chunk_coord, std::make_unique<AABB>(min, max));
}

void ChunkMap::generate(ivec3 chunk_coord) {
    //    LOG_DEBUG("Generating chunk for {}", dbg_fmt(chunk_coord));
}

bool ChunkMap::isDirty(ivec3 pos) const {
    return chunks.at(pos)->flags.isDirty;
}
bool ChunkMap::isClean(ivec3 pos) const {
    return !chunks.at(pos)->flags.isDirty;
}

bool ChunkMap::isMeshing(ivec3 pos) const {
    return chunks.at(pos)->flags.isMeshing;
}
bool ChunkMap::isMeshed(ivec3 pos) const {
    return chunks.at(pos)->flags.finishedMeshing;
}
bool ChunkMap::isGenerated(ivec3 pos) const {
    return chunks.at(pos)->flags.finishedGeneration;
}

void ChunkMap::markDirty(ivec3 pos) {
    chunks.at(pos)->flags.isDirty = true;
}

void ChunkMap::markClean(ivec3 pos) {
    chunks.at(pos)->flags.isDirty = false;
}

void ChunkMap::markMeshing(ivec3 pos) {
    chunks.at(pos)->flags.isMeshing = true;
}
void ChunkMap::markMeshed(ivec3 pos) {
    chunks.at(pos)->flags.finishedMeshing= true;
}
void ChunkMap::markGenerated(ivec3 pos) {
    chunks.at(pos)->flags.finishedGeneration = true;
}
