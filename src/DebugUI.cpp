#include <optional>
#include <print>
#include <mdspan>
#include <concepts>
#include <string>

#include "ChunkEntry.hpp"
#include "UIDebugLog.hpp"
#include "DebugFormat.hpp"

#include "FmtStyle.hpp"
#include "PendingBlockWrites.hpp"
#define DISABLE_STYLE
#include "FormatSpecs.hpp"
#undef DISABLE_STYLE

#include "DebugUI.hpp"
#include "Assertion.hpp"
#include "ChunkConcurrency.hpp"
#include "Window.hpp"
#include "ChunkInvariants.hpp"
#include "Engine.hpp"
#include "CoordTypes.hpp"
#include "CommonConcepts.hpp"
#include "ImGuiWrapper.hpp"
#include "glmWrapper.hpp"
#include "GLFWWrapper.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Profiling.hpp"
#include "imgui.h"

#include "DebugUI_Internal.hpp"
#include "Assertion.hpp"

using namespace glm;
namespace IG = ImGui;  // namespace alias for convinience

std::vector<WindowConfig> win_configs;



DebugUI::DebugUI(){
    IMGUI_CHECKVERSION();
    IG::CreateContext();
    ImGuiIO& io = IG::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;      // IF using Docking Branch
    f32 main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());
    IG::StyleColorsDark();

    ImGuiStyle& style = IG::GetStyle();
    style.ScaleAllSizes(
        main_scale * UI_SCALE);  // Bake a fixed style scale. (until we have a solution for dynamic style
                            // scaling, changing this requires resetting Style + calling this again)
    style.FontScaleDpi = main_scale * UI_SCALE;
}

DebugUI::~DebugUI(){
    destroy();
}
void DebugUI::draw() {
    auto* ctx = static_cast<Engine*>(glfwGetWindowUserPointer(win_ptr));
    DebugUI::StartFrame();

    {
        g_StyleConfig::disable();
        for (auto& win_cfg: win_configs){
            win_cfg.draw(win_cfg, ctx);
        }
        g_StyleConfig::enable();
    }

    IG::Render();
    ImGui_ImplOpenGL3_RenderDrawData(IG::GetDrawData());
}


void DebugUI::update() {
    auto* ctx = static_cast<Engine*>(glfwGetWindowUserPointer(win_ptr));
}

void drawLogWindow(WindowConfig& self, Engine* ctx){
    self.setAlpha(0.9f);
    self.setup();
    self.setSize(UVSize{0.4,0.4});
    self.setAlign(WinAlign::TopMid());
    self.setFlags();
    self.start_at(true, UVPos{0.7,0.5},[&self, &ctx]{
        auto& window = self;
        window.open_section("Global Log:",[&self, &ctx]{
            // Shown in most->least recent vertical order
            for (const auto& entry: global_log | std::views::reverse){
                UI::Text(DebugLog::entry_tostr(entry));
            }
        });
        window.section("Per Chunk Log:",[&self, &ctx]{
            // Shown in most->least recent vertical order
            WorldChunkCoord cur_chunk = toWorldChunkCoord(ctx->playerCam.pos);
            if (per_chunk_log.contains(cur_chunk)){
                for (const auto& entry: per_chunk_log.at(cur_chunk) | std::views::reverse){
                    UI::Text(DebugLog::entry_tostr(entry));
                }
            }
        });
    });
}


void drawFullscreenOverlay(WindowConfig& self, Engine* ctx) {
    auto io = IG::GetIO();
    if (ctx->isPaused()){
        auto* d = IG::GetForegroundDrawList();
        auto display_size = io.DisplaySize;
        d->AddRectFilled({0,0}, display_size, IM_COL32(20, 20, 20, 64));
        const char* text = "PAUSED";
        ScreenPos text_size = IG::CalcTextSize(text) * 2.5f; // default font size

        ScreenPos text_pos = ScreenPos(
            (display_size.x - text_size.x) * 0.5f,
            (display_size.y - text_size.y) * 0.5f
        );

        d->AddText(text_pos, IM_COL32_WHITE, text);
    }
}

void drawNoisePreviewWindow(WindowConfig& self, Engine* ctx) {
    // TODO: Make this.
    // Perhaps a 2d noise preview image (black to white)
    // Alongside the adjustments for each one 
    self.setAlpha(0.65f);
    self.setup();
    self.setAlign(WinAlign::TopMid());
    self.start_at(true, UVPos{0.5,0},[&self, ctx]{
        auto& window = self;
        window.section("Noise sliders:",[]{
            UI::Text("Noise preview hahahaha");
        });
        window.dropdown.show();
    });
}

void drawConcurrencyWindow(WindowConfig& self, Engine* ctx) {
    self.setAlpha(0.95f);
    self.setup();
    self.setAlign(WinAlign::TopMid());
    self.start_at(true, UVPos{0.5,0},[&]{
        auto& window = self;
        const auto& mesh_threads = ctx->rend.meshers.mesherThreads;
        const auto& gen_threads = ctx->world.chunkMap.generator.genThreads;
        auto q_print_stats = [](std::string_view name, const auto& q, const auto& rb){
            const auto sz = q.size_unlocked();
            const auto cap = q.capacity;
            const auto load = sz/static_cast<f32>(cap);
            auto new_per_sec = rb.avg();
            std::string avg = std::format("+{:>6.3}/s",new_per_sec);
            UI::Text("{} Q: {: 4}/{} ({:3.2f}%)",name, sz,cap,load*100.0f);
            UI::SameLine();
            UI::ColoredText({0,1,0,1},"{}",avg);
        };
        window.open_section("Mesh Threads:",[&]{
            q_print_stats("Job", ctx->rend.meshers.meshJobQueue, ctx->rb_meshJobsAdded);
            q_print_stats("Res", ctx->rend.meshers.meshResultQueue,ctx->rb_meshResultsAdded);
            for (const auto& tracker: mesh_threads.trackers ){
                UI::BulletPoint("{}",tracker.to_str());
            }
        });
        window.open_section("Gen Threads:",[&]{
            q_print_stats("Job",ctx->world.chunkMap.generator.genJobQueue,ctx->rb_genJobsAdded);
            q_print_stats("Res",ctx->world.chunkMap.generator.genResultQueue,ctx->rb_genResultsAdded);
            for (const auto& tracker: gen_threads.trackers ){
                UI::BulletPoint("{}",tracker.to_str());
            }
        });
    });
}


void drawSecondCameraWindow(WindowConfig& self, Engine* ctx) {
    self.setAlpha(0.65f);
    self.setup();
    f32 aspect  = ctx->fixedCamTarget.size.x / ctx->fixedCamTarget.size.y;
    f32 xwidth = ctx->fixedCamTarget.size.x/ctx->win.px_w * 2;
    self.setSize(UVSize{xwidth, xwidth * aspect});
    self.setAlign(WinAlign::TopMid());
    self.setFlags(UI::WinFlags::NoResize);
    self.start_at(true, UVPos{0.5,0},[&self, &ctx]{
        auto& window = self;
        window.section("Secondary View:",[&ctx]{
            UI::Text("scr: {}, {}",ctx->fixedCamTarget.pos, ctx->fixedCamTarget.size);
            UI::Text("  w: {}, {}",ctx->droneCam.pos.raw(), ctx->droneCam.ortho_zoom);
            UI::DrawTexture(ctx->fixedCamTarget);

        });
    });

    
}

void drawGeneralDebugOverlay(WindowConfig& self, Engine* ctx) {
    self.setAlpha(0.65f);
    self.setup();
    self.setFlags(UI::WinFlags::NoResize);
    self.start_at(UVPos{0,0},[&self, ctx]{
        auto& window = self;
        window.open_section("Chunk States:",[ctx]{
            IG::Separator();
            UI::Text("GenStates:");
            #define X(name) UI::ColoredText(GenDebugOutlineColor(GenState :: name),"{}: {}", #name, ctx->n_gen_ ##name);
            GEN_STATE_LIST
            #undef X

            IG::Separator();
            UI::Text("MeshStates:");
            #define X(name) UI::ColoredText(MeshDebugOutlineColor(MeshState:: name),"{}: {}", #name, ctx->n_mesh_ ##name);
            MESH_STATE_LIST
            #undef X
            UI::Text("Meshed (clean): {}",ctx->n_mesh_done_clean);
            UI::Text("Chunk Entries: {}",ctx->world.chunkMap.entries.size());
            UI::Text("opaque_unloaded:{}",  ctx->opaque_unloaded);
            UI::Text("opaque_loaded   :{}   ",ctx->opaque_loaded );
            UI::Text("opaque_removed :{}",  ctx->opaque_removed);
        });
        window.open_section("World Data:",[&]{
            IG::Text("Chunks meshed: %lu", ctx->chunksMeshed);
            IG::Text("Chunks meshed: %lu", ctx->chunksMeshed);
            IG::Text("Generated chunks: %lu", ctx->world.chunkMap.entries.size());
            auto n_pending_ungenerated  = 0uz;
            auto n_pending_unmeshed     = 0uz;
            auto n_pending_clean_meshed = 0uz;
            auto n_pending_dirty_meshed = 0uz;
            for (const auto& [key, _]: ctx->world.chunkMap.pending_writes){
                ctx->world.chunkMap.entries.if_contains_else(
                    key,
                    [&](ChunkEntry& entry){
                        if (entry.state.mesh == MeshState::done){
                            n_pending_clean_meshed += entry.is_mesh_clean();
                            n_pending_dirty_meshed += entry.is_mesh_dirty();
                        }  else{
                            n_pending_unmeshed++;
                        }
                    },
                    [&](){
                        n_pending_ungenerated++;
                    }
                );
            }
            window.sub_section("Pending Writes:",[&](){
                IG::Text("Total Pending: %lu", ctx->world.chunkMap.pending_writes.size());
                IG::Text("Ungenerated Pending: %lu", n_pending_ungenerated);
                IG::Text("Generated Pending: %lu", n_pending_unmeshed+n_pending_clean_meshed+n_pending_dirty_meshed);
                IG::Text("Unmeshed Pending: %lu", n_pending_unmeshed);
                IG::Text("Clean Meshed Pending: %lu", n_pending_clean_meshed);
                IG::Text("Dirty Meshed Pending: %lu", n_pending_dirty_meshed);
                {
                    int successful = ctx->world.chunkMap.pendingWritesSuccessful;
                    int attempted = ctx->world.chunkMap.pendingWritesAttempted;
                    IG::Text("Pending chunk writes completed: %d/%d", successful, attempted);
                }
            });
        });
        window.open_section("Performance:",[&]{
            UI::Text("Optimization lvl: -O{}",ctx->ui.dbg_params.opt_lvl);
            UI::Text("vsync: {}", ctx->win.enable_vsync ? "enabled" : "disabled");
            UI::Text("draw dist: {}", ctx->draw_distance.val);

            const auto& fps_rb = ctx->profiler.ringbufs.at("frame");
            auto k =std::max(1.0f, fps_rb.n_percent_high(1.0));
            assert(k!=0);
            std::string one_pcnt_low = std::format("1% low: {:2.1f}", 1000.0/k);
            IG::Text("FPS: %2.1lf", 1000.0/fps_rb.avg());
            IG::SameLine(); IG::Text("(%s)",one_pcnt_low.c_str());

            for (const auto& [key, val]: ctx->profiler.ringbufs){
                if (key=="frame") continue;
                plotRingBuf(val, 10, std::string(key), "2.2lfms", true);
            }

        });

        window.open_section("Per frame draw info:",[ctx]{
            std::size_t n_vtx = ctx->rend.debug.vertex_count;
            std::size_t n_bytes = n_vtx * sizeof(Vertex);
            f32 kb = n_bytes /1000.0;
            f32 mb = kb/1000.0;
            f32 gb = mb/1000.0;
            IG::Text("Vertex Count: %lu (%7.2lfMB | %6.2lfGB) ",n_vtx, mb,gb); 
            IG::Text("Draw Calls: %llu", ctx->rend.debug.draw_calls);
            IG::Text("Mesh Count: %llu", ctx->rend.debug.mesh_count);
        });

        window.open_section("Process Metrics:",[]{
            IG::Text("Resident Set Size: %5.2lfmb", current_rss_bytes()/1024.0/1024.0);
        });

        window.open_section("Concurrency:",[ctx]{
            auto& gen = ctx->world.chunkMap.generator;
            auto& mesher = ctx->rend.meshers;
            auto drawSizeAndUniqueness = [ctx]
                (const std::string name, std::size_t max, auto& q, auto newcount, const auto& rb){

                auto total_sz = q.wait_size();
                std::string cur_size = std::format( "{}.size()={:<4} ", name, total_sz);

                auto newcounts_per_second = rb.avg();
                std::string additions = std::format("+{:<4}",newcount);
                std::string avg = std::format("+{:>6.3}/s",newcounts_per_second);
                UI::Text(cur_size); UI::SameLine();
                {
                    UI::setTextColor(0,255,0);
                        UI::Text(additions.c_str()); 
                    UI::ResetTextColor();

                    UI::SameLine();

                    UI::setTextColor(0,255,0);
                        UI::Text(avg.c_str()); 
                    UI::ResetTextColor();
                }
                plotRingBuf(rb, max, name);
            };

            auto idk =[](auto cur_size, auto& rb, auto max, auto& name){
                UI::Text("{}:", name); UI::SameLine();
                UI::Text("{}", cur_size); UI::SameLine();
                {
                    UI::setTextColor(0,255,0);
                        UI::Text("{}",rb.avg()); 
                    UI::ResetTextColor();
                }
                plotRingBuf(rb, max, name);
            };
            idk(
                ctx->n_generating,
                ctx->rb_generating,
                gen.genJobQueue.capacity + gen.genResultQueue.capacity,
                "Generating"
            );

            idk(
                ctx->n_meshing,
                ctx->rb_meshing,
                ctx->rend.meshers.meshJobQueue.capacity + ctx->rend.meshers.meshResultQueue.capacity,
                "Meshing"
            );

                
            IG::Separator();

            drawSizeAndUniqueness(
                " genJobQ",
                ctx->maxGenJobsPerFrame,
                gen.genJobQueue,
                ctx->genJobsThisFrame,
                ctx->rb_genJobsAdded
            );
            drawSizeAndUniqueness(
                " genResQ",
                ctx->maxGenUploadsPerFrame,
                gen.genResultQueue,
                ctx->genResultsThisFrame,
                ctx->rb_genResultsAdded
            );

            drawSizeAndUniqueness(
                "meshJobQ",
                ctx->maxMeshJobsPerFrame,
                mesher.meshJobQueue,
                ctx->meshJobsThisFrame,
                ctx->rb_meshJobsAdded
            );
            drawSizeAndUniqueness(
                "meshResQ",
                ctx->maxMeshUploadsPerFrame,
                mesher.meshResultQueue,
                ctx->meshResultsThisFrame,
                ctx->rb_meshResultsAdded
            );

            });


        window.section("Chunk Data:",[ctx]{
            auto mesh_state_color = DefaultDebugColor();
            auto gen_state_color = DefaultDebugColor();
            auto ch_pos = toWorldChunkCoord(ctx->playerCam.pos);
            bool showGenState = ctx->ui.dbg_view.showGenState;
            std::string gen_state_str{"No state entry."};
            std::string mesh_state_str{"No state entry."};
            auto target_mesh_id = 0uz;
            auto inflight_mesh_id = 0uz;
            auto loaded_mesh_id = 0uz;
            ctx->world.chunkMap.entries.if_contains(
                ch_pos,
                [&](ChunkEntry& entry){
                    target_mesh_id = entry.target_mesh_revision;
                    inflight_mesh_id = entry.inflight_mesh_revision;
                    loaded_mesh_id = entry.loaded_mesh_revision;
                    ChunkState& state = entry.state;
                    GenState gen_stage{};
                    bool gen_clean{};
                    bool noBlocks{};
                    bool opaque_loaded{};
                    bool transp_loaded{};
                    bool opaque_empty{};
                    bool transp_empty{};
                    bool mesh_clean{};
                    MeshState mesh_stage{};
                    gen_stage = state.gen;
                    gen_clean = entry.is_gen_clean();

                    
                    ctx->world.chunkMap.entries.if_contains(
                        ch_pos,
                        [&](ChunkEntry& entry){
                            noBlocks = entry.block_data.empty();
                        }
                    );
                    
                    gen_state_str = std::format(
                        "{} ({})  {}",
                        state.gen,
                        (gen_clean ? "clean"     : "dirty"),
                        (noBlocks ? "(empty)" : "")
                    );
                    gen_state_color = GenDebugOutlineColor(state.gen);

                    mesh_stage=state.mesh;
                    mesh_clean = entry.is_mesh_clean();

                    
                    ctx->rend.opaqueChunkMeshes.if_contains(
                        ch_pos,
                        [&](Mesh& mesh){
                            opaque_empty = mesh.vertex_count==0;
                            opaque_loaded = mesh.isLoaded();
                        }
                    );
                    ctx->rend.transparentChunkMeshes.if_contains(
                        ch_pos,
                        [&](Mesh& mesh){
                            transp_empty = mesh.vertex_count==0;
                            transp_loaded = mesh.isLoaded();
                        }
                    );
                    
                    mesh_state_str = std::format(
                        "{} ({})  OP:{}{} TR:{}{}",
                        state.mesh,
                        (mesh_clean ? "clean"     : "dirty"    ),
                        (opaque_loaded ? "LOADED" : "UNLOADED" ),
                        (opaque_empty ? "(empty)" : ""         ),
                        (transp_loaded ? "LOADED" : "UNLOADED" ),
                        (transp_empty ? "(empty)" : ""         )
                    );
                    mesh_state_color = MeshDebugOutlineColor(state.mesh);
                    // state.gen 
                    // opqMesh: [Loaded|Unloaded] [(empty)] 
                    // trnMesh: [Loaded|Unloaded] [(empty)] 
                    // isDirty
                }
            );
            auto n_pending_writes = 0uz;
            ctx->world.chunkMap.pending_writes.if_contains(
                ch_pos,
                [&](PendingWriteQueue& pwq){
                    n_pending_writes = pwq.size();
                }
            );



            std::string facing_str = get_facing_str(ctx->playerCam.getFront());
            IG::Text("WorldChunkCoord:%+3d,%+3d,%+3d", ch_pos.x, ch_pos.y, ch_pos.z);
            UI::ColoredText01(gen_state_color ,"G:{}", gen_state_str);
            UI::ColoredText01(mesh_state_color,"M:{}", mesh_state_str);
            UI::Text("MeshRev: loaded:{}\t | inflight:{}\t | target:{}",loaded_mesh_id,inflight_mesh_id,target_mesh_id);
            UI::Text("PendingWrites:{}",n_pending_writes);
            auto total_opaque   =0uz;
            auto loaded_opaque  =0uz;
            auto total_trans    =0uz;
            auto loaded_trans   =0uz;
            ctx->rend.opaqueChunkMeshes.for_each_if([](auto& mesh){return mesh.isLoaded();}, [&loaded_opaque](auto& mesh){loaded_opaque++;});
            ctx->rend.transparentChunkMeshes.for_each_if([](auto& mesh){return mesh.isLoaded();}, [&loaded_trans](auto& mesh){loaded_trans++;});
            UI::Text("Loaded opaque:{}/{}",loaded_opaque,total_opaque);
            UI::Text("Loaded trans:{}/{}",loaded_trans,total_trans);
        });
        window.section("Positions",[ctx]{
            auto ch_pos = toWorldChunkCoord(ctx->playerCam.pos);
            std::string facing_str = get_facing_str(ctx->playerCam.getFront());
            IG::Text("World: %+03.1f,%+03.1f,%+03.1f", ctx->playerCam.pos.x, ctx->playerCam.pos.y, ctx->playerCam.pos.z);
            IG::Text("Chunk:%+3d,%+3d,%+3d", ch_pos.x, ch_pos.y, ch_pos.z);
            IG::Text("cam.pitch|yaw: %03.1f, %03.1f", ctx->playerCam.pitch, ctx->playerCam.yaw);
            IG::Text("Facing: %s", facing_str.c_str());
        });

        window.section("Padding:",[ctx]{
            UI::Text("");
            UI::Text("");
            UI::Text("");
            UI::Text("");
        });

    });
}

void DebugUI::init(GLFWwindow* _win_ptr) {
    this->win_ptr = _win_ptr;
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(win_ptr, true);
    // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();
    win_configs.insert_range(
        win_configs.begin(),std::vector<WindowConfig>{ 
            {"GENERAL DEBUG OVERLAY", UI::WinFlagGroup::MovableOverlay, drawGeneralDebugOverlay,this},
            {"SECOND CAMERA", UI::WinFlagGroup::MovableOverlay,drawSecondCameraWindow,this},
            {"FULLSCREEN OVERLAY", UI::WinFlagGroup::Overlay,drawFullscreenOverlay,this},
            {"NOISE PREVIEW", UI::WinFlagGroup::Normal,drawNoisePreviewWindow,this},
            {"CONCURRENCY", UI::WinFlagGroup::Normal,drawConcurrencyWindow,this},
            {"LOG WINDOW", UI::WinFlagGroup::MovableOverlay,drawLogWindow,this},
        }
    );
}

void DebugUI::StartFrame(){
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    IG::NewFrame();
}
void DebugUI::destroy() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    IG::DestroyContext();
}
