#pragma once 

// facilitates the searching aa
#include "ChunkEntry.hpp"
#include "ChunkMap.hpp"
#include "CoordIteration.hpp"
#include "CoordTypes.hpp"
#include "LM.hpp"
#include "Logger.hpp"
#include <queue>
#include <span>
#include <unordered_set>

// fifo queue which rejects duplicate elements.
// Requires hashable element type 
template<typename T>
struct UniqueQueue{
public:
    using value_type = T;
private:
    std::vector<T> q;
    std::unordered_map<T, std::size_t> q_idx;
public:
    
    void push(T key) noexcept{
        auto it = q_idx.find(key);
        if (it != q_idx.end()){
//            LOG_ERROR("Attempt to push duplicate element onto unique queue");
        }else{
            q_idx.emplace_hint(it,key,q.size());
            q.push_back(key);
        }
    }
    void erase(T victim_key) noexcept{
        auto it = q_idx.find(victim_key);
        if (it == q_idx.end()){
            return;
        }
        const auto old_back_key = q.back();
        const auto victim_pos = it->second;
        if (q.back() != q.at(victim_pos)){
            std::swap(q.back(),q.at(victim_pos));
            q_idx.at(old_back_key) = victim_pos;
            // restore the q_idx mapping of key->pos in queue
        }
        // regardless, we need to erase the mapping from victim_key->victim_pos
        q_idx.erase(it); 
        q.pop_back();
    }

    T&& pop() {
        erase(q.back());
    }
    auto size() const noexcept{
        return q.size();
    }
    auto begin()noexcept{
        return q.rbegin();
    }
    auto end()noexcept{
        return q.rend();
    }
};
struct ChunkDirector{

    ChunkMap& chunk_map;
    WorldChunkCoord prev_chunk_pos{};
    WorldChunkCoord cur_chunk_pos{};
    // pushed to by mark_mesh_dirty()
    UniqueQueue<WorldChunkCoord> ready_for_mesh;
    void upload_generated_chunk(GenResult gen_res);

    void mark_mesh_enqueue(ChunkEntry& entry, std::string_view reason="N/A"){
        // we cant just erase them from the ready for mesh, as a newer revision might be required.
        // Actually, i think we can, because at the moment of enqueue, it is necessarily the newest revision which is being pushed. Any future revisions will incur a push into the ready_for_mesh queue.
        ready_for_mesh.erase(entry.state.coord);
    }

    void mark_mesh_dirty(ChunkEntry& entry, std::string_view reason="N/A"){
        entry.mark_mesh_dirty(reason);
        ready_for_mesh.push(entry.state.coord);
    }

    std::vector<WorldChunkCoord> find_mesh_jobs(std::size_t N){
        std::size_t count = std::min(N,ready_for_mesh.size());
        return {ready_for_mesh.begin(), ready_for_mesh.begin()+count};
    }

    inline void mark_neighbours_dirty(WorldChunkCoord key, std::string_view reason="N/A"){
        auto lo = key.raw() + glm::ivec2{-1};
        auto hi = key.raw() + glm::ivec2{2};
        ForEachInRangeEx(lo,hi,[&](i32 x, i32 z){
            chunk_map.entries.if_contains(
                WorldChunkCoord{x,z},
                [&](ChunkEntry& neighbour_entry){
                    if (neighbour_entry.state.gen == GenState::done){
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
    bool did_player_cross_chunk_boundary(WorldFloatPos new_pos)const noexcept{
        return prev_chunk_pos != toWorldChunkCoord(new_pos);
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
    }
    void end_frame(){
        prev_chunk_pos = cur_chunk_pos;
    }
private:
    void handle_pending_writes(const WorldChunkCoord chunkCoord, ChunkSpan srcBlocks, const PendingWriteList& newWriteList);
    void update_neighbour_map(WorldChunkCoord chunkCoord);
    void update_bounding_boxes_map(WorldChunkCoord chunkCoord);
};
