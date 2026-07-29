#include "ChunkDirector.hpp"
#include "Renderer.hpp"
#include "ChunkNoiseDebug.hpp"

void ChunkDirector::handle_mesh_sorting(Renderer& rend, WorldFloatPos player_cam_pos){
    // NOTE: 
    // The reordering process is decently complex, but for good reason:
    // -> If we were broad and heavy-handed about this, we could just resort every single chunk, every single frame,
    // and every single face/quad within every single chunk mesh.
    // -> In a large enough scene, (say 16 chunk render dist, superflat world), 
    // that is AT LEAST: 
    // -> 1089 O(log(n)) sorts, each around 256 faces. Thats 278'784 total ops. >1m ops on 64 render dist. 
    // -> + the 1089 chunks, which also need to be sorted.
    // Thats every single frame. And we barely even gain anything. the cost will almost definitely dominate any savings from overdraw if we are doing opaque sorting. So thats out the window.
    // Second off, something like 90% of the work we do sorting the blended faces is COMPLETELY useless.
    // The player is not realistically able to make out per-quad draw order issues on most textures from >~30 blocks away. So all this work to make everything correct, and we lose a potential optimisation, AND gain little to no correctness.



    // Anyways, this is the process, two main steps:
    // 1. Perform chunk-wise sorting (resort the draw order of entire CHUNK meshes)
    // -> This can occur entirely CPU side, as we are deciding simply the order of 
    //    glDraw() commands. 
    // -> It is also not too expensive, and is at worst O(log(C)), where C = number of chunks meshed
    // -> Thus, we can be more broad. This is like the broad phase of a collision detection system
    //
    bool blended_sorted_mismatch = rend.blended_chunk_meshes.size() != rend.sorted_blended_coords.size();
    if (player_crossed_chunk_boundary() ||
        blended_sorted_mismatch)
    {
        rend.sort_blended_chunks(player_cam_pos);
    }

    if(DebugOption::enable_opaque_sorting){
        bool opaque_sorted_mismatch = rend.opaque_chunk_meshes.size() != rend.sorted_opaque_coords.size();
        if (player_crossed_chunk_boundary() || opaque_sorted_mismatch) {
            rend.sort_opaque_chunks(player_cam_pos);
        }
        bool cutout_sorted_mismatch = rend.cutout_chunk_meshes.size() != rend.sorted_cutout_coords.size();
        if (player_crossed_chunk_boundary() || cutout_sorted_mismatch) {
            rend.sort_cutout_chunks(player_cam_pos);
        }
    }

    // 2. Perform quad-wise sorting (resort the draw order of EACH QUAD.)
    // -> This requires a reupload of the EBO to handle the reordered vertices WITHIN a mesh
    // -> This is more expensive, being around O(log(B)), where B is the number of BLOCKS TOTAL we are re-sorting.
    // -> Thus, we cannot be as broad. We only do this for a subset of chunks in a small radius around the player.
    //
    // -> It is hard to notice individual quads out of order if they are a few chunks away - it is VERY easy to notice
    //    if the entire CHUNKS draw order is wrong.
    
    // The radius of chunks AROUND the players chunk that will have every single mesh resorted 
    // after a block boundary cross.
    static constexpr i32 per_quad_resort_chunk_radius = {2};
    if (player_crossed_block_boundary()){
        // first, lets just try resorting only the current chunks mesh
        std::vector<WorldChunkCoord> resort_every_quad_victims{
            cur_chunk_pos
        };
        const auto& R = per_quad_resort_chunk_radius;
        for (i32 x = -R; x<=R; x++){
        for (i32 z = -R; z<=R; z++){
            resort_every_quad_victims.push_back(cur_chunk_pos + ChunkOffset{x,z});
        }
        }
        for (const auto& chunk_coord: resort_every_quad_victims){
            rend.blended_chunk_meshes.if_contains(
                chunk_coord,
                [&](IndexedMesh& mesh){
                    mesh.resort_quad_indices(player_cam_pos,false);
                }
            );
        }
    }
}

void ChunkDirector::discover_candidates(
    ChunkBenchmarkerNoRevision & mesh_enqueue_delay_bench, 
    ChunkBenchmarkerNoRevision & gen_enqueue_delay_bench){
    // if we come across a chunk which:
    // - has an entry
    // - has been generated
    // - mesh=ready_for_enqueue
    // - its NOT on ready_for_mesh, 
    // ADD it to ready_for_mesh.
    const auto chunkCoord = cur_chunk_pos;
    // enumerate them based on their range to the player, such that nearest chunks come first.

    const auto mesh_sq_dist_req = std::pow(RENDER_DIST,2);
    const auto lighting_sq_dist_req = std::pow(LIGHTING_DIST,2);
    auto gen_count = 0uz;
    auto light_count = 0uz;
    auto mesh_count = 0uz;
    for_each_spiral(chunkCoord, GENERATION_DIST, 
        [&](i32 x, i32 z) {
            const auto key = WorldChunkCoord{x,z};
            auto* chunk = chunk_map.entries.try_get(key);
            bool is_mesh_candidate = false;
            bool is_gen_candidate = false;
            bool is_light_candidate = false;
            if (chunk){
                if (chunk->can_be_generated()){
                    // 1. if an entry exists, check if it needs regeneration.
                    is_gen_candidate = true;
                }
                if (chunk->can_be_lit()
                    && LM::sq_dist(chunkCoord, key) < lighting_sq_dist_req){
                    is_light_candidate = true;
                }
                if (chunk->can_be_meshed() 
                    && LM::sq_dist(chunkCoord, key) < mesh_sq_dist_req){
                    is_mesh_candidate = true;
                }
            }else{
                // 2. if an entry doesnt exists, it needs to be generated.
                is_gen_candidate = true;
            }

            if (is_gen_candidate && gen_count < max_gen_discovery_pf){
                gen_enqueue_delay_bench.bench_start(key);
                ready_for_gen.push(key);
                gen_count++;
            }
            if (is_light_candidate && light_count < max_light_discovery_pf){
                //light_enqueue_delay_bench.bench_start(key);
                ready_for_lighting.push(key);
                light_count++;
            }
            if (is_mesh_candidate && mesh_count < max_mesh_discovery_pf){
                mesh_enqueue_delay_bench.bench_start(key);
                ready_for_mesh.push(key);
                mesh_count++;
            }
            bool max_meshes_found = mesh_count >= max_mesh_discovery_pf;
            bool max_gens_found = gen_count >= max_gen_discovery_pf;
            bool max_lights_found = light_count >= max_light_discovery_pf;

            return !(max_meshes_found && max_gens_found && max_lights_found);
        }
    );
}

void ChunkDirector::upload_generated_chunk(GenResult gen_res) {
    ChunkBlockStore& generatedBlocks = gen_res.chunkBlocks;
    const auto& deferredWrites = gen_res.deferredWrites;
    const auto& chunkCoord = gen_res.chunkCoord;

    auto* entry = AT(chunk_map.entries,chunkCoord);
    handle_pending_writes(chunkCoord, generatedBlocks.view(), deferredWrites);
    // why the fuck did they make it (src,dst) fucking AT&T propaganda
    
    ranges::copy(generatedBlocks, entry->block_data.begin());

    //mark_neighbours_dirty(chunkCoord,"Neighbour generated");
    mark_lighting_dirty(entry, "newly generated");
//    mark_mesh_dirty(*entry,"Newly generated"); // allow for meshing
}
void ChunkDirector::handle_pending_writes(const WorldChunkCoord chunkCoord, ChunkBlockView srcBlocks, const PendingWriteList& newWriteList) {
    // 1. apply any pending writes TO CURRENT chunk which exist on the map.
    chunk_map.pending_writes.if_contains(
        chunkCoord,
        [&](auto & writesForMe){
            while (!writesForMe.empty()){
                const auto write = writesForMe.back(); 
                writesForMe.pop_back();
                if (tryWrite(write,srcBlocks)){
                    chunk_map.pendingWritesSuccessful ++;
                    // NOTE:
                    // We make all adjacent chunks meshes dirty, as a block change has occured
                    // potentially on the border.
                    // In future, it might be good to distinguish this, i.e only endirty
                    // the actual chunks it impacts (if on border/corner it impacts however many)
                    mark_neighbour_meshes_dirty(chunkCoord);
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
        // a.) if the TARGET chunk IS GENERATED, apply the write IMMEDIATELY to the TARGET chunk
        const auto& targetChunkCoord = toWorldChunkCoord(write.target_world);
        auto* target_entry = chunk_map.entries.try_get(targetChunkCoord);
        bool target_chunk_is_generated = target_entry && target_entry->gen.has_data();
        if (target_chunk_is_generated){
            // if target exists, and is generated, attempt the write
            if (tryWrite(write,target_entry->block_data.view())){
                chunk_map.pendingWritesSuccessful++;
                // also mark the target as dirty,
                // alongside all its neighbours
                mark_neighbour_meshes_dirty(targetChunkCoord);
            }
            chunk_map.pendingWritesAttempted++;
        }else{
            // B -> entry either doesnt exist or is on gen queue, 
            // regardless we must push the write to the chunks queue 
            auto* target_queue = chunk_map.get_or_emplace_pwq(targetChunkCoord);
            target_queue->push_back(write);

        }
    }
}





