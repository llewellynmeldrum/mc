#include "ChunkScheduler.hpp"
#include "CoordIteration.hpp"
#include "World.hpp"

std::vector<WorldChunkCoord> ChunkScheduler::find_gen_ready_chunks(WorldChunkCoord center, glm::ivec3 extents, std::size_t max){
    auto candidates = std::vector<WorldChunkCoord>();
    candidates.reserve(max);


    for_each_spiral(
        max,
        center, 
        extents,
        [this, &candidates](i32 x, i32 y, i32 z) -> bool {
            const auto key = WorldChunkCoord{x,y,z};
            bool candidate_qualifies = world.entries.if_contains_else(
                key,
                [&](ChunkEntry& entry){
                    // 1. if an entry exists, check if it needs regeneration.
                    return entry.is_gen_dirty();
                },
                [&](){
                    // 2. if no entry exists; then the chunk hasnt been generated => it qualifies.
                    return true;
                }
            );
            if (candidate_qualifies) candidates.emplace_back(key);
            return candidate_qualifies;
        }
    );
    return candidates;
}
bool ChunkScheduler::submit_gen_request(WorldChunkCoord coord){
    auto transition_to_gen_enqueue = [](ChunkEntry& entry){
        entry.state_transition(gen_enqueue);
    };
    auto make_chunk_entry = [&](){
        world.make_chunk_entry(coord);
    };

    bool success = gen.job_q.try_emplace(
        coord, 
        world.seed,
        world.genConfig
    );
    if (success){
        world.entries.if_contains_else(
            coord,
            transition_to_gen_enqueue,
            make_chunk_entry
        );
    }
    return success;
}

std::vector<WorldChunkCoord> ChunkScheduler::find_mesh_ready_chunks(WorldChunkCoord center, glm::ivec3 extents, std::size_t max){
    auto candidates = std::vector<WorldChunkCoord>();
    candidates.reserve(max);
    // pull some candidates from mesh_candidates set
    const auto candidate_count = std::min(max,world.persistent_mesh_candidates.size());
    // 1. trim the current persistent_mesh_candidates

    trim_empty_meshes(world.persistent_mesh_candidates);

    std::vector<WorldChunkCoord> candidateList;

    auto it = world.persistent_mesh_candidates.cbegin();
    for (auto i = 0uz; i<candidate_count; i++){
        candidateList.emplace_back(*it);
        it++;
    }
    return candidateList;
}
// TODO: mid refactor moving out stuff into chunkscheduler

std::size_t ChunkScheduler::trim_empty_meshes(std::unordered_set<WorldChunkCoord>& set){
    auto n = 0uz;
    for (auto it = set.begin(); it!=set.end(); it++){
        auto* entry = AT(world.entries,*it);
//        if (!entry->qualifies_for_mesh_enqueue()) return true;
        if (entry->block_data.isAllAir()){
            //NOTE: skip mesh enqueue entirely
            entry->inflight_mesh_revision = entry->target_mesh_revision;
            entry->loaded_mesh_revision = entry->inflight_mesh_revision;
            entry->state.mesh = MeshState::done;
            //this->chunksMeshed++; // It didnt actually get meshed.
            it = world.persistent_mesh_candidates.erase(it);
            log_to_all("EARLY Skipped a mesh: all blocks are air.");
            n++;
        }
    }
    return n;
}

GenJob ChunkScheduler::construct_gen_job(WorldChunkCoord coord){

}
MeshJob ChunkScheduler::construct_mesh_job(WorldChunkCoord coord){
    std::vector<std::optional<ChunkSlice2D>> neighbours_copy;
    for (const auto& [dir, dir_idx]: eachDirIndex){
        const auto& neighbour_coord = entry->neighbours[dir_idx];
        if (!neighbour_coord){
            neighbours_copy.emplace_back(std::nullopt);
            continue;
        }
        ChunkBlockPos p0{}, p1{};
        constexpr auto XE = CHUNK_XWIDTH;
        constexpr auto YE = CHUNK_HEIGHT;
        constexpr auto ZE = CHUNK_ZWIDTH;
        SliceType slice_type = {};
        switch (dir){
            // -Z
            case Direction ::BACKWARD: p0 = {0,0,0}; p1 ={XE,YE,1}; slice_type = SliceType::Z; break;
            // +Z
            case Direction ::FORWARD: p0 = {0,0,ZE-1}; p1 ={XE,YE,ZE}; slice_type = SliceType::Z; break;

            // -X
            case Direction ::RIGHT: p0 = {0,0,0}; p1 ={1,YE,ZE}; slice_type = SliceType::X; break;
            // +X
            case Direction ::LEFT   : p0 = {XE-1,0,0}; p1 ={XE,YE,ZE}; slice_type = SliceType::X; break;

            // - Y
            case Direction ::UP :p0 = {0,0,0}; p1 ={XE,1,ZE}; slice_type = SliceType::Y; break;
            // + Y
            case Direction ::DOWN     :p0 = {0,YE-1,0}; p1 ={XE,YE,ZE}; slice_type = SliceType::Y; break;
            default:
                break;
        }
        world.entries.if_contains_else(
            neighbour_coord.value(),
            [&](ChunkEntry& neighbour){
                std::optional<ChunkSlice2D> copy = std::make_optional<ChunkSlice2D>(
                   &neighbour.block_data,
                    slice_type,
                    p0,
                    p1
                );
                neighbours_copy.emplace_back(copy);
            },
            [&](){
                neighbours_copy.emplace_back(std::nullopt);
            }
        );
    }
    return{entry->target_mesh_revision,candidateCoord,&rend.atlas,entry,neighbours_copy};
}
