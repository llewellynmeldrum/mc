
#include "FormatSpecs.hpp"
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

    auto* entry = chunk_entries.emplace(chunkCoord,chunkCoord);
    auto* state = states.at(chunkCoord);
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
    pending_writes.if_contains(
        chunkCoord,
        [&](PendingWriteQueue & writesForMe){
            while (!writesForMe.empty()){
                const auto write = writesForMe.top(); writesForMe.pop();
                //TODO: mark all neighbours as dirty after applying the writes to current
                if (srcBlocks.tryWrite(write)){
                    pendingWritesSuccessful ++;
                    glm::ivec3 lo = chunkCoord.raw() + ivec3{-1,-1,-1};
                    glm::ivec3 hi = chunkCoord.raw() + ivec3{1,1,1};
                    ForEachInRangeEx(lo,hi,[&](i32 x, i32 y, i32 z){
                        states.if_contains(
                            WorldChunkCoord{x,y,z},
                            [](ChunkState& neigh_state){
                                neigh_state.mark_dirty_mesh();
                            }
                        );
                    });
                }
                pendingWritesAttempted++;
            }
            if (AT(pending_writes,chunkCoord)->empty()){
                // remove the queue if it no longer has anything remaining
                // TODO: Is this is a bad idea?
                pending_writes.erase(chunkCoord);
            }
        }
    );
    
    // 2. Apply any NEW pending writes TO OTHER chunks from pwl
    for (const auto& write: newWriteList){
        // a.) if the TARGET chunk exists, apply the write IMMEDIATELY to the TARGET chunk
        const auto& targetChunkCoord = toWorldChunkCoord(write.targetWorldBlockPos);
        chunk_entries.if_contains_else(
            targetChunkCoord,
            [&](ChunkEntry& entry){
                if (entry.block_data.tryWrite(write)){
                    pendingWritesSuccessful++;
                    // also mark the target as dirty,
                    // alongside all its neighbours, since 
                    glm::ivec3 lo = targetChunkCoord.raw() + ivec3{-1,-1,-1};
                    glm::ivec3 hi = targetChunkCoord.raw() + ivec3{1,1,1};
                    ForEachInRangeEx(lo,hi,[&](i32 x, i32 y, i32 z){
                        states.if_contains(
                            WorldChunkCoord{x,y,z},
                            [](ChunkState& neigh_state){
                                neigh_state.mark_dirty_mesh();
                            }
                        );
                    });
                }
                pendingWritesAttempted++;
            },
            [&](){
                // b.) if the TARGET chunk DOESNT exist, create an entry in the pending writes map,
                // and then enqueue the write.
                auto* target_queue = pending_writes.get_or_emplace(targetChunkCoord,std::priority_queue<PendingBlockWrite>{});
                target_queue->push(write);
            }
        );
    }
}
const AABB*   ChunkMap::getBoundingBox(WorldChunkCoord chunkCoord) const{
    return &AT(chunk_entries,chunkCoord)->bounding_box;
}

// TODO: Continue plugging in the new strong coord types
// then do world gen



// assign our neighbours if they exist in the chunkmap,
// also add ourselves  to our neighbours neighbourlist.
void ChunkMap::updateNeighbourMap(WorldChunkCoord chunkCoord) {
    //NOTE: this might be a good spot to invalidate neighbours meshes after generation, 
    //if they are older than us by some amount.
    auto* self_ptr = &(chunk_entries[chunkCoord]->block_data);

    std::array<const Chunk*, N_NEIGHBOURS> my_neighbours{};
    for (const auto& [dir,offset] : eachDirOffset) {
        const i32   dir_idx = static_cast<i32>(dir);
        const auto neighbourChunkCoord = chunkCoord + ChunkOffset{offset};
        chunk_entries.if_contains(
            neighbourChunkCoord,
            [&](ChunkEntry& neighbourEntry){
                // assign NEIGHBOUR to OUR NeighbourList @dir
                auto* neighbourState = states.at(neighbourChunkCoord);
                my_neighbours[dir_idx] = &(neighbourEntry.block_data);
                // also INVALIDATE THEIR MESH
                neighbourState->mark_dirty_mesh();

                // NOTE: This is an optmisation for opaque chunks, 
                // but NECESSARY for transparent chunks. 

                // assign OURSELVES to NEIGHBOUR.dir @inverseDir
                const auto inverseDir_idx = inverseDirection_n.at(dir_idx);
                chunk_entries[neighbourChunkCoord]->neighbours[inverseDir_idx] = self_ptr;
            }
        );
    }
    // ALSO!!! invalidate all neighbours mesh as invalid here
    
    // what the fuck am i looking at 
    AT(chunk_entries,chunkCoord)->neighbours.assign_range(std::move(my_neighbours));

}

void ChunkMap::updateBoundingBoxesMap(WorldChunkCoord chunkCoord) {
    const auto min = toWorldOrigin(chunkCoord).raw();
    const auto max = min + Chunk::Extents;
    AT(chunk_entries,chunkCoord)->bounding_box = {min, max};
}


