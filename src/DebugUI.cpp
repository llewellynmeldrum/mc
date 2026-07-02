#include <optional>
#include <print>
#include <mdspan>
#include <concepts>
#include <string>

#include "ChunkEntry.hpp"
#include "DebugChunkLog.hpp"
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
#include "ChunkMap.hpp"

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
        static bool on = false;
//        for ()
        for (auto& [key, val]: is_log_type_enabled ){
            window.checkbox(key, &val);
        }
        window.open_section("Per chunk log:",[&self, &ctx]{
            // Shown in most->least recent vertical order
            WorldChunkCoord cur_chunk = toWorldChunkCoord(ctx->player_cam.pos);
            if (per_chunk_log.contains(cur_chunk)){
                for (const auto& entry: per_chunk_log.at(cur_chunk) | std::views::reverse){
                    auto [log_type, duration, contents] = entry;
                    if (is_log_type_enabled.at(log_type)){
                        UI::Text(DebugLog::entry_tostr(entry));
                    }
                }
            }
        });
    });
}


void drawFullscreenOverlay(WindowConfig& self, Engine* ctx) {
    auto io = IG::GetIO();
    if (ctx->paused){
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
    }else if (ctx->chunk_updates_paused){
        auto* d = IG::GetForegroundDrawList();
        auto display_size = io.DisplaySize;
        d->AddRectFilled({0,0}, display_size, IM_COL32(20, 20, 20, 6));
        const char* text = "CHUNK UPDATES PAUSED";
        ScreenPos text_size = IG::CalcTextSize(text) * 2.5f; // default font size

        ScreenPos text_pos = ScreenPos(
            (display_size.x - text_size.x) * 0.5f,
            (display_size.y - text_size.y) * 0.1f
        );

        d->AddText(text_pos, IM_COL32_WHITE, text);
    }
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
            UI::Text("  w: {}, {}",ctx->drone_cam.pos.raw(), ctx->drone_cam.ortho_zoom);
            UI::DrawTexture(ctx->fixedCamTarget);

        });
    });

    
}

void drawGeneralDebugOverlay(WindowConfig& self, Engine* ctx) {
    self.setAlpha(0.65f);
    self.setup();
    self.setFlags(UI::WinFlags::AlwaysAutoResize);
    self.start_at(UVPos{0,0},[&self, ctx]{
        auto& window = self;
        const auto& fps_rb = ctx->profiler.ringbufs.at("frame");
        const auto k = std::max(1.0f, fps_rb.n_percent_high(1.0));
        assert(k!=0);
        const auto p99 = 1000.0/k;
        const auto fps = 1000.0/fps_rb.avg();
        const auto ft_ms = fps_rb.avg();

        const auto draw_ms = ctx->profiler.ringbufs.at("draw").avg();
        const auto upd_ms = ctx->profiler.ringbufs.at("update").avg();
        const auto upd_pcnt = 100.0 * upd_ms / ft_ms;
        const auto draw_pcnt = 100.0 * draw_ms / ft_ms;

        const auto n_chunks_loaded = ctx->rend.opaqueChunkMeshes.size();
        const auto n_chunks_meshing = ctx->n_meshing;
        const auto n_chunks_genning = ctx->n_generating;

        const auto pos = ctx->player_cam.pos;
        const auto round_pos = glm::ivec3{LM::floor(ctx->player_cam.pos).raw()};
        const auto ch_pos = toWorldChunkCoord(ctx->player_cam.pos);
        const auto cl_pos = pos.raw() - 
            glm::vec3{
                ch_pos.x * Chunk::Extents.x,
                round_pos.y,
                ch_pos.z * Chunk::Extents.z,
            };

        UI::Text("fps: {: 4.1f} (p99: {: 4.1f})",fps, p99);
        UI::Text("frametime: {: 4.1f}ms (upd: {: 3.1f}%, draw: {: 3.1f}%)", ft_ms,upd_pcnt,draw_pcnt);
        UI::Separator();
        UI::Text("meshed: {: 7}, +{: 7} generating, +{: 7} meshing",n_chunks_loaded,n_chunks_genning,n_chunks_meshing);
        UI::Text("entries : {: 7} ",ctx->world.chunkMap.entries.size());
        UI::Separator();
        UI::Text("pos :{: 4.1f},{: 4.1f},{: 4.1f} (B:{: 3},{: 3},{: 3})",
                 pos.x,pos.y,pos.z,std::floor(pos.x),std::floor(pos.y),std::floor(pos.z));
        UI::Text("chunk :{: 3},{: 3}",ch_pos.x,ch_pos.z);
        UI::Text("chunk local: {: 4.1f},{: 4.1f},{: 4.1f} (B:{: 3},{: 3},{: 3})",
                 cl_pos.x,cl_pos.y,cl_pos.z,std::floor(cl_pos.x),std::floor(cl_pos.y),std::floor(cl_pos.z));
        UI::Separator();
        if (!ctx->ui.is_ui_expanded){
            UI::Text("Press '`' to expand.");
            return;
        }
        UI::Text("Press '`' to minimize.");
        window.section("Chunk States:",[ctx]{
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
            UI::Text("Chunk Entries: {}",ctx->world.chunkMap.entries.size());
        });
        window.dropdown.show();


        window.section("Chunk Data:",[ctx]{
            auto mesh_state_color = DefaultDebugColor();
            auto gen_state_color = DefaultDebugColor();
            auto ch_pos = toWorldChunkCoord(ctx->player_cam.pos);
            bool showGenState = DebugOption::showGenState;
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



            std::string facing_str = get_facing_str(ctx->player_cam.getFront());
            UI::Text("WorldChunkCoord:{:+3},{:+3}", ch_pos.x,ch_pos.z);
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
            auto ch_pos = toWorldChunkCoord(ctx->player_cam.pos);
            std::string facing_str = get_facing_str(ctx->player_cam.getFront());
            IG::Text("World: %+03.1f,%+03.1f,%+03.1f", ctx->player_cam.pos.x, ctx->player_cam.pos.y, ctx->player_cam.pos.z);
            UI::Text("WorldChunkCoord:{:+3},{:+3}", ch_pos.x,ch_pos.z);
            IG::Text("cam.pitch|yaw: %03.1f, %03.1f", ctx->player_cam.pitch, ctx->player_cam.yaw);
            IG::Text("Facing: %s", facing_str.c_str());
        });
        window.section("World Data:",[ctx]{
            UI::Text("Chunks meshed: {}", ctx->chunksMeshed);
            UI::Text("Generated chunks: {}", ctx->world.chunkMap.entries.size());
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
            UI::Text("Total Pending: {}", ctx->world.chunkMap.pending_writes.size());
            UI::Text("Ungenerated Pending: {}", n_pending_ungenerated);
            UI::Text("Generated Pending: {}", n_pending_unmeshed+n_pending_clean_meshed+n_pending_dirty_meshed);
            UI::Text("Unmeshed Pending: {}", n_pending_unmeshed);
            UI::Text("Clean Meshed Pending: {}", n_pending_clean_meshed);
            UI::Text("Dirty Meshed Pending: {}", n_pending_dirty_meshed);
            {
                int successful = ctx->world.chunkMap.pendingWritesSuccessful;
                int attempted = ctx->world.chunkMap.pendingWritesAttempted;
                UI::Text("Pending chunk writes completed: {}/{}", successful, attempted);
            }
            UI::Text(".");
            UI::Text(".");
            UI::Text(".");
            UI::Text(".");
        });

        window.section("Perf:",[ctx]{
            UI::Text("Optimization lvl: -O{}",DebugOption::compiler_optimisation_level);
            IG::Text("vsync: %s", ctx->win.enable_vsync ? "enabled" : "disabled");

            const auto& fps_rb = ctx->profiler.ringbufs.at("frame");
            auto k =std::max(1.0f, fps_rb.n_percent_high(1.0));
            assert(k!=0);
            std::string one_pcnt_low = std::format("1% low: {:2.1f}", 1000.0/k);
            IG::Text("FPS: %2.1lf", 1000.0/fps_rb.avg());
            IG::SameLine(); IG::Text("(%s)",one_pcnt_low.c_str());

            for (const auto& [key, val]: ctx->profiler.ringbufs){
                plotRingBuf(val, 10, std::string(key), "2.2lfms", true);
            }

        });

        window.section("Per frame draw info:",[ctx]{
            std::size_t n_vtx = ctx->rend.debug.vertex_count;
            std::size_t n_bytes = n_vtx * sizeof(Vertex);
            f32 kb = n_bytes /1000.0;
            f32 mb = kb/1000.0;
            f32 gb = mb/1000.0;
            IG::Text("Vertex Count: %lu (%7.2lfMB | %6.2lfGB) ",n_vtx, mb,gb); 
            IG::Text("Draw Calls: %llu", ctx->rend.debug.draw_calls);
            IG::Text("Mesh Count: %llu", ctx->rend.debug.mesh_count);
        });

        window.section("Process Metrics:",[]{
            IG::Text("Resident Set Size: %5.2lfmb", current_rss_bytes()/1024.0/1024.0);
        });

        window.section("Concurrency:",[ctx]{
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

            drawSizeAndUniqueness(" genJobQ",ctx->maxGenJobsPerFrame,gen.genJobQueue,ctx->genJobsThisFrame, ctx->rb_genJobsAdded);
            drawSizeAndUniqueness(" genResQ",ctx->maxGenUploadsPerFrame,gen.genResultQueue,ctx->genResultsThisFrame,ctx->rb_genJobsAdded);

            drawSizeAndUniqueness("meshJobQ",ctx->maxMeshJobsPerFrame,mesher.meshJobQueue,ctx->meshJobsThisFrame,ctx->rb_meshJobsAdded);
            drawSizeAndUniqueness("meshResQ",ctx->maxMeshUploadsPerFrame, mesher.meshResultQueue,ctx->meshResultsThisFrame,ctx->rb_meshResultsAdded);

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
