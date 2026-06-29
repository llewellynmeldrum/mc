#pragma once

#include "Camera.hpp"
#include "CommonUtils.hpp"
#include "MirroredRingBuf.hpp"
#include "Window.hpp"
#include "Timer.hpp"
#include "Input.hpp"
#include "DebugUI.hpp"
#include "Renderer.hpp"
#include "cppslop.hpp"
#include "Profiler.hpp"

#include "World.hpp"

#include "Concurrency.hpp"

#include "ChunkScheduler.hpp"
// src/Engine.cpp
struct Engine {
  public:
    Engine() : scheduler(std::ref(world)){
        Engine::setup();
    }
    ~Engine() = default;
    Engine(const Engine&) = delete;
    Engine(Engine&&) = delete;
    decltype(auto) operator=(const Engine&) = delete;
    decltype(auto) operator=(Engine&&) = delete;

    std::size_t loop_count{0};
    void setup();
    void loop();
    bool should_close(); 
    i32 exit(i32 exit_code);

    Window   win;
    Profiler profiler;
    Input    input;
    Camera   playerCam;
    Camera   droneCam;
    TextureTarget fixedCamTarget{{0,0},{640,480}};
    Renderer rend;
    DebugUI  ui;
    World    world;
    ChunkScheduler scheduler;

    std::vector<Line3D> lines3d;
    std::vector<Line3D> chunkOutlines;
void count_chunk_states();

    static constexpr std::size_t maxGenUploadsPerFrame= 16;
    static constexpr std::size_t maxGenJobsPerFrame = 16;
    static constexpr std::size_t maxMeshUploadsPerFrame= 256;
    static constexpr std::size_t maxMeshJobsPerFrame = 256;

    std::size_t chunksMeshed{0};

    static constexpr std::size_t RB_SZ = 256;
    MirroredRingBuf<f32, RB_SZ> rb_genJobsAdded;
    MirroredRingBuf<f32, RB_SZ> rb_genResultsAdded;

    MirroredRingBuf<f32, RB_SZ> rb_meshJobsAdded;
    MirroredRingBuf<f32, RB_SZ> rb_meshResultsAdded;

    std::size_t n_generating{};
    MirroredRingBuf<f32, RB_SZ> rb_generating;

    std::size_t n_meshing{};
    MirroredRingBuf<f32, RB_SZ> rb_meshing;

    std::size_t genJobsThisFrame = 0;
    std::size_t genResultsThisFrame = 0;

    std::size_t meshJobsThisFrame = 0;
    std::size_t meshResultsThisFrame = 0;
    
    // include/ChunkEntry.hpp

    std::size_t n_gen_on_queue               {};
    std::size_t n_gen_done                   {};

    std::size_t n_mesh_awaiting_generation   {};
    std::size_t n_mesh_ready_for_enqueue     {};
    std::size_t n_mesh_on_queue              {};
    std::size_t n_mesh_done                  {};
    std::size_t n_mesh_done_clean                  {};

    std::size_t opaque_unloaded {};
    std::size_t opaque_loaded {};
    std::size_t opaque_removed {};

    void update_debug_info();
    RenderTargetView screenView();
    RenderTargetView secondaryView();
    bool inPlayerFrustum(WorldChunkCoord coord);
    void cullMeshes(bool enableFrustumCulling);
    void unMeshAllChunks();
    void unGenerateAllChunks();
    void captureCursor();
    void freeCursor();
    Bounded<i32> draw_distance{8,4,8};

    // anything INSIDE these is meshed/genned
    glm::ivec3 mesh_extent() {return {draw_distance, 4, draw_distance};}
    glm::ivec3 gen_extent() {return mesh_extent()+glm::ivec3{1};}

    // anything outside these is culled
    glm::ivec3 mesh_nocull_extent() {return {draw_distance, 4, draw_distance};}
    glm::ivec3 gen_nocull_extent() {return mesh_extent()+glm::ivec3{1};}


    void handle_inputs();
    void update_scene();
    void draw_scene();
    void pause();
    void unpause();
    bool isPaused()const ;
    void togglePause();
    template<typename ResType>
    std::vector<ResType> drain_queue(Queue<ResType>& queue, std::size_t maxUploads){
        std::vector<ResType> output; output.reserve(maxUploads);

        for (std::size_t mesh_count = 0; mesh_count < maxUploads; mesh_count++){
            std::optional<ResType> result = queue.try_dequeue();
            if (result.has_value()){
                output.emplace_back(*result);
            } else{
                break; // give up this frame
            }
        }
        return output;
    }
private:
    bool paused;

    std::size_t enqueueGenerationJobs(std::span<WorldChunkCoord> candidates);
    std::size_t enqueueMeshingJobs(std::span<WorldChunkCoord> candidates);

    std::size_t drainAndUploadGenResults(std::size_t maxUploads);
    std::size_t drainAndUploadMeshResults(std::size_t maxUploads);
};

// Input::Key definitions (based on glfw3)
