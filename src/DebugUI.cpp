#include "DebugUI.hpp"
#include "Context.hpp"
#include "DebugFormat.hpp"
#include "GLFW/glfw3.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_stdlib.h"

#include "GLFWWrapper.hpp"
namespace IG = ImGui;  // namespace alias for convinience
void
DebugUI::drawUI() {
    ShowOverlay(nullptr);
}
void
DebugUI::setupDebugUI(GLFWwindow* _win_ptr) {
    this->win_ptr = _win_ptr;
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    IG::CreateContext();
    ImGuiIO& io = IG::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;      // IF using Docking Branch
    float main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());
    IG::StyleColorsDark();

    ImGuiStyle& style = IG::GetStyle();
    style.ScaleAllSizes(
        main_scale * 1.5);  // Bake a fixed style scale. (until we have a solution for dynamic style
                            // scaling, changing this requires resetting Style + calling this again)
    style.FontScaleDpi = main_scale * 1.5;

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(win_ptr, true);
    // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();
}

void
DebugUI::drawDebugUI() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    IG::NewFrame();
    drawUI();
}
void
DebugUI::render() {
    IG::Render();
    ImGui_ImplOpenGL3_RenderDrawData(IG::GetDrawData());
}

void
DebugUI::destroyDebugUI() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    IG::DestroyContext();
}

void
DebugUI::ShowOverlay(bool* p_open) {
    auto*            ctx = static_cast<Context*>(glfwGetWindowUserPointer(win_ptr));
    ImGuiIO&         io = IG::GetIO();
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking |
                                    ImGuiWindowFlags_AlwaysAutoResize |
                                    ImGuiWindowFlags_NoSavedSettings |
                                    ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
    const float      PAD = 10.0f;
    const ImGuiViewport* viewport = IG::GetMainViewport();
    ImVec2 work_pos = viewport->WorkPos;  // Use work area to avoid menu-bar/task-bar, if any!
    ImVec2 work_size = viewport->WorkSize;
    ImVec2 window_pos, window_pos_pivot;
    window_pos.x = (work_pos.x + PAD);
    window_pos.y = (work_pos.y + PAD);
    window_pos_pivot.x = 0.0f;
    window_pos_pivot.y = 0.0f;
    IG::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
    IG::SetNextWindowViewport(viewport->ID);
    window_flags |= ImGuiWindowFlags_NoMove;
    IG::SetNextWindowBgAlpha(0.35f);  // Transparent background
    std::string facing_str{};
    const vec3& facing = ctx->cam.front;
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

    auto ch_pos = World::worldToChunkCoord(ctx->cam.pos);
    if (IG::Begin("Debug Overlay", p_open, window_flags)) {
        IG::Text("DEBUG OVERLAY");

        IG::Separator();
        IG::Text("Positions:");
        IG::Text("World: %+03.1f,%+03.1f,%+03.1f", ctx->cam.pos.x, ctx->cam.pos.y, ctx->cam.pos.z);
        IG::Text("Chunk:%+3d,%+3d,%+3d", ch_pos.x, ch_pos.y, ch_pos.z);
        IG::Text("cam.front: %03.1f,%03.1f,%03.1f", ctx->cam.front.x, ctx->cam.front.y,
                 ctx->cam.front.z);
        IG::Text("Facing: %s", facing_str.c_str());

        IG::Separator();
        IG::Text("Performance:");
        const auto& fps_rb = ctx->time.framerate_ringbuf;
        std::string one_pcnt_low = std::format("1% low: {:2.1f}", fps_rb.n_percent_low(1.0));
        IG::Text("Frametime: %2.2lfms", 1000.0 / fps_rb.avg());
        IG::Text("FPS: %2.1lf", fps_rb.avg());
        IG::SameLine();
        IG::PlotLines("##frameratePlot", fps_rb.data(), fps_rb.size(), 0, one_pcnt_low.c_str());

        IG::Separator();
        IG::Text("Per frame draw info:");
        IG::Text("Vertex Count: %llu", ctx->rend.debug.vertex_count);
        IG::Text("Draw Calls: %llu", ctx->rend.debug.draw_calls);
        IG::Text("Mesh Count: %llu", ctx->rend.debug.mesh_count);
    }
    IG::End();
}
