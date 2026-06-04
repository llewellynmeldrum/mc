#include <optional>
#include <print>
#include <mdspan>
#include <concepts>
#include <string>

#include "DebugFormat.hpp"

#define DISABLE_STYLE
#include "DebugFormatSpecializations.hpp"
#undef DISABLE_STYLE

#include "DebugUI.hpp"
#include "Assertion.hpp"
#include "ChunkConcurrency.hpp"
#include "Window.hpp"
#include "ChunkConstants.hpp"
#include "Simulation.hpp"
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
    auto* ctx = static_cast<Simulation*>(glfwGetWindowUserPointer(win_ptr));
    DebugUI::StartFrame();

    for (auto& win_cfg: win_configs){
        win_cfg.draw(win_cfg, ctx);
    }

    IG::Render();
    ImGui_ImplOpenGL3_RenderDrawData(IG::GetDrawData());
}


void DebugUI::update() {
    auto* ctx = static_cast<Simulation*>(glfwGetWindowUserPointer(win_ptr));
}


void drawFullscreenOverlay(WindowConfig& self, Simulation* ctx) {
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

void drawNoisePreviewWindow(WindowConfig& self, Simulation* ctx) {
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
void drawSecondCameraWindow(WindowConfig& self, Simulation* ctx) {
    self.setAlpha(0.65f);
    self.setup();
    f32 aspect  = ctx->fixedCamTarget.size.x / ctx->fixedCamTarget.size.y;
    self.setSize(UVSize{0.4,0.4*aspect});
    self.setAlign(WinAlign::TopMid());
    self.setFlags(UI::WinFlags::NoResize);
    self.start_at(true, UVPos{0.5,0},[&self, &ctx]{
        auto& window = self;
        window.section("Secondary View:",[&ctx]{
            UI::Text("{}, {}",dbg_fmt(ctx->fixedCamTarget.pos), dbg_fmt(ctx->fixedCamTarget.size));
            UI::DrawTexture(ctx->fixedCamTarget);

        });
    });

    
}

void drawGeneralDebugOverlay(WindowConfig& self, Simulation* ctx) {
    self.setAlpha(0.65f);
    self.setup();
    self.start_at(UVPos{0,0},[&self, ctx]{
        auto& window = self;
        window.section("Chunk Debug Colors:",[]{
            UI::ColoredText(ChunkEntryStatus::dirtyMeshed.DebugColor(), "Dirty Meshed");
            UI::ColoredText(ChunkEntryStatus::cleanMeshed.DebugColor(), "Clean Meshed");
            UI::ColoredText(ChunkEntryStatus::meshingInProgress.DebugColor(), "Meshing in progress");
            UI::ColoredText(ChunkEntryStatus::generationFinished.DebugColor(), "Generated");
            UI::ColoredText(ChunkEntryStatus::generationInProgress.DebugColor(), "Generation in progress");
            UI::ColoredText(ChunkEntryStatus::ungenerated.DebugColor(), "Ungenerated");
        });
        window.dropdown.show();


        window.section("Positions",[ctx]{
            auto ch_pos = toWorldChunkCoord(ctx->playerCam.pos);
            std::string facing_str = get_facing_str(ctx->playerCam.getFront());
            IG::Text("World: %+03.1f,%+03.1f,%+03.1f", ctx->playerCam.pos.x, ctx->playerCam.pos.y, ctx->playerCam.pos.z);
            IG::Text("Chunk:%+3d,%+3d,%+3d", ch_pos.x, ch_pos.y, ch_pos.z);
            IG::Text("cam.pitch|yaw: %03.1f, %03.1f", ctx->playerCam.pitch, ctx->playerCam.yaw);
            IG::Text("Facing: %s", facing_str.c_str());
        });

        window.section("Perf:",[ctx]{
            const auto& fps_rb = ctx->profiler.ringbufs.at("frame");

            auto k =std::max(1.0f, fps_rb.n_percent_high(1.0));
            assert(k!=0);
            std::string one_pcnt_low = std::format("1% low: {:2.1f}", 1000.0/k);
            IG::Text("FPS: %2.1lf", 1000.0/fps_rb.avg());
            IG::SameLine(); IG::Text("(%s)",one_pcnt_low.c_str());

            for (const auto& [key, val]: ctx->profiler.ringbufs){
                plotRingBuf(val, 10, std::string(key), "2.2lfms", true);
            }

            IG::Text("vsync: %s", ctx->win.enable_vsync ? "enabled" : "disabled");
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
                auto unique_sz = q.uniqueSet.size();
                std::string percentUnique ="all ";
                if (total_sz!=unique_sz){
                    percentUnique = std::format("{:4.1}%",((f64)unique_sz/total_sz)*100.0);
                }
                std::string cur_size = std::format( "{}.size()={:<4} ", name, total_sz);

                auto newcounts_per_second = rb.avg();
                std::string additions = std::format("+{:<4}",newcount);
                std::string avg = std::format("+{:>6.3}/s",newcounts_per_second);
                std::string n_unique = std::format("({:<5} unique)",percentUnique);
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
                UI::SameLine();
                UI::Text(n_unique);
                plotRingBuf(rb, max, name);
            };


            drawSizeAndUniqueness(" genJobQ",ctx->maxGenJobsPerFrame,gen.genJobQueue,ctx->genJobsThisFrame, ctx->rb_genJobsAdded);
            drawSizeAndUniqueness(" genResQ",ctx->maxGenUploadsPerFrame,gen.genResultQueue,ctx->genResultsThisFrame,ctx->rb_genJobsAdded);

            drawSizeAndUniqueness("meshJobQ",ctx->maxMeshJobsPerFrame,mesher.meshJobQueue,ctx->meshJobsThisFrame,ctx->rb_meshJobsAdded);
            drawSizeAndUniqueness("meshResQ",ctx->maxMeshUploadsPerFrame, mesher.meshResultQueue,ctx->meshResultsThisFrame,ctx->rb_meshResultsAdded);

            });

            window.section("World Data:",[ctx]{
            IG::Text("Chunks meshed: %lu", ctx->chunksMeshed);
            IG::Text("Loaded generated chunks: %lu", ctx->world.chunkMap.entries.size());
            IG::Text("Chunks with pending reads: %lu", ctx->world.chunkMap.pendingWritesMap.size());
            {
                std::size_t successful = ctx->world.chunkMap.pendingWritesSuccessful;
                std::size_t  attempted = ctx->world.chunkMap.pendingWritesAttempted;
                IG::Text("Pending chunk writes completed: %lu/%lu", successful, attempted);
            }
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
            {"GENERAL DEBUG OVERLAY", UI::WinFlagGroup::Overlay, drawGeneralDebugOverlay},
            {"SECOND CAMERA", UI::WinFlagGroup::MovableOverlay,drawSecondCameraWindow},
            {"FULLSCREEN OVERLAY", UI::WinFlagGroup::Overlay,drawFullscreenOverlay},
            {"NOISE PREVIEW", UI::WinFlagGroup::Normal,drawNoisePreviewWindow},
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
