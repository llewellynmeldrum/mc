#pragma once

#include <numbers>
#include <optional>
#include <print>
#include <utility>

#include "Camera.hpp"
#include "ChunkConcurrency.hpp"
#include "FmtStyle.hpp"
#include "MirroredRingBuf.hpp"
#include "Timer.hpp"
#include "Types.hpp"
#include "Window.hpp"
#include "BenchmarkMap.hpp"
#include "Input.hpp"
#include "DebugUI.hpp"
#include "Renderer.hpp"
#include "WorldGen_Config.hpp"
#include "cppslop.hpp"
#include "ChunkDirector.hpp"
#include "ChunkMesher.hpp"
#include "ChunkGenerator.hpp"
#include "ProgressBar.hpp"
#include "SkyboxState.hpp"

#include "World.hpp"
#include "Concurrency.hpp"

// src/Simulation.cpp
using namespace std::chrono_literals;
struct Engine {
    static constexpr auto default_skybox_cfg(TickCount tick_count){
        return SkyboxConfig{
            .ticks_per_day = ticks_per_day,
            .tick_count = tick_count,
        };
    }
  public:
    Engine() : 
        win(), 
        profiler(),
        input(win.ptr), 
        player_cam({-56.181,+135.793,-62.740}, +15.588,+185.125),
        drone_cam(),
        rend(),
        fixedCamTarget({0,0},{640,480}),
        ui(default_skybox_cfg(ticks_per_day* 0.841)),
        world(default_world_seed),
        director(world.chunkMap,world)
    {}
    ~Engine() = default;

    static constexpr auto ticks_per_day = TickCount{20 * 60 * 24};// 24 minute day/night cycle at 20tps
    
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
    template<JobType JT>
    std::optional<RevisionState::ID> enqueue_job_type(WorldChunkCoord coord){
        auto& q = job_queue<JT>();
        auto job = make_job<JT>(coord);
        auto rev = job.rev;
        job_idle_bencher<JT>().bench_start(coord,rev);
        if (!q.try_enqueue(job)){
            return std::nullopt;
        }
        auto* entry = world.chunkMap.entries.try_get(coord);
        if constexpr (JT == JobType::Gen){
            if (!entry) entry = world.make_chunk_entry(coord);
        } else if constexpr (JT == JobType::Light){
            assert(entry);
            assert(entry->light_data.read().buf.size() == ChunkInfo::SIZE);
        }else if constexpr(JT == JobType::Mesh){
            assert(entry);

        }else {
            static_assert(false);
        }
        director.mark_enqueue<JT>(entry);
        return std::make_optional(rev);
    }
    template<JobType JT>
    bool upload(res_struct_t<JT> && res){
        if constexpr (JT == JobType::Gen){
            return upload_gen(std::move(res));
        }else if constexpr (JT == JobType::Light){
            return upload_light(std::move(res));
        }else if constexpr (JT == JobType::Mesh){
            return upload_mesh(std::move(res));
        }else{
            static_assert(false);
        }
    }

    MeshJob make_mesh_job(WorldChunkCoord coord);
    LightingJob make_light_job(WorldChunkCoord coord);
    GenJob make_gen_job(WorldChunkCoord coord);
    template<JobType JT>
    job_struct_t<JT> make_job(WorldChunkCoord coord){
        if constexpr (JT == JobType::Gen){
            return make_gen_job(std::move(coord));
        }else if constexpr (JT == JobType::Light){
            return make_light_job(std::move(coord));
        }else if constexpr (JT == JobType::Mesh){
            return make_mesh_job(std::move(coord));
        }else{
            static_assert(false);
        }
    }


    bool upload_mesh(MeshResult&& res);
    bool upload_light(LightingResult&& res);
    bool upload_gen(GenResult&& res);



    // tick machinery
    TickCount ticks_per_second {200uz};
    TickCount tick_count = ticks_per_day * 0.20;
    timer::duration tick_gap_accumulator = timer::duration{0};
    timer::time_point t_frame_start = timer::now();
    constexpr auto msPerTick(){return timer::milliseconds(1000.0 / ticks_per_second);}

    constexpr static auto maxGapContributionPerFrame = timer::milliseconds(250.0); // limit on how many 'lagging' ticks are CREATED
    constexpr static auto max_ticks_per_frame = 8uz; // limit on how many 'lagging' ticks are ACCEPTED
    
    void perform_tick_updates(timer::duration dt); // called once per frame
    void per_tick_update(); // called [1,8] times per frame
    

    
    void setup(bool log_setup_stages=false);
    i32 exit(i32 exit_code);

    void loop();
    
    template<JobType JT>
    constexpr ChunkBenchContext make_bench_ctx(){
        return ChunkBenchContext{
            work_bencher<JT>(),
            job_idle_bencher<JT>(),
            res_queue_idle_bencher<JT>()
        };
    }


    Window   win;
    FrameProfiler profiler;
    // TODO: Make these templated on job type

    #define jt_matcher(name)                                                   \
    if constexpr (JT == JobType::Gen){                                      \
        return gen_##name;                                  \
    }else if constexpr (JT == JobType::Light){                              \
        return light_##name;                                \
    }else if constexpr (JT == JobType::Mesh){                               \
        return mesh_##name;                                 \
    }else{                                                                  \
        static_assert(false);                                               \
    }
    // Measures the time from enqueue to upload (enqueue->work->upload)
    // start = after job enqueue (main thread)
    // end = after res deque + upload (main thread)
    ChunkBenchmarker mesh_rtt_bencher;
    ChunkBenchmarker gen_rtt_bencher;
    ChunkBenchmarker light_rtt_bencher;

    // measures the time spent actually working
    // start = after job dequeue (worker thread)
    // end = before res enqueue + upload (worker thread)
    ConcurrentChunkBenchmarker mesh_work_bencher;
    ConcurrentChunkBenchmarker gen_work_bencher;
    ConcurrentChunkBenchmarker light_work_bencher;
    // include/ChunkConcurrency.hpp


    // Measures the time spent in a valid state before being enqueued. 
    // start = after adding to ready_for_mesh/ready_for_gen
    // end = after enqueue


    // Measures the time spent idle on the job queue.
    // start = job enqueue  (on main thread)
    // end = job dequeue    (on worker thread)
    ConcurrentChunkBenchmarker mesh_job_queue_idle_bencher; // BUG: Disabled due to bug
    ConcurrentChunkBenchmarker  gen_job_queue_idle_bencher;
    ConcurrentChunkBenchmarker  light_job_queue_idle_bencher;
    // aka bench.job_idle in job structs


    // Measures the time spent idle on the result queue.
    // start = res enqueue  (on worker thread)
    // end = res dequeue    (on main thread)
    ConcurrentChunkBenchmarker mesh_res_queue_idle_bencher;
    ConcurrentChunkBenchmarker  gen_res_queue_idle_bencher;
    ConcurrentChunkBenchmarker  light_res_queue_idle_bencher;

    template<JobType JT> ChunkBenchmarker & rtt_bencher(){ jt_matcher(rtt_bencher); }
    template<JobType JT> ConcurrentChunkBenchmarker& work_bencher(){ jt_matcher(work_bencher); }

    template<JobType JT> ChunkBenchmarkerNoRevision & enqueue_delay_bencher(){ return director.enqueue_delay_bencher<JT>();}
    template<JobType JT> ConcurrentChunkBenchmarker & job_idle_bencher(){ jt_matcher(job_queue_idle_bencher); }

    // Measures the time spent idle on the result queue.
    // start = res enqueue  (on worker thread)
    // end = res dequeue    (on main thread)
    template<JobType JT> ConcurrentChunkBenchmarker & res_queue_idle_bencher(){ jt_matcher(res_queue_idle_bencher); }

    // aka bench.res_idle in job structs

    Input    input;
    Camera   player_cam;
    Camera   drone_cam;
    Renderer rend;
    TextureTarget fixedCamTarget;
    DebugUI  ui;
    World    world;
    ChunkDirector director;


    template<JobType JT>
    auto drain(){
        discard_results<JT>();
    }
    void regenerate_world();
    void remesh_world();
    void set_debug_params();
    SkyboxConfig get_skybox_cfg();
    void handle_input();
    void refresh_visible_chunks();
    void classify_visible_chunks();



    template<JobType JT>
    static constexpr i64 max_enqueue_per_frame{};
    template<> constexpr auto max_enqueue_per_frame<JobType::Mesh> = 32;
    template<> constexpr auto max_enqueue_per_frame<JobType::Light> =32;
    template<> constexpr auto max_enqueue_per_frame<JobType::Gen> = 32;

    template<JobType JT>
    static constexpr i64 max_upload_per_frame{};
    template<> constexpr auto max_upload_per_frame<JobType::Mesh> = 32;
    template<> constexpr auto max_upload_per_frame<JobType::Light> = 32;
    template<> constexpr auto max_upload_per_frame<JobType::Gen> = 32;


    template<JobType JT>
    static constexpr i64 max_dq_attempts_per_frame{};
    template<> constexpr auto max_dq_attempts_per_frame<JobType::Mesh> = 8;
    template<> constexpr auto max_dq_attempts_per_frame<JobType::Light> = 8;
    template<> constexpr auto max_dq_attempts_per_frame<JobType::Gen> = 8;
    

    template<JobType JT>
    static constexpr std::string_view enqueue_profiler_name{};
    template<> constexpr auto enqueue_profiler_name<JobType::Mesh> = "enqueueMesh";
    template<> constexpr auto enqueue_profiler_name<JobType::Light> = "enqueueLight";
    template<> constexpr auto enqueue_profiler_name<JobType::Gen> = "enqueueGen";

    template<JobType JT>
    static constexpr std::string_view find_jobs_profiler_name{};
    template<> constexpr auto find_jobs_profiler_name<JobType::Mesh> =  "findJobsMesh";
    template<> constexpr auto find_jobs_profiler_name<JobType::Light> = "findJobsLight";
    template<> constexpr auto find_jobs_profiler_name<JobType::Gen> =   "findJobsGen";

    template<JobType JT>
    static constexpr std::string_view upload_profiler_name{};
    template<> constexpr auto upload_profiler_name<JobType::Mesh> = "drainMesh";
    template<> constexpr auto upload_profiler_name<JobType::Light> = "drainLight";
    template<> constexpr auto upload_profiler_name<JobType::Gen> = "drainGen";


    template<JobType JT>
    constexpr auto& res_queue(){static_assert(false);}
    template<> auto& res_queue<JobType::Gen>(){return director.generators.res_queue;}
    template<> auto& res_queue<JobType::Light>(){return director.lighters.res_queue;}
    template<> auto& res_queue<JobType::Mesh>(){return director.meshers.res_queue;}

    template<JobType JT>
    constexpr auto& job_queue(){static_assert(false);}
    template<> auto& job_queue<JobType::Gen>(){return director.generators.job_queue;}
    template<> auto& job_queue<JobType::Light>(){return director.lighters.job_queue;}
    template<> auto& job_queue<JobType::Mesh>(){return director.meshers.job_queue;}

    RenderTargetView screen_view();
    RenderTargetView secondaryView();
    auto construct_mesh_job(WorldChunkCoord candidateCoord);
    bool is_chunk_in_frustum(const Frustum& frustum, WorldChunkCoord coord)const;
    void evict_meshes_outside_radius(i32 radius);

    void update_player_cam(Camera& player_cam);
    void update_drone_cam(Camera& drone_cam, WorldFloatPos target_pos, f32 fly_height=100.0f);

    template<JobType JT>
    void discard_results(){
        using namespace std::chrono_literals;
        auto& c = get_state_counters<JT>();
        update_state_counters<JT>();
        while (c.n_on_queue>0){
            for (auto& res: drain_queue<JT>(128)){
                ChunkEntry* entry = director.chunk_map.entries.try_get(res.coord);
                if (entry){
                    entry->revision_state<JT>().drop_inflight();
                }
            }
            update_state_counters<JT>();
//            std::println(stderr, "==============");
//            std::println(stderr, "discarding {}",JT);
//            std::println(stderr, "==============");
//            std::println(stderr, "ready4Q:{}", director.ready_for_enqueue<JT>().size());
//            std::println(stderr, "n_pending:{}", c.n_pending);
//            std::println(stderr, "n_ready:{}", c.n_ready_for_enqueue);
//            std::println(stderr, "n_on_queue:{}", c.n_on_queue);
//            std::println(stderr, "n_done:{}", c.n_done);
            std::this_thread::sleep_for(5ms);
        }
    };
    ProgressBar<i32> gen_bake_progress{"gen_bake_progress"};
    ProgressBar<i32> light_bake_progress{"light_bake_progress"};
    ProgressBar<i32> mesh_bake_progress{"mesh_bake_progress"};
    template<JobType JT, typename Fn>
    void render_busy_wait_until (i32 count, Fn&& pred){
        using namespace std::chrono_literals;
        update_state_counters<JT>();
        while (!pred()){
            enqueue_jobs<JT>(count);
            upload_results<JT>(count);
            update_state_counters<JT>();
            auto& c = get_state_counters<JT>();
            draw_scene(); 
            ui.draw();
            win.swapBuffers();
            input.poll();
            handle_input();
            if (!baking_starting_chunks) return;
            std::this_thread::sleep_for(25ms);
        }
    };
    void bake_n_chunks(i32 count);
    void handle_chunk_scheduling();
    void draw_scene();
    void draw_chunk_boundaries(Camera& cam, RenderTargetView target );
    bool paused{false};
    bool mouse_mode{false};
    bool chunk_updates_paused{false};
    bool tick_updates_paused{true};

    bool baking_starting_chunks{false};

    // =========
    // telemetry
    // =========
    static constexpr i64 RB_SZ = 256;
    i64 chunksMeshed{0};
    i64 chunksLit{0};
    MirroredRingBuf<f32, RB_SZ> rb_genJobsAdded;
    MirroredRingBuf<f32, RB_SZ> rb_genResultsAdded;

    MirroredRingBuf<f32, RB_SZ> rb_meshJobsAdded;
    MirroredRingBuf<f32, RB_SZ> rb_meshResultsAdded;

    MirroredRingBuf<f32, RB_SZ> rb_lightJobsAdded;
    MirroredRingBuf<f32, RB_SZ> rb_lightResultsAdded;

    MirroredRingBuf<f32, RB_SZ> rb_generating;
    MirroredRingBuf<f32, RB_SZ> rb_meshing;
    MirroredRingBuf<f32, RB_SZ> rb_lighting;


    template<JobType JT>
    constexpr MirroredRingBuf<f32,RB_SZ>& rb_on_queue(){static_assert(false, "Unknown job type");}
    template<> constexpr MirroredRingBuf<f32,RB_SZ>& rb_on_queue<JobType::Gen>(){ return rb_generating; }
    template<> constexpr MirroredRingBuf<f32,RB_SZ>& rb_on_queue<JobType::Light>(){ return rb_lighting; }
    template<> constexpr MirroredRingBuf<f32,RB_SZ>& rb_on_queue<JobType::Mesh>(){ return rb_meshing; }

    template<JobType JT>
    constexpr MirroredRingBuf<f32,RB_SZ>& rb_enqueues(){ static_assert(false, "Unknown job type");}
    template<> constexpr MirroredRingBuf<f32,RB_SZ>& rb_enqueues<JobType::Gen>(){ return rb_genJobsAdded; }
    template<> constexpr MirroredRingBuf<f32,RB_SZ>& rb_enqueues<JobType::Light>(){ return rb_lightJobsAdded; }
    template<> constexpr MirroredRingBuf<f32,RB_SZ>& rb_enqueues<JobType::Mesh>(){ return rb_meshJobsAdded; }
    template<JobType JT>
    constexpr MirroredRingBuf<f32,RB_SZ>& rb_uploads(){ static_assert(false, "Unknown job type");}
    template<> constexpr MirroredRingBuf<f32,RB_SZ>& rb_uploads<JobType::Gen>(){ return rb_genResultsAdded; }
    template<> constexpr MirroredRingBuf<f32,RB_SZ>& rb_uploads<JobType::Light>(){ return rb_lightResultsAdded; }
    template<> constexpr MirroredRingBuf<f32,RB_SZ>& rb_uploads<JobType::Mesh>(){ return rb_meshResultsAdded; }


    template<JobType JT>
    struct StateCounter{
        i64 n_pending{};
        i64 n_ready_for_enqueue{};
        i64 n_on_queue{};
        i64 n_done{};
        void reset(){
            n_pending=0;
            n_ready_for_enqueue=0;
            n_on_queue=0;
            n_done=0;
        };
    };
    StateCounter<JobType::Gen> gen_counters;
    StateCounter<JobType::Light> light_counters;
    StateCounter<JobType::Mesh> mesh_counters;

    template<JobType JT>
    struct PFAccumulators{
        i64 enqueues_this_frame{};
        i64 dequeues_this_frame{};
        i64 uploads_this_frame{};
        void reset(){
            enqueues_this_frame=0;
            dequeues_this_frame=0;
            uploads_this_frame=0;
        };
    };
    PFAccumulators<JobType::Gen> gen_accum_pf;
    PFAccumulators<JobType::Light> light_accum_pf;
    PFAccumulators<JobType::Mesh> mesh_accum_pf;

    template<JobType JT>
    StateCounter<JT> & get_state_counters(){
        if constexpr (JT == JobType::Gen){
            return gen_counters;
        }else if constexpr (JT == JobType::Light){
            return light_counters;
        }else if constexpr (JT == JobType::Mesh){
            return mesh_counters;
        }else{
            static_assert(false);
        }
    }
    template<JobType JT>
    PFAccumulators<JT>& get_pf_accums(){
        if constexpr (JT == JobType::Gen){
            return gen_accum_pf;
        }else if constexpr (JT == JobType::Light){
            return light_accum_pf;
        }else if constexpr (JT == JobType::Mesh){
            return mesh_accum_pf;
        }else{
            static_assert(false);
        }
    }
    //template<JobType JT>
    //struct TotalAccumulator{
    //    i64 enqueues{};
    //    i64 dequeues{};
    //    i64 uploads{};
    //};
    //TotalAccumulator<JobType::Gen> gen_accum_total;
    //TotalAccumulator<JobType::Light> light_accum_total;
    //TotalAccumulator<JobType::Mesh> mesh_accum_total;
    //template<JobType JT>
    //TotalAccumulator<JT>& get_total_accums(){
    //    if constexpr (JT == JobType::Gen){
    //        return gen_accum_total;
    //    }else if constexpr (JT == JobType::Light){
    //        return light_accum_total;
    //    }else if constexpr (JT == JobType::Mesh){
    //        return mesh_accum_total;
    //    }else{
    //        static_assert(false);
    //    }
    //}


private:
    std::vector<WorldChunkCoord> findChunksForGeneration(i64 maxJobs);


    template<JobType JT>
    void enqueue_jobs(std::optional<size_t>max_jobs_override = std::nullopt){
        profiler.bench_start(enqueue_profiler_name<JT>);
        auto& counter = get_pf_accums<JT>().enqueues_this_frame;

        auto max_jobs = max_jobs_override ? *max_jobs_override : max_enqueue_per_frame<JT>;

        profiler.bench_start(find_jobs_profiler_name<JT>);
        auto jobs = director.find_jobs<JT>(max_jobs);
        profiler.bench_end(find_jobs_profiler_name<JT>);
        for (const auto& coord: jobs){
            if (auto rev = enqueue_job_type<JT>(coord)){
                enqueue_delay_bencher<JT>().bench_end(coord);
                          rtt_bencher<JT>().bench_start(coord,*rev);
                counter++;
            }
        }
        profiler.bench_end(enqueue_profiler_name<JT>);
    }
    template<JobType JT>
    [[nodiscard]]
    auto drain_queue(size_t max_uploads){
        auto& q  = res_queue<JT>();
        using res_type = res_struct_t<JT>;
        auto& counter = get_pf_accums<JT>().dequeues_this_frame;
        constexpr auto max_dq_attempts = max_dq_attempts_per_frame<JT>;
        for (i64 attempt = 0; attempt < max_dq_attempts; attempt++){
            auto res = q.try_batch_dequeue(max_uploads);
            if (res){
                counter = (*res).size();
                return std::move(*res);
            }
        }
        counter = 0;
        return std::vector<res_type>{};
    };
    template<JobType JT>
    void upload_results(std::optional<size_t> max_uploads_override = std::nullopt){
        auto max_uploads = max_uploads_override ? *max_uploads_override : max_upload_per_frame<JT>;
        profiler.bench_start(upload_profiler_name<JT>);
        auto& counter = get_pf_accums<JT>().uploads_this_frame;
        for (auto&& res: drain_queue<JT>(max_uploads)){
            res_queue_idle_bencher<JT>().bench_end(res.coord,res.rev);
            auto rev = res.rev;
            auto coord = res.coord;
            if (upload<JT>(std::move(res))){
                counter++;
                rtt_bencher<JT>().bench_end(coord,rev);

            }
        }
        profiler.bench_end(upload_profiler_name<JT>);
    }
    template<JobType JT>
    void update_state_counters(){
        auto pf_accum = get_pf_accums<JT>();
        rb_enqueues<JT>().write(pf_accum.enqueues_this_frame);
        rb_uploads<JT>().write(pf_accum.uploads_this_frame);
        rb_on_queue<JT>().write(pf_accum.uploads_this_frame);
        

        auto& counter = get_state_counters<JT>();
        counter.reset();

        for (const auto& [key, val]: world.chunkMap.entries){
            switch(val.derive_pipeline_state<JT>()){
                case PipelineState::pending: counter.n_pending++; break;
                case PipelineState::on_queue: counter.n_on_queue++; break;
                case PipelineState::done: counter.n_done++; break;
                case PipelineState::ready_for_enqueue: counter.n_ready_for_enqueue++; break;
            }
        }

    }
    void count_states();
};

// Input::Key definitions (based on glfw3)
