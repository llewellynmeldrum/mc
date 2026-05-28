#include "ChunkHelpers.hpp"
#include "CoordTypes.hpp"
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
            const auto& targetChunkCoord = toWorldChunkCoord(write.targetWorldBlockPos);
            if (entries.contains(targetChunkCoord)){
                auto& entry = entries.at(targetChunkCoord);
                pendingWritesSuccessful += entry->block_data.tryWrite(write);
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
const AABB*   ChunkMap::getBoundingBox(WorldChunkCoord chunkCoord) const{
    return &entries.at(chunkCoord)->bounding_box;
}

// TODO: Continue plugging in the new strong coord types
// then do world gen



// assign our neighbours if they exist in the chunkmap,
// also add ourselves  to our neighbours neighbourlist.
void ChunkMap::updateNeighbourMap(WorldChunkCoord chunkCoord) {
    //NOTE: this might be a good spot to invalidate neighbours meshes after generation, 
    //if they are older than us by some amount.
    auto* self_ptr = &entries[chunkCoord]->block_data;

    std::array<const Chunk*, N_NEIGHBOURS> my_neighbours{};
    for (const auto& [dir,offset] : eachDirOffset) {
        const i32   dir_idx = static_cast<i32>(dir);
        const auto neighbourChunkCoord = chunkCoord + ChunkOffset{offset};
        if (entries.contains(neighbourChunkCoord)) {
            // assign NEIGHBOUR to OUR NeighbourList @dir
            const auto* neighbourChunk = &entries[neighbourChunkCoord]->block_data;
            if (!neighbourChunk){
                continue;
            }
            my_neighbours[dir_idx] = neighbourChunk;

            // assign OURSELVES to NEIGHBOUR.dir @inverseDir
            const auto inverseDir_idx = inverseDirection_n.at(dir_idx);
            entries[neighbourChunkCoord]->neighbours[inverseDir_idx] = self_ptr;
        }
    }
    // what the fuck am i looking at 
    entries.at(chunkCoord)->neighbours.assign_range(std::move(my_neighbours));

}

void ChunkMap::updateBoundingBoxesMap(WorldChunkCoord chunkCoord) {
    const auto min = toWorldOrigin(chunkCoord);
    const auto max = min + BlockOffset{Chunk::Extents};
    entries.at(chunkCoord)->bounding_box = {min, max};
}


