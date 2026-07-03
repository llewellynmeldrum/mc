#include "ChunkDirector.hpp"
void ChunkDirector::upload_generated_chunk(GenResult gen_res) {
    ChunkStore& generatedBlocks = gen_res.chunkBlocks;
    const auto& deferredWrites = gen_res.deferredWrites;
    const auto& chunkCoord = gen_res.chunkCoord;

    auto* entry = AT(chunk_map.entries,chunkCoord);
    // update entry to reflect generation data
    handle_pending_writes(chunkCoord, static_cast<ChunkSpan>(generatedBlocks), deferredWrites);
    
    // why the fuck did they make it (src,dst) fucking AT&T propaganda
    std::ranges::copy(generatedBlocks, entry->block_data.begin());
    update_neighbour_map(chunkCoord);
    update_bounding_boxes_map(chunkCoord);
    mark_neighbours_dirty(chunkCoord,"Neighbour generated");
    mark_mesh_dirty(*entry,"Newly generated"); // allow for meshing
}
void ChunkDirector::handle_pending_writes(const WorldChunkCoord chunkCoord, ChunkSpan srcBlocks, const PendingWriteList& newWriteList) {
    // 1. apply any pending writes TO CURRENT chunk which exist on the map.
    chunk_map.pending_writes.if_contains(
        chunkCoord,
        [&](PendingWriteQueue & writesForMe){
            while (!writesForMe.empty()){
                const auto write = writesForMe.top(); writesForMe.pop();
                if (srcBlocks.tryWrite(write)){
                    // TODO: add pending writes to queue -> requests and fulfillments
                    chunk_map.pendingWritesSuccessful ++;
                    // NOTE:
                    // We make all adjacent chunks meshes dirty, as a block change has occured
                    // potentially on the border.
                    // In future, it might be good to distinguish this, i.e only endirty
                    // the actual chunks it impacts (if on border/corner it impacts however many)
                    mark_neighbours_dirty(chunkCoord);
                }
                chunk_map.pendingWritesAttempted++;
            }
            if (AT(chunk_map.pending_writes,chunkCoord)->empty()){
                // remove the queue if it no longer has anything remaining
                // Q: Is this is a bad idea?
                chunk_map.pending_writes.erase(chunkCoord);
            }
        }
    );
    
    // 2. Apply any NEW pending writes TO OTHER chunks from pwl
    for (const auto& write: newWriteList){
        // a.) if the TARGET chunk exists, apply the write IMMEDIATELY to the TARGET chunk
        const auto& targetChunkCoord = toWorldChunkCoord(write.targetWorldBlockPos);
        chunk_map.entries.if_contains_else(
            targetChunkCoord,
            [&](ChunkEntry& entry){
                if (entry.block_data.tryWrite(write)){
                    chunk_map.pendingWritesSuccessful++;
                    // also mark the target as dirty,
                    // alongside all its neighbours, since 
                    // ... since what
                    mark_neighbours_dirty(targetChunkCoord);
                }
                chunk_map.pendingWritesAttempted++;
            },
            [&](){
                // b.) if the TARGET chunk DOESNT exist, create an entry in the pending writes map,
                // and then enqueue the write.
                auto* target_queue = chunk_map.pending_writes.get_or_emplace(targetChunkCoord,std::priority_queue<PendingBlockWrite>{});
                target_queue->push(write);
            }
        );
    }
}



// assign our neighbours if they exist in the chunkmap,
// also add ourselves  to our neighbours neighbourlist.
void ChunkDirector::update_neighbour_map(WorldChunkCoord chunkCoord) {
    auto* self_ptr = &(chunk_map.entries[chunkCoord]->block_data);

    std::array<std::optional<WorldChunkCoord>, N_NEIGHBOURS> my_neighbours{};
    for (const auto& [dir,offset] : eachDirOffset2D) {
        const i32   dir_idx = static_cast<i32>(dir);
        const auto neighbourChunkCoord = chunkCoord + ChunkOffset{offset};
        chunk_map.entries.if_contains(
            neighbourChunkCoord,
            [&](ChunkEntry& neighbour_entry){
                // 1. assign NEIGHBOUR to OUR NeighbourList @dir
                my_neighbours[dir_idx] = std::make_optional(neighbourChunkCoord);
                        // 2. INVALIDATE THEIR MESH, we have just generated next to them,
                        // and they need to be made aware of our blocks to correctly cull faces.
                    mark_mesh_dirty(neighbour_entry,"Neighbour is newly generated. (update_neighbour_map)");


                // 3. assign OURSELVES to NEIGHBOUR.dir @inverseDir
                const auto inverseDir_idx = inverseDirection_n.at(dir_idx);
                
                auto& neighbours_neighbours = chunk_map.entries[neighbourChunkCoord]->neighbours;
                neighbours_neighbours[inverseDir_idx] = std::make_optional(chunkCoord);
            }
        );
    }
    // ALSO!!! invalidate all neighbours mesh as invalid here
    
    // what the fuck am i looking at 
    AT(chunk_map.entries,chunkCoord)->neighbours.assign_range(std::move(my_neighbours));

}

void ChunkDirector::update_bounding_boxes_map(WorldChunkCoord chunkCoord) {
    const auto min = toWorldOrigin(chunkCoord).raw();
    const auto max = min + Chunk::Extents;
    AT(chunk_map.entries,chunkCoord)->bounding_box = {min, max};
}

