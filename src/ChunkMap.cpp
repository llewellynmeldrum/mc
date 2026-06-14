
#include "DebugFormatSpecializations.hpp"
#include "NothrowLookup.hpp"

#include "ChunkConstants.hpp"
#include "ChunkHelpers.hpp"
#include "CoordTypes.hpp"
#include "ChunkMap.hpp"
#include "CommonUtils.hpp"
#include "Logger.hpp"
#include <memory>
#include <ranges>

using namespace glm;
void ChunkMap::uploadGeneratedChunk(GenResult gen_res) {
    ChunkStore& generatedBlocks = gen_res.chunkBlocks;
    ChunkMetadata& generatedMeta = gen_res.meta;
    const auto& deferredWrites = gen_res.deferredWrites;
    const auto& chunkCoord = gen_res.chunkCoord;

    auto* entry = make_entry(chunkCoord);
    auto& state = states.at(chunkCoord);
    // update entry to reflect generation data
    handlePendingWrites(chunkCoord, static_cast<ChunkSpan>(generatedBlocks), deferredWrites);
    
    // why the fuck did they make it (src,dst) fucking AT&T propaganda
    std::ranges::copy(generatedBlocks, entry->block_data.begin());
    entry->metadata=generatedMeta;
    updateNeighbourMap(chunkCoord);
    updateBoundingBoxesMap(chunkCoord);
}
void ChunkMap::handlePendingWrites(const WorldChunkCoord chunkCoord, ChunkSpan srcBlocks, const PendingWriteList& newWriteList) {
    // 1. apply any pending writes TO CURRENT chunk which exist on the map.
    if (pendingWritesMap.contains(chunkCoord)){
        PendingWriteQueue& writesForMe = AT(pendingWritesMap,chunkCoord);
        while (!writesForMe.empty()){
            const auto write = writesForMe.top(); writesForMe.pop();
            //TODO: mark all neighbours as dirty after applying the writes to current
            if (srcBlocks.tryWrite(write)){
                pendingWritesSuccessful ++;
                glm::ivec3 lo = chunkCoord.raw() + ivec3{-1,-1,-1};
                glm::ivec3 hi = chunkCoord.raw() + ivec3{1,1,1};
                ForEachInRangeEx(lo,hi,[&](i32 x, i32 y, i32 z){
                    const auto neigh_state = try_get_state(WorldChunkCoord{x,y,z});
                    if (neigh_state){
                        (*neigh_state)->makeDirtyIfMeshed();
                    }
                });
            }
            pendingWritesAttempted++;
        }
        if (AT(pendingWritesMap,chunkCoord).empty()){
            // remove the queue if it no longer has anything remaining
            // TODO: Is this is a bad idea?
            pendingWritesMap.erase(chunkCoord);
        }
    }
    
    // 2. Apply any NEW pending writes TO OTHER chunks from pwl
    for (const auto& write: newWriteList){
        // a.) if the TARGET chunk exists, apply the write IMMEDIATELY to the TARGET chunk
        const auto& targetChunkCoord = toWorldChunkCoord(write.targetWorldBlockPos);
        if (has_entry(targetChunkCoord)){
            auto& entry = AT(entries,targetChunkCoord);
            if (entry->block_data.tryWrite(write)){
                pendingWritesSuccessful++;
                // also mark the target as dirty,
                // alongside all its neighbours, since 
                glm::ivec3 lo = targetChunkCoord.raw() + ivec3{-1,-1,-1};
                glm::ivec3 hi = targetChunkCoord.raw() + ivec3{1,1,1};
                ForEachInRangeEx(lo,hi,[&](i32 x, i32 y, i32 z){
                    const auto neigh_state = try_get_state(WorldChunkCoord{x,y,z});
                    if (neigh_state){
                        (*neigh_state)->makeDirtyIfMeshed();
                    }
                });
            }
            pendingWritesAttempted++;
        }else{
            // b.) if the TARGET chunk DOESNT exist, create an entry in the pending writes map,
            // and then enqueue the write.
            bool pendingWritesEntryExists = pendingWritesMap.contains(targetChunkCoord);
            if (!pendingWritesEntryExists){
                pendingWritesMap.emplace(targetChunkCoord, std::priority_queue<PendingBlockWrite>{});
            }
            AT(pendingWritesMap,targetChunkCoord).push(write);
        }
    }
}
const AABB*   ChunkMap::getBoundingBox(WorldChunkCoord chunkCoord) const{
    return &AT(entries,chunkCoord)->bounding_box;
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
            auto* neighbourEntry = get_entry(neighbourChunkCoord);
            auto* neighbourState = get_state(neighbourChunkCoord);
            my_neighbours[dir_idx] = &neighbourEntry->block_data;
            // also INVALIDATE THEIR MESH
            if (neighbourState->isCleanMeshed()){
                neighbourState->markMeshAsDirty();
            }
            // NOTE: This is an optmisation for opaque chunks, 
            // but necessary for transparent chunks. 

            // assign OURSELVES to NEIGHBOUR.dir @inverseDir
            const auto inverseDir_idx = inverseDirection_n.at(dir_idx);
            entries[neighbourChunkCoord]->neighbours[inverseDir_idx] = self_ptr;
        }
    }
    // ALSO!!! invalidate all neighbours mesh as invalid here
    
    // what the fuck am i looking at 
    AT(entries,chunkCoord)->neighbours.assign_range(std::move(my_neighbours));

}

void ChunkMap::updateBoundingBoxesMap(WorldChunkCoord chunkCoord) {
    const auto min = toWorldOrigin(chunkCoord).raw();
    const auto max = min + Chunk::Extents;
    AT(entries,chunkCoord)->bounding_box = {min, max};
}


