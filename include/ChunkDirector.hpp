#pragma once 

#include <optional>
#include <print>
#include <queue>
#include <range/v3/view/chunk.hpp>
#include <span>
#include <unistd.h>
#include <unordered_set>


#include "ChunkConcurrency.hpp"
#include "ChunkEntry.hpp"
#include "ChunkMap.hpp"
#include "CoordIteration.hpp"
#include "CoordTypes.hpp"
#include "GlobalDebugLog.hpp"
#include "LM.hpp"
#include "Logger.hpp"
#include "FormatSpecs.hpp"
#include "UniqueQueue.hpp"
#include "cppslop.hpp"
#include "glm/gtx/norm.hpp"

FORWARD_DECL_STRUCT(Renderer)
struct ChunkDirector{

    ChunkMap& chunk_map;
    // pushed to by mark_mesh_dirty()
    UniqueQueue<WorldChunkCoord> ready_for_gen;
    UniqueQueue<WorldChunkCoord> ready_for_lighting;
    UniqueQueue<WorldChunkCoord> ready_for_mesh;


    void handle_mesh_sorting(Renderer& rend, WorldFloatPos player_cam_pos);

    bool neighbours_are_generated(ChunkEntry* entry){
        for (const auto& neighbour_coord: entry->neighbour_coords4()){
            auto* neighbour_entry = chunk_map.entries.try_get(neighbour_coord);
            if (!neighbour_entry){
                return false;
            }
            if (!neighbour_entry->gen.has_data()){
                return false;
            }
        }
        return true;
    }
    bool neighbours_are_lit(ChunkEntry* entry){
        for (const auto& neighbour_coord: entry->neighbour_coords4()){
            auto* neighbour_entry = chunk_map.entries.try_get(neighbour_coord);
            if (!neighbour_entry){
                return false;
            }
            if (!neighbour_entry->lighting.has_data()){
                return false;
            }
        }
        return true;
    }
    bool qualifies_for_mesh_enqueue(ChunkEntry* entry){
        return (entry && entry->can_be_meshed() && neighbours_are_lit(entry));
    }
    bool qualifies_for_light_work(ChunkEntry* entry){
        return (entry && entry->can_be_lit() && neighbours_are_generated(entry));
    }

    // SECTION: LOGGING HELPER FUNCTIONS
    void log_lighting_state(ChunkEntry* entry, std::string_view msg){
        assert(entry);
        log_to_chunk(LogType::lighting_state, entry->coord, "L: {}",msg);
    }
    void log_mesh_state(ChunkEntry* entry, std::string_view msg){
        assert(entry);
        log_to_chunk(LogType::mesh_state, entry->coord, "M: {}",msg);
    }
    void log_gen_state(ChunkEntry* entry, std::string_view msg){
        assert(entry);
        log_to_chunk(LogType::gen_state, entry->coord, "G: {}",msg);
    }


    // SECTION: MARK ENQUEUE
    void mark_gen_enqueue(ChunkEntry* entry, std::string_view reason="N/A"){
        log_gen_state(entry, std::format("enqueue ?: {}",reason));

        ready_for_gen.pop(entry->coord);
        entry->gen.mark_job_inflight();
    }
    void mark_lighting_enqueue(ChunkEntry* entry, std::string_view reason="N/A"){
        log_lighting_state(entry, std::format("enqueue ?: {}",reason));

        ready_for_lighting.pop(entry->coord);
        entry->lighting.mark_job_inflight();
    }
    void mark_mesh_enqueue(ChunkEntry* entry, std::string_view reason="N/A"){
        log_mesh_state(entry, std::format("enqueue ?: {}",reason));

        ready_for_mesh.pop(entry->coord);
        entry->mesh.mark_job_inflight();
    }

    // SECTION: MARK DELETED
    void mark_lighting_deleted(ChunkEntry* entry, std::string_view reason="N/A"){
        log_lighting_state(entry, std::format("deletion ?: {}",reason));

        ready_for_lighting.pop(entry->coord);
        entry->lighting.mark_deleted();
    }

    void mark_mesh_deleted(ChunkEntry* entry, std::string_view reason="N/A"){
        log_mesh_state(entry, std::format("deletion ?: {}",reason));
        ready_for_mesh.pop(entry->coord);
        entry->mesh.mark_deleted();
    }
    void mark_gen_deleted(ChunkEntry* entry, std::string_view reason="N/A"){
        log_gen_state(entry, std::format("deletion ?: {}",reason));

        ready_for_gen.pop(entry->coord);
        entry->gen.mark_deleted();
    }

    // SECTION: MARK DIRTY
    void mark_gen_dirty(ChunkEntry* entry, std::string_view reason="N/A"){
        auto s = entry->gen.target;
        log_gen_state(entry, std::format("made dirty ({})->({}) ?:{}",s,s+1,reason));

        ready_for_gen.push(entry->coord);
        entry->gen.mark_dirty();
    }

    void mark_lighting_dirty(ChunkEntry* entry, std::string_view reason="N/A"){
        auto s = entry->lighting.target;
        log_lighting_state(entry, std::format("made dirty ({})->({}) ?:{}",s,s+1,reason));

        ready_for_lighting.push(entry->coord);
        entry->lighting.mark_dirty();
    }

    void mark_mesh_dirty(ChunkEntry* entry, std::string_view reason="N/A"){
        auto s = entry->mesh.target;
        log_mesh_state(entry, std::format("made dirty ({})->({}) ?:{}",s,s+1,reason));

        ready_for_mesh.push(entry->coord);
        entry->mesh.mark_dirty();
    }

    // NOTE: on mark_dirty vs deletion.
    // NOTE: 
    // Deletion means: 
    // -> return to the original state, before any work was done. 
    // -> Pop from any intermediary queues
    //      (Deletion is what we do to a mesh/chunk/lighting when we are sufficiently far enough away.)
    //
    // NOTE: 
    // mark_dirty means: 
    // -> some change was made to the underlying data, and we must now enqueue a job,
    //    with the new data, in order to produce a result which reflects the underlying data correctly.



    // =========
    // Generation
    // =========
    i32 RENDER_DIST = 16;
    i32 LIGHTING_DIST = 8;
    i32 GENERATION_DIST = RENDER_DIST+2; //controls chunk gen
    i32 MESH_CULL_DIST(){return RENDER_DIST+2;}


    bool is_chunk_inside_radius(WorldChunkCoord coord, i32 radius)const noexcept{
        auto lo = cur_chunk_pos + ChunkOffset{-radius};
        auto hi = cur_chunk_pos + ChunkOffset{radius};
        return LM::isVecInBounds(coord,lo, hi);
    }
    bool is_chunk_outside_radius(WorldChunkCoord coord, i32 radius)const noexcept{
        return !is_chunk_inside_radius(coord,radius);
    }
    template<typename Fn>
    std::vector<WorldChunkCoord> find_N_nearest_ready(UniqueQueue<WorldChunkCoord>& uq, size_t N, Fn&& skip_entry_if){
        auto& q = uq.q;
        for (size_t i = 0; i<q.size();){
            auto coord = q[i];
            auto* entry = chunk_map.entries.try_get(coord);
            auto should_skip =  !entry || skip_entry_if(coord,entry);
            if (should_skip){
                uq.pop(coord);
            }else{
                i++;
            }
        }
        size_t count = std::min(N,uq.size());
        auto copy = q;
        // 1. partial sort the bottom N elements 
        auto nearest_to_player = [&](auto p1, auto p2){
            return LM::sq_dist(cur_chunk_pos,p1) < LM::sq_dist(cur_chunk_pos,p2);
        };
        ranges::partial_sort(copy, copy.begin()+count, nearest_to_player); 
        copy.resize(count);
        return copy;
    }
    std::vector<WorldChunkCoord> find_mesh_jobs(size_t N){
        return find_N_nearest_ready(
            ready_for_mesh,
            N,
            [&](WorldChunkCoord coord, ChunkEntry* entry){
                return is_chunk_outside_radius(coord, MESH_CULL_DIST())
                || entry->mesh.is_clean();
            }
        );
    }
    std::vector<WorldChunkCoord> find_lighting_jobs(size_t N){
        auto s = find_N_nearest_ready(
            ready_for_lighting,
            N,
            [&](WorldChunkCoord coord, ChunkEntry* entry){
                return is_chunk_outside_radius(coord, LIGHTING_DIST)
                || entry->lighting.is_clean();
            }
        );
        if (s.size() != 0 || ready_for_lighting.size() !=0)
            LOG_DEBUG("found {}/{} lighting jobs", s.size(),ready_for_lighting.size());
        return s;
    }

    // ============
    // Generation
    // ============
    void upload_generated_chunk(GenResult gen_res);

    void discover_candidates(ChunkBenchmarkerNoRevision & mesh_enqueue_delay_bench, ChunkBenchmarkerNoRevision & gen_enqueue_delay_bench);

    static constexpr i64 max_gen_discovery_pf= 128;
    static constexpr i64 max_mesh_discovery_pf= 128;
    static constexpr i64 max_light_discovery_pf = 128;

    std::vector<WorldChunkCoord> find_gen_jobs(size_t N){
        size_t count = std::min(N,ready_for_gen.size());
        return {ready_for_gen.begin(), ready_for_gen.begin()+count};
    }

    inline void mark_neighbour_meshes_dirty(WorldChunkCoord key, std::string_view reason="N/A"){
        auto* entry = chunk_map.entries.try_get(key);
        if (!entry) return;
        for (const auto& neighbour_coord : entry->neighbour_coords4()){
            auto* neighbour = chunk_map.entries.try_get(neighbour_coord);
            mark_mesh_dirty(neighbour,"Neighbour is newly generated");
        }
    }
    bool place_block(WorldBlockPos wpos, BlockType block){
        auto* chunk = chunk_map.entries.try_get(toWorldChunkCoord(wpos));
        std::println("chunk:{}",toWorldChunkCoord(wpos));
        if (chunk){
            ChunkBlockPos cpos = toChunkBlockPos(wpos);
            std::println("cpos:{}",cpos);
            chunk->block_data.at(cpos) = block;
            mark_lighting_dirty(chunk);
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

    void start_frame(WorldFloatPos player_pos){
        cur_chunk_pos = toWorldChunkCoord(player_pos);
        cur_block_pos = toWorldBlockPos(player_pos);
    }
    void end_frame(){
        prev_block_pos = cur_block_pos;
        prev_chunk_pos = cur_chunk_pos;
    }
    WorldBlockPos prev_block_pos{};
    WorldBlockPos cur_block_pos{};
    WorldChunkCoord prev_chunk_pos{};
    WorldChunkCoord cur_chunk_pos{};
private:
    void handle_pending_writes(const WorldChunkCoord chunkCoord, ChunkBlockView srcBlocks, const PendingWriteList& newWriteList);
};
