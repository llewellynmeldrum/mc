#pragma once 

#include <optional>
#include <print>
#include <queue>
#include <range/v3/view/chunk.hpp>
#include <span>
#include <unistd.h>
#include <unordered_set>


#include "BenchmarkMap.hpp"
#include "ChunkConcurrency.hpp"
#include "ChunkConstants.hpp"
#include "ChunkEntry.hpp"
#include "ChunkMap.hpp"
#include "CoordIteration.hpp"
#include "CoordTypes.hpp"
#include "GlobalDebugLog.hpp"
#include "LM.hpp"
#include "Logger.hpp"
#include "FormatSpecs.hpp"
#include "UniqueQueue.hpp"
#include "World.hpp"
#include "cppslop.hpp"
#include "glm/gtx/norm.hpp"

#include "Lighting.hpp"
#include "ChunkGenerator.hpp"
#include "ChunkMesher.hpp"

FORWARD_DECL_STRUCT(Renderer)
struct ChunkDirector{
    // =========
    // Constants
    // =========

    // Maximum number of additions to the ready_for_* queues, per job type. 
    static constexpr i64 max_gen_discovery_pf = 128;
    static constexpr i64 max_mesh_discovery_pf= 128;
    static constexpr i64 max_light_discovery_pf = 128;
    void handle_mesh_sorting(Renderer& rend, WorldFloatPos player_cam_pos);
    void discover_candidates();

    void upload_gen_result(ChunkEntry * entry, GenResult&& gen_res);
    void upload_light_result(ChunkEntry* entry, LightingResult&& res) ;
    void upload_mesh_result(ChunkEntry* entry,Renderer& rend, MeshResult&& res);

    ChunkMap& chunk_map;
    World& world;
    ChunkBenchmarkerNoRevision mesh_enqueue_delay_bencher;
    ChunkBenchmarkerNoRevision gen_enqueue_delay_bencher;
    ChunkBenchmarkerNoRevision light_enqueue_delay_bencher;

    JobProcessor<JobType::Gen, GenJob, GenResult,1 > generators{worker_loop<JobType::Gen>};
    JobProcessor<JobType::Light, LightingJob, LightingResult, 2> lighters{worker_loop<JobType::Light>};
    JobProcessor<JobType::Mesh , MeshJob, MeshResult,7> meshers{worker_loop<JobType::Mesh>};

    // We use intermediary queues on the main thread to avoid scanning over chunks (as much as we can)
    // Gen completion puts a coord on ready_for_light, mesh completion on ready_for_mesh, etc
    UniqueQueue<WorldChunkCoord> ready_for_gen;
    UniqueQueue<WorldChunkCoord> ready_for_light;
    UniqueQueue<WorldChunkCoord> ready_for_mesh;

    template<JobType JT>
    inline auto get_revision(WorldChunkCoord coord){
        if constexpr (JT == JobType::Gen){
            return world.worldgen_epoch;
        }else if constexpr (JT == JobType::Light || JT==JobType::Mesh){
            auto* entry = world.chunkMap.entries.try_get(coord);
            assert(entry);
            return entry->revision_state<JT>().get_candidate_rev();
        }else{
            static_assert(false);
        }
    }
    // pushed to by mark_mesh_dirty()
    template<JobType JT>
    auto& ready_for_enqueue(){
        if constexpr(JT == JobType::Gen){ return ready_for_gen;}
        else if constexpr(JT == JobType::Light){ return ready_for_light;}
        else if constexpr(JT == JobType::Mesh){ return ready_for_mesh;}
        else {static_assert(false);}
    }
    template<JobType JT>
    auto& job_queue(){
        if constexpr(JT == JobType::Gen){ return generators.job_queue;}
        else if constexpr(JT == JobType::Light){ return lighters.job_queue;}
        else if constexpr(JT == JobType::Mesh){ return meshers.job_queue;}
        else {static_assert(false);}
    }
    template<JobType JT>
    auto& res_queue(){
        if constexpr(JT == JobType::Gen){ return generators.res_queue;}
        else if constexpr(JT == JobType::Light){ return lighters.res_queue;}
        else if constexpr(JT == JobType::Mesh){ return meshers.res_queue;}
        else {static_assert(false);}
    }



    template<typename Fn>
        requires return_type_is<bool, Fn, ChunkEntry*>
    bool neighbours_are(ChunkEntry* entry, Fn&& pred){
        for (const auto& neighbour_coord: entry->neighbour_coords4()){
            auto* neighbour_entry = chunk_map.entries.try_get(neighbour_coord);
            if (!neighbour_entry || !pred(neighbour_entry)){
                return false;
            }
        }
        return true;
    }

    bool neighbours_are_generated(ChunkEntry* entry){
        return neighbours_are(entry, [](auto* e){ return e->gen.has_data();});
    }
    bool neighbours_are_lit(ChunkEntry* entry){
        return neighbours_are(entry, [](auto* e){ return e->lighting.has_data();});
    }
    bool neighbours_are_meshed(ChunkEntry* entry){
        return neighbours_are(entry, [](auto* e){ return e->mesh.has_data();});
    }




    template<JobType JT>
    void mark_enqueue(ChunkEntry* entry, std::string_view reason="N/A"){
        log_state<JT>(entry, std::format("enqueue ?: {}",reason));
        ready_for_enqueue<JT>().pop(entry->coord);
        entry->revision_state<JT>().mark_job_inflight();
    }
    void mark_gen_enqueue(ChunkEntry* entry, std::string_view reason="N/A"){
        mark_enqueue<JobType::Gen>(entry,reason);
    }
    void mark_lighting_enqueue(ChunkEntry* entry, std::string_view reason="N/A"){
        mark_enqueue<JobType::Light>(entry,reason);
    }
    void mark_mesh_enqueue(ChunkEntry* entry, std::string_view reason="N/A"){
        mark_enqueue<JobType::Mesh>(entry,reason);
    }

    template<JobType JT>
    void mark_deleted(ChunkEntry* entry, std::string_view reason="N/A"){
        log_state<JT>(entry, std::format("deletion ?: {}",reason));
        ready_for_enqueue<JT>().pop(entry->coord);
        enqueue_delay_bencher<JT>().bench_cancel(entry->coord);
        entry->revision_state<JT>().mark_deleted();
    }
    void mark_lighting_deleted(ChunkEntry* entry, std::string_view reason="N/A"){
        mark_deleted<JobType::Light>(entry,reason);
    }
    void mark_mesh_deleted(ChunkEntry* entry, std::string_view reason="N/A"){
        mark_deleted<JobType::Mesh>(entry,reason);
    }
    void mark_gen_deleted(ChunkEntry* entry, std::string_view reason="N/A"){
        mark_deleted<JobType::Gen>(entry,reason);
    }

    template<JobType JT> ChunkBenchmarkerNoRevision & enqueue_delay_bencher(){
        if constexpr (JT == JobType ::Gen) {
            return gen_enqueue_delay_bencher;
        } else if constexpr (JT == JobType ::Light) {
            return light_enqueue_delay_bencher;
        } else if constexpr (JT == JobType ::Mesh) {
            return mesh_enqueue_delay_bencher;
        } else {
            static_assert(false);
        };
    };
    template<JobType JT>
    void push_ready(WorldChunkCoord coord){
        enqueue_delay_bencher<JT>().bench_start(coord);
        ready_for_enqueue<JT>().push(coord);
    }
    template<JobType JT>
    void mark_dirty(ChunkEntry* entry, std::string_view reason="N/A"){
        assert(entry);
        auto& revision_state = entry->revision_state<JT>();
        auto s = entry->gen.target;
        log_state<JT>(entry, std::format("made dirty ({})->({}) ?:{}",s,s+1,reason));
        push_ready<JT>(entry->coord);
        revision_state.mark_dirty();
    }
    void mark_lighting_dirty(ChunkEntry* entry, std::string_view reason="N/A"){
        mark_dirty<JobType::Light>(entry,reason);
    }
    void mark_mesh_dirty(ChunkEntry* entry, std::string_view reason="N/A"){
        mark_dirty<JobType::Mesh>(entry,reason);
    }
    void mark_gen_dirty(ChunkEntry* entry, std::string_view reason="N/A"){
        mark_dirty<JobType::Gen>(entry,reason);
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



    static constexpr i32 RENDER_DIST = 16;
    static constexpr i32 GENERATION_DIST = RENDER_DIST+2; //controls chunk gen
    static constexpr i32 LIGHTING_DIST = RENDER_DIST+1;
    static constexpr i32 MESH_CULL_DIST(){return RENDER_DIST+2;}
    #define CONSTEXPR_SWITCH(a) if constexpr(true){}
    #define CASE(a, b) else if constexpr(a == b)
    
    template<JobType JT>
    i32 cull_dist(){
        if constexpr (JT == JobType ::Gen) return GENERATION_DIST;
        else if constexpr (JT == JobType ::Light) return LIGHTING_DIST;
        else if constexpr (JT == JobType ::Mesh) return RENDER_DIST;
    }

    // Have the prerequisites external to the chunk been met?
    template<JobType JT>
    i32 external_prereqs_met(ChunkEntry* entry){
        if constexpr (JT == JobType ::Gen) return true;
        else if constexpr (JT == JobType ::Light) return neighbours_are_generated(entry);
        else if constexpr (JT == JobType ::Mesh) return neighbours_are_lit(entry);
        else static_assert(false,"Unknown job type");
    }

    // Have the the INTERNAL prereqs to this stage been met, AND do they actually want to do work?
    template<JobType JT>
    bool wants_work(ChunkEntry* entry){
        if constexpr (JT == JobType ::Gen) return !entry || entry->wants_generation();
        else if constexpr (JT == JobType ::Light) return entry && entry->wants_lighting();
        else if constexpr (JT == JobType ::Mesh) return entry && entry->wants_meshing();
        else static_assert(false,"Unknown job type");
    }


    bool is_chunk_inside_radius(WorldChunkCoord coord, i32 radius)const noexcept{
        auto lo = cur_chunk_pos + ChunkOffset{-radius};
        auto hi = cur_chunk_pos + ChunkOffset{radius};
        return LM::isVecInBounds(coord,lo, hi);
    }
    bool is_chunk_outside_radius(WorldChunkCoord coord, i32 radius)const noexcept{
        return !is_chunk_inside_radius(coord,radius);
    }
    enum struct ReadyFilter{
        drop,   // dropped jobs get removed from ready queues.
        defer,  // deferred jobs are skipped, but left on ready queues.
        take    // taken jobs are used.
    };
    template<typename Fn>
    std::vector<WorldChunkCoord> find_N_nearest_ready(
        UniqueQueue<WorldChunkCoord>& uq,
        size_t N, 
        Fn&& classify
    ){
        auto& q = uq.q;
        std::vector<WorldChunkCoord> eligible;
        for (size_t i = 0; i<q.size();){
            auto coord = q[i];
            auto* entry = chunk_map.entries.try_get(coord);
            switch (classify(coord,entry)){
                case ReadyFilter::drop: uq.pop(coord); break;
                case ReadyFilter::defer: i++; break;
                case ReadyFilter::take: eligible.push_back(coord); i++; break;
            }
        }
        size_t count = std::min(N,eligible.size());
        auto nearest_to_player = [&](auto p1, auto p2){
            return LM::sq_dist(cur_chunk_pos,p1) < LM::sq_dist(cur_chunk_pos,p2);
        };
        ranges::partial_sort(eligible, eligible.begin()+count, nearest_to_player); 
        eligible.resize(count);
        return eligible;
    }


    template<JobType JT>
    bool qualifies_for_work(ChunkEntry* entry){
        if constexpr(JT == JobType::Gen) return qualifies_for_gen_work(entry);
        else if constexpr(JT == JobType::Light) return qualifies_for_light_work(entry);
        else if constexpr(JT == JobType::Mesh) return qualifies_for_mesh_work(entry);
        else static_assert(false); 
    }
    bool qualifies_for_gen_work(ChunkEntry* entry){
        return (!entry)
              || entry->wants_generation();
    }
    bool qualifies_for_light_work(ChunkEntry* entry){
        // Neighbour slices are read unconditionally, therefore they must be generated first
        return entry 
            && entry->wants_lighting() 
            && neighbours_are_generated(entry);
    }

    bool qualifies_for_mesh_work(ChunkEntry* entry){
        return entry 
            && entry->wants_meshing()
            && neighbours_are_lit(entry);
    }


    template<JobType JT>
    auto find_jobs(size_t N){
        if constexpr (JT == JobType::Gen){
            size_t count = std::min(N,ready_for_gen.size());
            auto res = ready_for_gen.q;
            auto nearest_to_player = [&](auto p1, auto p2){
                return LM::sq_dist(cur_chunk_pos,p1) < LM::sq_dist(cur_chunk_pos,p2);
            };
            ranges::partial_sort(res, res.begin()+count, nearest_to_player); 
            res.resize(count);
            return res;
        }else{
            auto res = find_N_nearest_ready( 
                ready_for_enqueue<JT>(), 
                N,
                [&](WorldChunkCoord coord, ChunkEntry* entry){
                    if (!entry) return ReadyFilter::drop;
                    else if (entry->revision_state<JT>().is_clean()) return ReadyFilter::drop;
                    else if (is_chunk_outside_radius(coord, cull_dist<JT>())) return ReadyFilter::drop;

                    if constexpr(JT == JobType::Light){
                        return qualifies_for_light_work(entry) ? ReadyFilter::take : ReadyFilter::defer;
                    }else if constexpr (JT == JobType::Mesh){
                        return qualifies_for_mesh_work(entry) ? ReadyFilter::take : ReadyFilter::defer;
                    }else {
                        static_assert(false, "Unknown job type, or Gen reached here. It has different conditions");
                    }
                }
            );
            log_to_ui("Found {}/{} jobs for {}",res.size(),N,JT);
            return res;
        }
    }

    inline void mark_neighbour_lights_dirty(WorldChunkCoord key, std::string_view reason="N/A"){
        auto* entry = chunk_map.entries.try_get(key);
        if (!entry) return;
        for (const auto& neighbour_coord : entry->neighbour_coords4()){
            chunk_map.entries.if_contains(neighbour_coord, [&](ChunkEntry& entry){
                mark_lighting_dirty(&entry,"Neighbour is newly lit");
            });
        }
    }
    inline void mark_neighbour_meshes_dirty(WorldChunkCoord key, std::string_view reason="N/A"){
        auto* entry = chunk_map.entries.try_get(key);
        if (!entry) return;
        for (const auto& neighbour_coord : entry->neighbour_coords4()){
            chunk_map.entries.if_contains(neighbour_coord, [&](ChunkEntry& entry){
                mark_mesh_dirty(&entry,"Neighbour is newly lit");
            });
        }
    }
    bool place_block(WorldBlockPos wpos, BlockType block){
        auto* chunk = chunk_map.entries.try_get(toWorldChunkCoord(wpos));
        if (chunk){
            ChunkBlockPos cpos = toChunkBlockPos(wpos);
            chunk->block_data.at(cpos) = block;
            mark_lighting_dirty(chunk, "Block placed");
            mark_mesh_dirty(chunk, "Block placed");
            return true;
        }
        return false;
    }
    std::optional<Block> block_at(WorldBlockPos wpos)const noexcept{
        assert_geq(wpos.y, 0);
        assert_leq(wpos.y, WORLD_YMAX-1);
        auto* chunk = chunk_map.entries.try_get(toWorldChunkCoord(wpos));
        if (chunk){
            ChunkBlockPos cpos = toChunkBlockPos(wpos);
            return std::make_optional(chunk->block_data.at(cpos));
        }
        return std::nullopt;
    }

    template<JobType JT>constexpr static auto thread_type();
    template<> constexpr auto thread_type<JobType::Mesh>(){return ThreadType::mesh;}
    template<> constexpr auto thread_type<JobType::Gen>(){return ThreadType::gen;}
    template<> constexpr auto thread_type<JobType::Light>(){return ThreadType::light;}

    template<JobType JT>
    static constexpr res_struct_t<JT>  perform_job_work(job_struct_t<JT>&& job){
        if constexpr(JT == JobType::Gen) return perform_gen_work(std::move(job));
        else if constexpr(JT == JobType::Light) return perform_light_work(std::move(job));
        else if constexpr(JT == JobType::Mesh) return perform_mesh_work(std::move(job));
        else static_assert(false); 
    }
    template<JobType JT>
    inline static void worker_loop(std::stop_token stop_token, Queue<job_struct_t<JT>>& in_queue, Queue<res_struct_t<JT>>& out_queue){
        ThreadTracker::assign_my_thread_type(thread_type<JT>());
        while (!stop_token.stop_requested()){
            auto job = in_queue.wait_dequeue();
            auto coord = job.coord;
            auto rev = job.rev;
            job.bench.job_idle.bench_end(job.coord,job.rev);

            job.bench.work.bench_start(coord,rev);
            auto res = perform_job_work<JT>(std::move(job));
            job.bench.work.bench_end(coord,rev);

            job.bench.res_idle.bench_start(coord,rev);
            out_queue.wait_emplace(std::move(res));
        }

    }
    void setup(WorldFloatPos player_pos){
        start_frame(player_pos);
        end_frame();
        lighters.launch_threads();
        generators.launch_threads();
        meshers.launch_threads();
    }

    bool player_crossed_chunk_boundary() const noexcept{ return prev_chunk_pos != cur_chunk_pos; }
    bool player_crossed_block_boundary() const noexcept{ return prev_block_pos != cur_block_pos; }

    void start_frame(WorldFloatPos player_pos)noexcept {
        cur_chunk_pos = toWorldChunkCoord(player_pos);
        cur_block_pos = toWorldBlockPos(player_pos);
    }
    void end_frame()noexcept {
        prev_block_pos = cur_block_pos;
        prev_chunk_pos = cur_chunk_pos;
    }
    WorldBlockPos prev_block_pos{};
    WorldBlockPos cur_block_pos{};
    WorldChunkCoord prev_chunk_pos{};
    WorldChunkCoord cur_chunk_pos{};

    // SECTION: LOGGING HELPER FUNCTIONS

    template<JobType JT>
    void log_state(ChunkEntry* entry, std::string_view msg){
        assert(entry);
        if constexpr(JT == JobType::Gen){
            log_to_chunk(LogType::gen_state, entry->coord, "G: {}",msg);
        } else if constexpr(JT == JobType::Light){ 
            log_to_chunk(LogType::lighting_state, entry->coord, "L: {}",msg);
        }else if constexpr(JT == JobType::Mesh){ 
            log_to_chunk(LogType::mesh_state, entry->coord, "M: {}",msg);
        }
        else {static_assert(false);}
    }
    void log_lighting_state(ChunkEntry* entry, std::string_view msg){
        log_state<JobType::Light>(entry,msg);
    }
    void log_mesh_state(ChunkEntry* entry, std::string_view msg){
        log_state<JobType::Mesh>(entry,msg);
    }
    void log_gen_state(ChunkEntry* entry, std::string_view msg){
        log_state<JobType::Gen>(entry,msg);
    }
private:
    void handle_pending_writes(WorldChunkCoord chunkCoord, ChunkBlockView srcBlocks, const PendingWriteList& newWriteList);
};
