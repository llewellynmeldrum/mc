#include "DebugUI.hpp"
#include "Assertion.hpp"
#include "ChunkConcurrency.hpp"
#include "ChunkConstants.hpp"
#include "Context.hpp"
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

constexpr ImVec2 PAD = {5.0f, 25.0f};
constexpr ImVec2 ALIGN_TOP_LEFT = {0.0f,0.0f};
constexpr ImVec2 ALIGN_TOP_RIGHT = {1.0f,0.0f};
constexpr ImVec2 ALIGN_MID_MID = {0.5f,0.5f};
constexpr f32 UI_SCALE = 1.25;
namespace IG = ImGui;  // namespace alias for convinience
enum struct DrawMode{
    Center=0,
};
struct DebugDraw{
    DrawMode drawMode = DrawMode::Center;
    ImDrawList* d = nullptr;
    ImVec2 transform={};
    ImColor fillColor{};
    ImColor strokeColor{};
    void selectWindow(){
        d= IG::GetWindowDrawList();
    }
    bool fillEnabled = false;
    void fill(ImColor col){
        fillEnabled = true;
        fillColor = col;
    }
    void noFill(ImColor col){
        fillEnabled = false;
    }

    bool strokeEnabled = false;
    void stroke(ImColor col){
        strokeEnabled = true;
        strokeColor = col;
    }
    void noStroke(ImColor col){
        strokeEnabled = false;
    }
    void validate_context(){
        ASSERT_NEQ(d,nullptr);
    }

    // NOTE: ImGui expects CLOCKWISE winding order 
    void pushWindowTransform(){
        transform = ImGui::GetCursorScreenPos();
    }
    void popWindowTransform(){
        transform={};
    }
    void rect(vec2 c, vec2 extents, f32 thick=1.0f){
        validate_context();
        ImVec2 h = {
            extents.x*0.5f,
            extents.y*0.5f
        };
        ImVec2 tl = {
            c.x-h.x,
            c.y-h.y,
        };
        ImVec2 br = {
            c.x+h.x,
            c.y+h.y,
        };
        tl+=transform; br+=transform;
        if (fillEnabled){
            d->AddRectFilled(tl,br,fillColor);
        }
        if (strokeEnabled){
            d->AddRect(tl,br,strokeColor,thick);
        }
    }
    void circle(vec2 c, f32 r, f32 thick=1.0f){
        validate_context();
        ImVec2 tl = {
            c.x-r,
            c.y-r,
        };
        ImVec2 br = {
            c.x+r,
            c.y+r,
        };
        c.x+=transform.x; 
        c.y+=transform.y; 
        if (fillEnabled){
            d->AddCircleFilled({c.x,c.y},r,fillColor);
        }
        if (strokeEnabled){
            d->AddCircle({c.x,c.y},r,fillColor);
        }
    }
    void text(const std::string& str, vec2 ic, f32 fontSize=16.0f){
        validate_context();
        ImVec2 c = {ic.x,ic.y};
        c+=transform;
        ImVec2 he = IG::CalcTextSize(str.c_str())*0.5f;
        ImVec2 tm = {c.x-he.x,c.y};
        if (fillEnabled){
            d->AddText(tm,fillColor,str.c_str());
        }
    }
};
struct ChunkDebugger{
    DebugDraw& draw;
    bool lock_chunk_y_to_cam=true;
    vec3 camPos;
    i32 chunkY = 0;
    static constexpr i32 halfExtent = 32;
    static constexpr i32 ROWS = halfExtent*2;
    static constexpr i32 COLS = halfExtent*2;
    static constexpr i32 HEIGHT = halfExtent*2;
    static constexpr f32 chunkW = 20;
    static constexpr f32 chunkGap = 2;
    std::array<ChunkEntry*, ROWS*COLS*HEIGHT> vals;

    decltype(auto) at(this auto&& self, i32 x, i32 y, i32 z){
        return self.span()[x+halfExtent,y+halfExtent,z+halfExtent];
    }
    decltype(auto) at(this auto&& self, i32 x, i32 z){
        return self.at(x,self.chunkY,z);
    }
    decltype(auto) at(this auto&& self, WorldChunkCoord v){
        return self.at(v.x,v.y,v.z);
    }
    decltype(auto) span(this auto&& self){
        return std::mdspan(self.vals.data(),ROWS,HEIGHT, COLS);

    }
    void updateChunkGrid(Simulation* ctx){
        const auto& coordList = ctx->world.generatedChunkCoordsInRadius({0,chunkY,0}, 8, 100);
        for (const auto& coord: coordList){
            if (coord.y<halfExtent && coord.y>=-halfExtent &&
                coord.x<halfExtent && coord.x>=-halfExtent &&
                coord.z<halfExtent && coord.z>=-halfExtent){
                at(coord) = ctx->world.chunkMap.get_entry(coord);
            }
        }
        camPos = toWorldChunkCoord(ctx->cam.pos);
        if (lock_chunk_y_to_cam){
            chunkY=camPos.y;
        }

    }
    ImColor defaultColor = ImColor(92, 89, 85);
    ImColor generatingColor = ImColor(184, 165, 140);
    ImColor generatedColor = ImColor(133, 81, 12);
    ImColor meshingColor = ImColor(255, 162, 0);
    ImColor meshedColor = ImColor(50, 255, 25);
    void showPreview(){
        draw.selectWindow();
        draw.pushWindowTransform();
        vec2 pos = {35,35};
        #define showColorPreview(name)\
        draw.fill(name);\
        draw.rect(pos,{chunkW,chunkW});\
        draw.fill(ImColor{255,255,255,255});\
        draw.text(#name, {pos.x+chunkW+chunkW, pos.y-12}, 12.0f);\
        pos.y+=chunkW+chunkGap;\

        showColorPreview(defaultColor)
        showColorPreview(generatingColor)
        showColorPreview(generatedColor)
        showColorPreview(meshingColor)
        showColorPreview(meshedColor)

    }
    void showChunkGrid(){
        draw.selectWindow();
        draw.pushWindowTransform();
        draw.transform += IG::GetWindowSize()*.5;
        
        draw.fill(ImColor(255,120,255));
        draw.stroke(ImColor(255,255,255));
        // somethings wrong with the transforms, probably some shit with the negativse.
        for (const auto [ix,iz] :EachInRange(-15,halfExtent, -halfExtent,halfExtent)){
            auto px = ix * chunkW + ix*chunkGap;
            auto py = iz * chunkW + iz*chunkGap;
            const auto& status = at(ix,iz)->status;
            draw.fill(defaultColor);
            if (at(ix,iz)){
                if (status.isGenerating()){
                    draw.fill(generatingColor);
                }
                if (status.isGenerated()){
                    draw.fill(generatedColor);
                } 
                if (status.isMeshing()){
                    draw.fill(meshingColor);
                } 
                if (status.isMeshed()){
                    draw.fill(meshedColor);
                }
            }
            draw.rect({px,py},{chunkW,chunkW});
        }
        draw.fill(ImColor(255,0,0));
        for (const auto ix :EachInRange(0,COLS)){
            auto px = ix * chunkW + ix*chunkGap;
            auto py = 50;
            draw.text(std::format("{}",ix-halfExtent),{px,py});
        }
        for (const auto iz :EachInRange(0,COLS)){
            auto py = iz * chunkW + iz*chunkGap;
            auto px = 50;
            draw.text(std::format("{}",iz-halfExtent),{px,-py});
        }
        draw.fill(ImColor(255,0,0));
        auto toscreen =[](auto v){
            auto px = v.x * chunkW + v.x*chunkGap;
            auto py = v.z * chunkW + v.z*chunkGap;
            return vec2{px,py};
        };
        draw.circle(toscreen(camPos),50);
    }
};
void DebugUI::drawUI() {
    auto* ctx = static_cast<Simulation*>(glfwGetWindowUserPointer(win_ptr));
    ShowFullscreenOverlay(ctx);
    ShowMainOverlay(ctx);
    ShowChunkDebugger(ctx);
}

DebugDraw draw;
ChunkDebugger dbg(draw);

void DebugUI::setupDebugUI(GLFWwindow* _win_ptr) {
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
    IG::NewFrame();
    drawUI();
}
void DebugUI::render() {
    IG::Render();
    ImGui_ImplOpenGL3_RenderDrawData(IG::GetDrawData());
}

void DebugUI::updateUI() {
    auto* ctx = static_cast<Simulation*>(glfwGetWindowUserPointer(win_ptr));
    dbg.updateChunkGrid(ctx);
    auto& io = IG::GetIO();
}

void DebugUI::destroyDebugUI() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    IG::DestroyContext();
}

void DebugUI::ShowFullscreenOverlay(Simulation* ctx) {
    auto io = IG::GetIO();
    if (ctx->isPaused()){
        auto* d = IG::GetForegroundDrawList();
        auto display_size = io.DisplaySize;
        d->AddRectFilled({0,0}, display_size, IM_COL32(20, 20, 20, 64));
        const char* text = "PAUSED";
        ImVec2 text_size = ImGui::CalcTextSize(text) * 2.5f; // default font size

        ImVec2 text_pos = ImVec2(
            (display_size.x - text_size.x) * 0.5f,
            (display_size.y - text_size.y) * 0.5f
        );

        d->AddText(text_pos, IM_COL32_WHITE, text);
    }
}
void DebugUI::ShowChunkDebugger(Simulation* ctx) {
    if (dbg.lock_chunk_y_to_cam){
        dbg.camPos= {
            ctx->cam.pos.x / Chunk_Extents.x,
            ctx->cam.pos.y / Chunk_Extents.y,
            ctx->cam.pos.z / Chunk_Extents.z,
        };
    }


    constexpr ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;


    auto&         io = IG::GetIO();
    const auto* viewport = IG::GetMainViewport();
    ImVec2 SCREEN_POS = viewport->WorkPos;
    ImVec2 SCREEN_SIZE = viewport->WorkSize;
    auto win_sz = SCREEN_SIZE * ImVec2{.4f,.5f};
    IG::SetNextWindowSize(win_sz,ImGuiCond_Always);

    IG::SetNextWindowPos( ImVec2{SCREEN_SIZE.x -win_sz.x-PAD.x,PAD.y}, ImGuiCond_Always, ALIGN_TOP_LEFT);

    if (IG::Begin("ChunkDebugger", nullptr, window_flags)) {
        ImVec2 avail = IG::GetContentRegionAvail();

        float spacing = IG::GetStyle().ItemSpacing.x;

        // Child windows relative to parent content size.
        float left_w  = avail.x * 0.25f;
        float right_w = avail.x - left_w - spacing;
        float child_h = avail.y;

        IG::Text("neither");
            {
                IG::BeginChild(
                    "chunk-debug-left-panel",
                    ImVec2{ left_w, child_h },
                    true // border
                );
                IG::Text("left panelkdajhf;dksjlfskfjlkjfakjflskjdklsfkljsdlkfjsdkl");
                if (IG::Checkbox("Lock map chunkY to cam pos?", &dbg.lock_chunk_y_to_cam)) {
                    std::println("Checkbox state: {}", dbg.lock_chunk_y_to_cam ? "true" : "false");
                }
                IG::BeginDisabled(dbg.lock_chunk_y_to_cam);
                IG::SliderInt("chunkY",&dbg.chunkY,-16, +16);
                IG::EndDisabled();
                dbg.showPreview();


                IG::EndChild();
            }

            IG::SameLine();
            {
                IG::BeginChild(
                    "chunk-debug-minimap-panel",
                    ImVec2{right_w, child_h},
                    true // border
                );
                IG::Text("minimap panel");
                dbg.showChunkGrid();

                
                draw.popWindowTransform();
            }
            IG::EndChild();
    }
    IG::End();
}
void DebugUI::ShowMainOverlay(Simulation* ctx) {
    ImGuiIO&         io = IG::GetIO();
    UI::WinFlags window_flags = UI::WinFlags::NoDecoration | UI::WinFlags::NoDocking |
                                    UI::WinFlags::AlwaysAutoResize | UI::WinFlags::NoSavedSettings |
                                    UI::WinFlags::NoFocusOnAppearing | UI::WinFlags::NoNav | UI::WinFlags::NoMove;
    const ImGuiViewport* viewport = IG::GetMainViewport();
    ImVec2 work_pos = viewport->WorkPos;  // Use work area to avoid menu-bar/task-bar, if any!
    ImVec2 work_size = viewport->WorkSize;
    ImVec2 window_pos, window_pos_pivot;
    window_pos.x = (work_pos.x + PAD.x);
    window_pos.y = (work_pos.y + PAD.y);
    window_pos_pivot.x = 0.0f;
    window_pos_pivot.y = 0.0f;
    IG::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
    IG::SetNextWindowViewport(viewport->ID);
    IG::SetNextWindowBgAlpha(0.35f);  // Transparent background
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
        IG::Text("DEBUG OVERLAY");

        IG::Separator();
        IG::Text("Positions:");
        IG::Text("World: %+03.1f,%+03.1f,%+03.1f", ctx->cam.pos.x, ctx->cam.pos.y, ctx->cam.pos.z);
        IG::Text("Chunk:%+3d,%+3d,%+3d", ch_pos.x, ch_pos.y, ch_pos.z);
        IG::Text("cam.pitch|yaw: %03.1f, %03.1f", ctx->cam.pitch, ctx->cam.yaw);
        IG::Text("Facing: %s", facing_str.c_str());

        IG::Separator();
        IG::Text("Performance:");
        const auto& fps_rb = ctx->time.ringbufs.at("frame");

        auto k =std::max(1.0f, fps_rb.n_percent_high(1.0));
        assert(k!=0);
        std::string one_pcnt_low = std::format("1% low: {:2.1f}", 1000.0/k);
        IG::Text("FPS: %2.1lf", 1000.0/fps_rb.avg());
        IG::SameLine(); IG::Text("(%s)",one_pcnt_low.c_str());

        auto plotFrametime = [](const auto& rb_map, const std::string key="????"){
            auto ft_rb = rb_map.at(key);
            // todo find longest string and make it the field length
            IG::Text("%-*s: %2.2lfms", 10, key.c_str(), ft_rb.avg());
            IG::SameLine();
            std::string id = "##"+key;
            IG::PlotLines(id.c_str(), ft_rb.data(), ft_rb.size(), 0);
        };
        for (const auto& [key, val]: ctx->time.ringbufs){
            plotFrametime(ctx->time.ringbufs,  std::string(key));
        }

        IG::Text("vsync: %s", ctx->win.enable_vsync ? "enabled" : "disabled");

        IG::Separator();
        IG::Text("Per frame draw info:");
        IG::Text("Vertex Count: %llu", ctx->rend.debug.vertex_count);
        IG::Text("Draw Calls: %llu", ctx->rend.debug.draw_calls);
        IG::Text("Mesh Count: %llu", ctx->rend.debug.mesh_count);

        IG::Separator();
        IG::Text("Process metrics");
        IG::Text("Resident Set Size: %5.2lfmb", current_rss_bytes()/1024.0/1024.0);

        auto& gen = ctx->world.chunkMap.generator;
        auto& mesher = ctx->rend.mesher;
        auto showSizeUnique = [](auto name, auto& q){

            auto total_sz = q.wait_size();
            auto unique_sz = q.uniqueSet.size();
            std::string pct ="all ";
            if (total_sz!=unique_sz){
                pct = std::format("{:4.2f}%",
                    ((f64)unique_sz/total_sz)*100.0
                );
            }

            IG::Text("%s size : %lu (%s unique)", name, total_sz, pct.c_str());
        };
        #define ShowSizeUnique(name) showSizeUnique(#name ,name)

        IG::Separator();
        IG::Text("Concurrency");
        ShowSizeUnique(gen.genJobQueue);
        ShowSizeUnique(gen.genResultQueue);

        ShowSizeUnique(mesher.meshJobQueue);
        ShowSizeUnique(mesher.meshResultQueue);


        IG::Separator();
        IG::Text("World Data");
        IG::Text("Chunks meshed: %lu", ctx->chunksMeshed);
        IG::Text("Loaded generated chunks: %lu", ctx->world.chunkMap.entries.size());
        IG::Text("Chunks with pending reads: %lu", ctx->world.chunkMap.pendingWritesMap.size());
        {
            std::size_t successful = ctx->world.chunkMap.pendingWritesSuccessful;
            std::size_t  attempted = ctx->world.chunkMap.pendingWritesAttempted;
            IG::Text("Pending chunk writes completed: %lu/%lu", successful, attempted);
        }
        // TODO: mesh queue is getting oversaturated
    }
    IG::End();
    struct Segment {
        vec3 s, e;
    };
    struct Line3D {
        mat4    vp;
        Camera& cam;
        Window& win;
        vec3    start;
        vec3    end;
        Line3D(mat4 vp, Camera& cam, Window& win, vec3 start, vec3 end)
            : vp(vp), cam(cam), win(win), start(start), end(end) {
            constexpr f32 segment_length = 0.01;
            size_t        seg_count = distance(start, end) / segment_length;
            vec3          s = start;
            vec3          e = start;

            for (size_t i = 0; i < seg_count; i++) {
                e += end * (i / (f32)seg_count);
                segments.emplace_back(s, e);
                s = e;
            }
        }
        std::vector<Segment> segments;

        f32 w2s(vec3 wpos, f32 wrad) {
            auto ss = w2s(wpos);
            auto es = w2s(wpos + cam.getRight() * wrad);
            if (!ss || !es)
                return 0;
            return distance(*ss, *es);
        }

        std::optional<vec2> w2s(vec3 w) {
            vec4 w4 = vp * vec4(w, 1.0);
            if (w4.w <= 0.0f)
                return std::nullopt;
            vec3 ndc = vec3{ w4.x, w4.y, w4.z } / w4.w;
            // -1 : 1
            // 0->W
            f32 x = ((ndc.x + 1.0) * win.px_w * 0.5);
            f32 y = ((1.0 - ndc.y) * win.px_h * 0.5);
            return std::make_optional<vec2>(x, y);
        }
        inline void draw_seg(ImDrawList* d, dvec3 s, vec3 e, i32 color = 0xFF00FFFF,
                             f32 thick = 3.0) {
            auto ss = w2s(s);
            auto es = w2s(e);
            if (!ss || !es)
                return;
            d->AddLine({ ss->x, ss->y }, { es->x, es->y }, color, thick);
        }
        inline void ig_draw(f32 wthick) {
            // split the line into a bunch of lines and render those
            auto* d = IG::GetForegroundDrawList();
            int   i = 0;
            for (const auto& s : segments) {
                f32 scr_thick = std::min(scr_thick, w2s(s.s + s.e, wthick));
                draw_seg(d, s.s, s.e, ImColor(i, i, i), scr_thick);
                i++;
            }
        }
        inline void ig_drawcircle(vec3 pos, f32 rad) {
            const auto& d = IG::GetForegroundDrawList();
            auto        ss = w2s(pos);
            f32         scr_rad = w2s(pos, rad);
            if (!ss)
                return;
            d->AddCircleFilled({ ss->x, ss->y }, scr_rad, 0xFF00FFFF);
        }
    };
    mat4 vp = ctx->cam.getProjectionMatrix() * ctx->cam.getViewMatrix();
    /*
    Line3D beacon(vp, ctx->cam, ctx->win, { 0, 0, 0 }, { 0, 1000, 0 });
    vec3   s = beacon.start;
    vec3   e = beacon.end;

    vec3 mid = s + e;
    mid *= 0.5;

    beacon.ig_drawcircle(mid, 100);
    beacon.ig_draw(3.0);
    */
}
