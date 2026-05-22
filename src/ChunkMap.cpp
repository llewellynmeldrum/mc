
#include "ChunkHelpers.hpp"
#include "DebugFormatSpecializations.hpp"
#include "ChunkMap.hpp"
#include "CommonUtils.hpp"
#include "Logger.hpp"
#include <memory>
#include <ranges>

using namespace glm;
    void ChunkMap::handlePendingWrites(const WorldChunkCoord chunkCoord, ChunkSpan srcBlocks, const PendingWriteList& newWriteList) {
        // 1. apply any pending writes TO CURRENT chunk which exist on the map.
        if (pendingWritesMap.contains(chunkCoord)){
            PendingWriteQueue& writesForMe = pendingWritesMap.at(chunkCoord);
            while (!writesForMe.empty()){
                const auto write = writesForMe.top(); writesForMe.pop();
                pendingWritesSuccessful += srcBlocks.tryWrite(write);
                pendingWritesAttempted++;
            }
            if (pendingWritesMap.at(chunkCoord).empty()){
                // remove the queue if it no longer has anything remaining
                // TODO: Is this is a bad idea?
                pendingWritesMap.erase(chunkCoord);
            }
        }
        
        // 2. Apply any NEW pending writes TO OTHER chunks from pwl
        for (const auto& write: newWriteList){
            // a.) if the TARGET chunk exists, apply the write IMMEDIATELY to the TARGET chunk
            const auto& targetChunkCoord = toWorldChunkCoord(write.blockPos);
            if (chunks.contains(targetChunkCoord)){
                pendingWritesSuccessful += chunks.at(targetChunkCoord)->tryWrite(write);
                pendingWritesAttempted++;
            }else{
                // b.) if the TARGET chunk DOESNT exist, create an entry in the pending writes map,
                // and then enqueue the write.
                bool mapEntryExists = pendingWritesMap.contains(targetChunkCoord);
                if (!mapEntryExists){
                    pendingWritesMap.emplace(targetChunkCoord, std::priority_queue<PendingBlockWrite>{});
                }
                pendingWritesMap.at(targetChunkCoord).push(write);
            }
        }
    }
std::span<const Chunk*const> ChunkMap::getSurroundingChunks(ivec3 pos) const {
    std::span res{neighbours.at(pos)};
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
    chunks.at(pos)->flags.finishedMeshing= false;
}

void ChunkMap::markClean(ivec3 pos) {
    chunks.at(pos)->flags.isDirty = false;
    chunks.at(pos)->flags.finishedMeshing= true;
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
