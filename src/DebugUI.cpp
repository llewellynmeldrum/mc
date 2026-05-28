#include "DebugUI.hpp"
#include "Assertion.hpp"
#include "ChunkConcurrency.hpp"
#include "ChunkConstants.hpp"
#include "Simulation.hpp"
#include "CoordTypes.hpp"
#include "DebugFormat.hpp"
#include "GLFW/glfw3.h"



#include "ImGuiWrapper.hpp"
#include "glmWrapper.hpp"

#include "GLFWWrapper.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Profiling.hpp"
#include "imgui.h"
#include <optional>
#include <print>
#include <mdspan>
#include <string>
using namespace glm;

using ScreenPos = ImVec2;
using Color = ImColor;
constexpr ScreenPos PAD = {5.0f, 25.0f};
constexpr ScreenPos ALIGN_TOP_LEFT = {0.0f,0.0f};
constexpr ScreenPos ALIGN_TOP_RIGHT = {1.0f,0.0f};
constexpr ScreenPos ALIGN_MID_MID = {0.5f,0.5f};
constexpr ImVec2 GRAPH_SIZE = {400,40};
constexpr f32 UI_SCALE = 1.25;
namespace IG = ImGui;  // namespace alias for convinience
enum struct DrawMode{
    Center=0,
};
void DebugUI::drawUI() {
    auto* ctx = static_cast<Simulation*>(glfwGetWindowUserPointer(win_ptr));
    drawFullscreenOverlay(ctx);
    drawMainOverlay(ctx);
}


void DebugUI::setupDebugUI(GLFWwindow* _win_ptr) {
    this->win_ptr = _win_ptr;
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;      // IF using Docking Branch
    float main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());
    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(
        main_scale * UI_SCALE);  // Bake a fixed style scale. (until we have a solution for dynamic style
                            // scaling, changing this requires resetting Style + calling this again)
    style.FontScaleDpi = main_scale * UI_SCALE;

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(win_ptr, true);
    // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();
}

void DebugUI::drawDebugUI() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    drawUI();
}
void DebugUI::render() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void DebugUI::updateUI() {
    auto* ctx = static_cast<Simulation*>(glfwGetWindowUserPointer(win_ptr));
    auto& io = ImGui::GetIO();
}

void DebugUI::destroyDebugUI() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void DebugUI::drawFullscreenOverlay(Simulation* ctx) {
    auto io = ImGui::GetIO();
    if (ctx->isPaused()){
        auto* d = ImGui::GetForegroundDrawList();
        auto display_size = io.DisplaySize;
        d->AddRectFilled({0,0}, display_size, IM_COL32(20, 20, 20, 64));
        const char* text = "PAUSED";
        ScreenPos text_size = ImGui::CalcTextSize(text) * 2.5f; // default font size

        ScreenPos text_pos = ScreenPos(
            (display_size.x - text_size.x) * 0.5f,
            (display_size.y - text_size.y) * 0.5f
        );

        d->AddText(text_pos, IM_COL32_WHITE, text);
    }
}

void DebugUI::drawMainOverlay(Simulation* ctx) {
    ImGuiIO&         io = ImGui::GetIO();
    UI::WinFlags window_flags = UI::WinFlags::NoDecoration | UI::WinFlags::NoDocking |
                                    UI::WinFlags::AlwaysAutoResize | UI::WinFlags::NoSavedSettings |
                                    UI::WinFlags::NoFocusOnAppearing | UI::WinFlags::NoNav | UI::WinFlags::NoMove;
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ScreenPos work_pos = viewport->WorkPos;  
    ScreenPos work_size = viewport->WorkSize;
    ScreenPos window_pos, window_pos_pivot;
    window_pos.x = (work_pos.x + PAD.x);
    window_pos.y = (work_pos.y + PAD.y);
    window_pos_pivot.x = 0.0f;
    window_pos_pivot.y = 0.0f;
    ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::SetNextWindowBgAlpha(0.35f);  // Transparent background
    std::string facing_str{};
    const vec3& facing = ctx->cam.getFront();
    auto        approx_eq = [](auto x, auto target) {
        const f32 tolerance = 0.1;
        return x >= (target - tolerance) && x <= (target + tolerance);
    };
    if (approx_eq(facing.x, +1.0))
        facing_str += "+X ";
    else if (approx_eq(facing.x, -1.0))
        facing_str += "-X ";
    else
        facing_str += "   ";

    if (approx_eq(facing.y, +1.0))
        facing_str += ",+Y ";
    else if (approx_eq(facing.y, -1.0))
        facing_str += ",-Y ";
    else
        facing_str += ",   ";

    if (approx_eq(facing.z, +1.0))
        facing_str += ",+Z ";
    else if (approx_eq(facing.z, -1.0))
        facing_str += ",-Z ";
    else
        facing_str += ",   ";

    auto ch_pos = toWorldChunkCoord(ctx->cam.pos);
    if (UI::StartWindow("DebugOverlay",window_flags)) {
        ImGui::Text("DEBUG OVERLAY");
        UI::ColoredText(ChunkEntryStatus::dirtyMeshed.dbg_color, "Dirty Meshed");
        UI::ColoredText(ChunkEntryStatus::cleanMeshed.dbg_color, "Clean Meshed");
        UI::ColoredText(ChunkEntryStatus::meshingInProgress.dbg_color, "Meshing in progress");
        UI::ColoredText(ChunkEntryStatus::generationFinished.dbg_color, "Generated");
        UI::ColoredText(ChunkEntryStatus::generationInProgress.dbg_color, "Generation in progress");
        UI::ColoredText(ChunkEntryStatus::ungenerated.dbg_color, "Ungenerated");


        ImGui::Separator();
        ImGui::Text("Positions:");
        ImGui::Text("World: %+03.1f,%+03.1f,%+03.1f", ctx->cam.pos.x, ctx->cam.pos.y, ctx->cam.pos.z);
        ImGui::Text("Chunk:%+3d,%+3d,%+3d", ch_pos.x, ch_pos.y, ch_pos.z);
        ImGui::Text("cam.pitch|yaw: %03.1f, %03.1f", ctx->cam.pitch, ctx->cam.yaw);
        ImGui::Text("Facing: %s", facing_str.c_str());

        ImGui::Separator();
        ImGui::Text("Performance:");
        const auto& fps_rb = ctx->time.ringbufs.at("frame");

        auto k =std::max(1.0f, fps_rb.n_percent_high(1.0));
        assert(k!=0);
        std::string one_pcnt_low = std::format("1% low: {:2.1f}", 1000.0/k);
        ImGui::Text("FPS: %2.1lf", 1000.0/fps_rb.avg());
        ImGui::SameLine(); ImGui::Text("(%s)",one_pcnt_low.c_str());

        auto plotRingBuf = [](const auto& rb,std::size_t max, const std::string key="????", const std::string fmt="p??", bool printValue=false){
            
            if (printValue){
                std::string _fmt = std::string("%-12s: %")+fmt;
                // todo find longest string and make it the field length
                ImGui::Text(_fmt.c_str(), key.c_str(), rb.avg());
                UI::SameLine();
            }
            std::string id = "##"+key;
//void ImGui::PlotLines(const char* label, const float* values, int values_count, int values_offset, const char* overlay_text, float scale_min, float scale_max, ImVec2 graph_size, int stride)
            if (!max) max=FLT_MAX;
            ImGui::PlotLines(id.c_str(), rb.data(), rb.size(), 0, "", FLT_MAX, max);
        };
        for (const auto& [key, val]: ctx->time.ringbufs){
            plotRingBuf(val, 10, std::string(key), "2.2lfms", true);
        }

        ImGui::Text("vsync: %s", ctx->win.enable_vsync ? "enabled" : "disabled");

        ImGui::Separator();
        ImGui::Text("Per frame draw info:");
        ImGui::Text("Vertex Count: %llu", ctx->rend.debug.vertex_count);
        ImGui::Text("Draw Calls: %llu", ctx->rend.debug.draw_calls);
        ImGui::Text("Mesh Count: %llu", ctx->rend.debug.mesh_count);

        ImGui::Separator();
        ImGui::Text("Process metrics");
        ImGui::Text("Resident Set Size: %5.2lfmb", current_rss_bytes()/1024.0/1024.0);

        auto& gen = ctx->world.chunkMap.generator;
        auto& mesher = ctx->rend.mesher;
        auto drawSizeAndUniqueness = [&plotRingBuf, &ctx](const std::string name, std::size_t max, auto& q, auto newcount, const auto& rb){

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

        ImGui::Separator();
        ImGui::Text("Concurrency");
        drawSizeAndUniqueness(" genJobQ",ctx->maxGenJobsPerFrame,gen.genJobQueue,ctx->genJobsThisFrame, ctx->rb_genJobsAdded);
        drawSizeAndUniqueness(" genResQ",ctx->maxGenUploadsPerFrame,gen.genResultQueue,ctx->genResultsThisFrame,ctx->rb_genJobsAdded);

        drawSizeAndUniqueness("meshJobQ",ctx->maxMeshJobsPerFrame,mesher.meshJobQueue,ctx->meshJobsThisFrame,ctx->rb_meshJobsAdded);
        drawSizeAndUniqueness("meshResQ",ctx->maxMeshUploadsPerFrame, mesher.meshResultQueue,ctx->meshResultsThisFrame,ctx->rb_meshResultsAdded);


        ImGui::Separator();
        ImGui::Text("World Data");
        ImGui::Text("Chunks meshed: %lu", ctx->chunksMeshed);
        ImGui::Text("Loaded generated chunks: %lu", ctx->world.chunkMap.entries.size());
        ImGui::Text("Chunks with pending reads: %lu", ctx->world.chunkMap.pendingWritesMap.size());
        {
            std::size_t successful = ctx->world.chunkMap.pendingWritesSuccessful;
            std::size_t  attempted = ctx->world.chunkMap.pendingWritesAttempted;
            ImGui::Text("Pending chunk writes completed: %lu/%lu", successful, attempted);
        }
        // TODO: mesh queue is getting oversaturated
    }
    ImGui::End();
}
