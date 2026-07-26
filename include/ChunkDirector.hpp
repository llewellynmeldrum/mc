#pragma once 

#include <optional>
#include <print>
#include <queue>
#include <span>
#include <unordered_set>


#include "ChunkConcurrency.hpp"
#include "ChunkEntry.hpp"
#include "ChunkMap.hpp"
#include "CoordIteration.hpp"
#include "CoordTypes.hpp"
#include "LM.hpp"
#include "Logger.hpp"
#include "FormatSpecs.hpp"
#include "UniqueQueue.hpp"
#include "cppslop.hpp"
#include "glm/gtx/norm.hpp"

FORWARD_DECL_STRUCT(Renderer)
struct ChunkDirector{

    ChunkMap& chunk_map;
    WorldBlockPos prev_block_pos{};
    WorldBlockPos cur_block_pos{};
    WorldChunkCoord prev_chunk_pos{};
    WorldChunkCoord cur_chunk_pos{};
    // pushed to by mark_mesh_dirty()
    UniqueQueue<WorldChunkCoord> ready_for_gen;
    UniqueQueue<WorldChunkCoord> ready_for_mesh;
    // ============
    // Meshing 
    // ============

    bool place_block(WorldBlockPos wpos, BlockType block){
        auto* chunk = chunk_map.entries.try_get(toWorldChunkCoord(wpos));
        std::println("chunk:{}",toWorldChunkCoord(wpos));
        if (chunk){
            ChunkBlockPos cpos = toChunkBlockPos(wpos);
            std::println("cpos:{}",cpos);
            chunk->block_data.at(cpos) = block;
            mark_mesh_dirty(chunk);
            return true;
        }
        return false;
    }
    std::optional<Block> block_at(WorldBlockPos wpos)const noexcept{
        auto* chunk = chunk_map.entries.try_get(toWorldChunkCoord(wpos));
        if (chunk){
            ChunkBlockPos cpos = toChunkBlockPos(wpos);
            return std::make_optional(chunk->block_data.at(cpos));
        }
        return std::nullopt;
    }
    void handle_mesh_sorting(Renderer& rend, WorldFloatPos player_cam_pos);
    bool qualifies_for_mesh_enqueue(ChunkEntry& entry){
        // 1. ensure i qualify
        if (!entry.qualifies_for_mesh_enqueue()){
            return false;
        }
        // 2. ensure neighbours are generated
        for (const auto& opt_neighbour_coord: entry.neighbours){
            if (!opt_neighbour_coord){
                return false;
            }
            auto neighbour_coord = opt_neighbour_coord.value();
            bool neighbour_generated = chunk_map.entries.if_contains_else(
                neighbour_coord,
                [&](ChunkEntry& neigh_entry){
                    return neigh_entry.state.gen == PipelineState::done;
                },
                [&](){
                    // neigh entry doesnt exist, not generated.
                    return false;
                }
            );
            if (!neighbour_generated){
                return false;
            }
        }
        return true;
    }

    void mark_gen_enqueue(ChunkEntry& entry, std::string_view reason="N/A"){
        ready_for_gen.erase(entry.coord);
        entry.gen_revision.inflight = entry.gen_revision.target;
        entry.state_transition(gen_enqueue);
    }

    void mark_mesh_enqueue(ChunkEntry& entry, std::string_view reason="N/A"){
        ready_for_mesh.erase(entry.coord);
        entry.mesh_revision.inflight = entry.mesh_revision.target;
        entry.state_transition(mesh_enqueue);
    }
    // aka discover_mesh()
    void mark_mesh_dirty(ChunkEntry& entry, std::string_view reason="N/A"){
        mark_mesh_dirty(&entry);
    }

    void mark_mesh_dirty(ChunkEntry* entry, std::string_view reason="N/A"){
        assert(entry);
        log_to_chunk("mesh_endirtying",entry->coord, 
                     "Mesh dirtied ({}->{}). Reason:{}",
                     entry->mesh_revision.target,
                     entry->mesh_revision.target+1,
                     reason);
        entry->mesh_revision.mark_dirty();
        ready_for_mesh.push(entry->coord);
    }

    std::vector<WorldChunkCoord> find_mesh_jobs(size_t N){
        size_t count = std::min(N,ready_for_mesh.size());
        auto copy = ready_for_mesh.q;
        // 1. partial sort the bottom N elements (OR JUST SORT THEM ALL)
        
        auto nearest_to_player = [&](auto p1, auto p2){
            auto d1 = LM::sq_dist(cur_chunk_pos,p1);
            auto d2 = LM::sq_dist(cur_chunk_pos,p2);
            return d1<d2;
        };
        ranges::sort(copy, nearest_to_player); // TODO: swap with a partial sort of the nearest N elements
        copy.resize(std::min(N,copy.size()));
        return copy;
    }

    // ============
    // Generation
    // ============
    void upload_generated_chunk(GenResult gen_res);

    std::pair<size_t,size_t> discover_candidates(
        ChunkBenchmarkerNoRevision & mesh_enqueue_delay_bench, 
        ChunkBenchmarkerNoRevision & gen_enqueue_delay_bench, 
        i64 max_jobs, i64 gen_radius, i64 mesh_radius);

    std::vector<WorldChunkCoord> find_gen_jobs(size_t N){
        size_t count = std::min(N,ready_for_gen.size());
        return {ready_for_gen.begin(), ready_for_gen.begin()+count};
    }

    inline void mark_neighbours_dirty(WorldChunkCoord key, std::string_view reason="N/A"){
        auto lo = key.raw() + glm::ivec2{-1};
        auto hi = key.raw() + glm::ivec2{2};
        ForEachInRangeEx(lo,hi,[&](i32 x, i32 z){
            chunk_map.entries.if_contains(
                WorldChunkCoord{x,z},
                [&](ChunkEntry& neighbour_entry){
                    if (neighbour_entry.state.gen == PipelineState::done){
                        mark_mesh_dirty(neighbour_entry);
                    }
                }
            );
        });
    }

    void init(WorldFloatPos player_pos){
        start_frame(player_pos);
        end_frame();
    }
    bool player_crossed_chunk_boundary() const noexcept{
        return prev_chunk_pos != cur_chunk_pos;
    }
    bool player_crossed_block_boundary() const noexcept{
        return prev_block_pos != cur_block_pos;
    }

    bool is_chunk_inside_cull_distance(WorldChunkCoord coord, i32 cull_dist)const noexcept{
        auto lo = cur_chunk_pos + ChunkOffset{-cull_dist};
        auto hi = cur_chunk_pos + ChunkOffset{cull_dist};
        return LM::isVecInBounds(coord,lo, hi);
    }
    bool is_chunk_outside_cull_distance(WorldChunkCoord coord, i32 cull_dist)const noexcept{
        return !is_chunk_inside_cull_distance(coord,cull_dist);
    }




    void start_frame(WorldFloatPos player_pos){
        cur_chunk_pos = toWorldChunkCoord(player_pos);
        cur_block_pos = toWorldBlockPos(player_pos);
    }
    void end_frame(){
        prev_block_pos = cur_block_pos;
        prev_chunk_pos = cur_chunk_pos;
    }
private:
    void handle_pending_writes(const WorldChunkCoord chunkCoord, ChunkView srcBlocks, const PendingWriteList& newWriteList);
    void update_neighbour_map(WorldChunkCoord chunkCoord);
    void update_bounding_boxes_map(WorldChunkCoord chunkCoord);
};
