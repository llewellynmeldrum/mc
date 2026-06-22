
#include "ChunkEntry.hpp"
#include "FormatSpecs.hpp"
#include "NothrowLookup.hpp"

#include "ChunkConstants.hpp"
#include "ChunkHelpers.hpp"
#include "CoordTypes.hpp"
#include "ChunkMap.hpp"
#include "CommonUtils.hpp"
#include "Logger.hpp"
#include <memory>
#include <optional>
#include <ranges>

using namespace glm;
void ChunkMap::uploadGeneratedChunk(GenResult gen_res) {
    ChunkStore& generatedBlocks = gen_res.chunkBlocks;
    ChunkMetadata& generatedMeta = gen_res.meta;
    const auto& deferredWrites = gen_res.deferredWrites;
    const auto& chunkCoord = gen_res.chunkCoord;

    auto* entry = AT(entries,chunkCoord);
    // update entry to reflect generation data
    handlePendingWrites(chunkCoord, static_cast<ChunkSpan>(generatedBlocks), deferredWrites);
    
    // why the fuck did they make it (src,dst) fucking AT&T propaganda
    std::ranges::copy(generatedBlocks, entry->block_data.begin());
    entry->metadata=generatedMeta;
    updateNeighbourMap(chunkCoord);
    updateBoundingBoxesMap(chunkCoord);
    mark_mesh_dirty(entry); // allow for meshing
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
                        entries.if_contains(
                            WorldChunkCoord{x,y,z},
                            [](ChunkEntry& neighbour_entry){
                                neighbour_entry.mark_mesh_dirty();
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
        entries.if_contains_else(
            targetChunkCoord,
            [&](ChunkEntry& entry){
                if (entry.block_data.tryWrite(write)){
                    pendingWritesSuccessful++;
                    // also mark the target as dirty,
                    // alongside all its neighbours, since 
                    glm::ivec3 lo = targetChunkCoord.raw() + ivec3{-1,-1,-1};
                    glm::ivec3 hi = targetChunkCoord.raw() + ivec3{1,1,1};
                    ForEachInRangeEx(lo,hi,[&](i32 x, i32 y, i32 z){
                        entries.if_contains(
                            WorldChunkCoord{x,y,z},
                            [](ChunkEntry& neigh_entry){
                                neigh_entry.mark_mesh_dirty();
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
    return &AT(entries,chunkCoord)->bounding_box;
}

// TODO: Continue plugging in the new strong coord types
// then do world gen



// assign our neighbours if they exist in the chunkmap,
// also add ourselves  to our neighbours neighbourlist.
void ChunkMap::updateNeighbourMap(WorldChunkCoord chunkCoord) {
    //NOTE: this might be a good spot to invalidate neighbours meshes after generation, 
    //if they are older than us by some amount.
    auto* self_ptr = &(entries[chunkCoord]->block_data);

    std::array<std::optional<WorldChunkCoord>, N_NEIGHBOURS> my_neighbours{};
    for (const auto& [dir,offset] : eachDirOffset) {
        const i32   dir_idx = static_cast<i32>(dir);
        const auto neighbourChunkCoord = chunkCoord + ChunkOffset{offset};
        entries.if_contains(
            neighbourChunkCoord,
            [&](ChunkEntry& neighbourEntry){
                // 1. assign NEIGHBOUR to OUR NeighbourList @dir
                my_neighbours[dir_idx] = std::make_optional(neighbourChunkCoord);

                entries.if_contains(
                    neighbourChunkCoord,
                    [](ChunkEntry& neighbour_entry){
                        // 2. INVALIDATE THEIR MESH, we have just generated next to them,
                        // and they need to be made aware of our blocks to correctly cull faces.
                        neighbour_entry.mark_mesh_dirty();
                    }
                );

                // 3. assign OURSELVES to NEIGHBOUR.dir @inverseDir
                const auto inverseDir_idx = inverseDirection_n.at(dir_idx);
                
                auto& neighbours_neighbours = entries[neighbourChunkCoord]->neighbours;
                if (neighbours_neighbours[inverseDir_idx]){
                    neighbours_neighbours[inverseDir_idx] = std::make_optional(chunkCoord);
                }
            }
        );
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


