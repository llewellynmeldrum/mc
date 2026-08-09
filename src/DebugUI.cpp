#include <optional>
#include <print>
#include <mdspan>
#include <concepts>
#include <string>


#include "ChunkState.hpp"
#include "DebugChunkRenderer.hpp"
#include "GlobalDebugLog.hpp"
#include "LM.hpp"
#include "WorldGen_BiomeBlockPalettes.hpp"
#include "WorldGen_BiomeClassification.hpp"
#include "WorldGen_NoiseGeneration.hpp"
#include "WorldGen_SharedBiomeFeatures.hpp"
#include "cpp23_ranges.hpp"

#include "ChunkEntry.hpp"
#include "DebugChunkLog.hpp"
#include "DebugFormat.hpp"

#include "DebugOptions.hpp"
#include "FmtStyle.hpp"
#include "PendingBlockWrites.hpp"
#define DISABLE_STYLE
#include "FormatSpecs.hpp"
#undef DISABLE_STYLE

#include "LM_Metaprogramming.hpp"
#include "DebugUI.hpp"
#include "Assertion.hpp"
#include "ChunkConcurrency.hpp"
#include "Window.hpp"
#include "ChunkConstants.hpp"
#include "Engine.hpp"
#include "CoordTypes.hpp"
#include "CommonConcepts.hpp"
#include "ImGuiWrapper.hpp"
#include "glmWrapper.hpp"
#include "GLFWWrapper.hpp"
#include "ChunkMap.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "imgui.h"

#include "DebugUI_Internal.hpp"
#include "Assertion.hpp"

using namespace glm;
namespace IG = ImGui;  // namespace alias for convinience

f32 g_UI_SCALE = 1.0f;
std::vector<WindowConfig> win_configs;



DebugUI::DebugUI(SkyboxConfig initial_cfg){
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
    skybox_cfg = std::move(initial_cfg);
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
//    auto* ctx = static_cast<Engine*>(glfwGetWindowUserPointer(win_ptr));
}

template<typename T>
static bool edit_enum(const char* label, T* selected, const_span<char const*> names){
    int * p = static_cast<int*>(static_cast<void*>(selected));
    return IG::Combo(label,p, names.data(),names.size());
}
static bool edit_noise_config(const char* label, NoiseConfig& c) {
    if (!ImGui::TreeNode(label)) return false;
    bool changed = false;
    changed |= ImGui::DragInt  ("seed offset",        &c.seed_offset);
    changed |= ImGui::DragFloat("freq",        &c.freq, 0.0001f, 0.0f, 0.05f, "%.5f");
    changed |= ImGui::SliderInt("octaves",     &c.frac_octaves, 1, 8);
    changed |= ImGui::SliderFloat("lacunarity",&c.frac_lacunarity, 1.0f, 4.0f);
    changed |= ImGui::SliderFloat("persistence",&c.frac_persistence, 0.0f, 1.0f);
    // enums - combos 
    changed |= edit_enum("type",    &c.type,      NoiseType_names);
    changed |= edit_enum("fractal", &c.frac_type, FractalType_names);
    ImGui::TreePop();
    return changed;
}
template<typename ResType>
bool draw_remap_table(const char* label, GenericRemapTable<ResType> & remap_table){
    bool changed = false;
    if (!ImGui::TreeNode(label)) return false;

    auto& map = remap_table.map;
    int deleteIdx = -1;
    for (int i = 0; i < (int)remap_table.size(); i++) {
        ImGui::PushID(i);
        changed |= ImGui::SliderFloat("##key", &map[i].first, 0.0f,1.0f);
        ImGui::SameLine();
        changed |= ImGui::ColorEdit3("##val", &map[i].second.x,ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_Float);
        ImGui::SameLine();
        if (ImGui::SmallButton("x")) deleteIdx = i;
        ImGui::PopID();
    }
    if (deleteIdx >= 0) {
        map.erase(map.begin() + deleteIdx); 
        changed = true; 
    }
    if (ImGui::SmallButton("+ add")) {
        map.push_back(map.empty() ? std::pair{0.0f, ResType{}} : map.back());
        changed = true;
    }
    if (changed && map.size()>1){
        std::stable_sort(map.begin(), map.end(),
                         [](auto& a, auto& b){ return a.first < b.first; });
    }
    ImGui::TreePop();
    return changed;
}
#include "meta_wrapper.hpp"
void draw_world_window(WindowConfig& self, Engine* ctx){
    self.setAlpha(0.9f);
    self.setup();
    self.setSize(UVSize{0.4,0.4});
    self.setAlign(WinAlign::TopMid());
    self.setFlags();
    self.start_at(true, UVPos{0.7,0.5},[&]{
        auto& window = self;
        auto cfg = ctx->get_skybox_cfg();
        window.open_section("Noise Config:",[&]{
//            window.slider("Ticks per second",&ctx->ticksPerSecond,TickCount{1},TickCount{2000});

            bool dirty= false;

            f32 t = cfg.tod01(); 
            bool& pause_tod_progression = ctx->tick_updates_paused;
            static f32 tod_min{0.05};
            static f32 tod_max{0.50};
            static f32 tod_len_seconds{30.0f};
            if (!DebugOption::skybox_ui_override) IG::BeginDisabled();
            window.slider("TOD min:", &tod_min,0.7f,1.0f);
            window.slider("TOD max:", &tod_max,0.0f,1.0f);
            window.slider("S/ day:", &tod_len_seconds,0.0f,120.0f);
            window.checkbox("Pause", &pause_tod_progression);

                if (window.slider("Time of day (t)",&t,0.0f,1.0f)){
                    dirty |= true;
                    cfg.tick_count = t * cfg.ticks_per_day;
                }

            if (!pause_tod_progression){
                auto dt = IG::GetIO().DeltaTime;
                f32 day_length = tod_len_seconds;
                t += dt / day_length;
                if (t<tod_min) t= tod_min;
                if (t>tod_max) t= tod_min;
                cfg.tick_count = t*cfg.ticks_per_day;
                dirty = true;
            }

            


            if (!DebugOption::skybox_ui_override) IG::EndDisabled();
            if (dirty){
                ctx->ui.skybox_cfg = cfg;
            }
        });
    });
}
void draw_worldgen_window(WindowConfig& self, Engine* ctx){
    self.setAlpha(0.9f);
    self.setup();
    self.setSize(UVSize{0.4,0.4});
    self.setAlign(WinAlign::TopMid());
    self.setFlags();
    self.start_at(true, UVPos{0.7,0.5},[&]{
        auto& window = self;
        auto& cfg = ctx->world.editable_cfg;
        window.open_section("Noise Config:",[&]{
            [[maybe_unused]] bool dirty = false;

            dirty |= IG::DragInt("sea level", &cfg.sea_level, 1, 0, 256);
            dirty |= IG::InputInt("seed",     &cfg.world_seed);
            IG::SameLine();
            if (IG::Button("(randomize)")){
                cfg.world_seed = random();
                dirty = true;
            }
            ImGui::SeparatorText("Noise channels");
            #define X(name) dirty |= edit_noise_config(#name, cfg.name ##_cfg);
                LIST_NOISE_PARAMS
            #undef X
        });
    });
}
void draw_graphics_window(WindowConfig& self, Engine* ctx){
    self.setAlpha(0.9f);
    self.setup();
    self.setSize(UVSize{0.4,0.4});
    self.setAlign(WinAlign::TopMid());
    self.setFlags();
    self.start_at(true, UVPos{0.7,0.5},[&]{
        auto& window = self;
        bool dirty = false;
        dirty |= window.dbg_toggle(DebugOption::show_lighting_system);
        dirty |= window.dbg_toggle(DebugOption::draw_blocklight);
        dirty |= window.dbg_toggle(DebugOption::fade_in_chunks);
        dirty |= window.dbg_toggle(DebugOption::enable_fog);
        dirty |= window.dbg_toggle(DebugOption::draw_sunlight);
        dirty |= window.slider("gamma",&ctx->rend.gamma, 0.0f, 2.0f);
        dirty |= window.checkbox("enable smooth light falloff", &(ctx->rend.enable_smooth_light_falloff));
        IG::BeginDisabled(!ctx->rend.enable_smooth_light_falloff);
            dirty |= window.slider<f32>("smooth light falloff (sunlight)", &(ctx->rend.sunlight_smooth_falloff_factor), 0, 1);
            dirty |= window.slider<f32>("smooth light falloff (blocklight)", &(ctx->rend.blocklight_smooth_falloff_factor), 0, 1);
        IG::EndDisabled();
        if (dirty) ctx->rend.update_debug_uniforms();
    });
}
void drawDebugSettingsWindow(WindowConfig& self, Engine* ctx){
    self.setAlpha(0.9f);
    self.setup();
    self.setSize(UVSize{0.4,0.4});
    self.setAlign(WinAlign::TopMid());
    self.setFlags();
    self.start_at(true, UVPos{0.7,0.5},[&]{
        auto& window = self;

        
        window.open_section("DebugOption::",[&]{
            edit_enum("Debug chunk mode", &DebugOption::render_state_mode, DebugOption::DebugRenderStateTarget_names);
            window.dbg_toggle(DebugOption::outline_neighbour_boundaries);
            window.dbg_toggle(DebugOption::enable_3d_debug_visuals);
            window.dbg_toggle(DebugOption::fill_neighbour_boundaries);
            window.dbg_toggle(DebugOption::outline_all_boundaries);
            window.dbg_toggle(DebugOption::fill_all_boundaries);

            window.dbg_toggle(DebugOption::show_debug_ui);
            window.dbg_toggle(DebugOption::HIDE_AIR_CHUNKS);
            window.dbg_toggle(DebugOption::HIDE_CLEAN_CHUNKS);

            window.slider<u8>("Boundary fill opacity", &DebugOption::ChunkDebugFillOpacity, 0, 255);
            window.slider<f32>("Boundary outline opacity",&DebugOption::ChunkDebugOutlineOpacity, 0.0f, 1.0f);

            window.dbg_opt_slider(DebugOption::player_cam_vfov);
//            window.slider<f32>("Cam vertical fov",&DebugOption::player_cam_vfov, 10.0f, 90.0f);
        });
    });
}

void drawGlobalLogWindow(WindowConfig& self, Engine* ctx){
    self.setAlpha(0.9f);
    self.setup();
    self.setSize(UVSize{0.4,0.4});
    self.setAlign(WinAlign::TopMid());
    self.setFlags();
    self.start_at(true, UVPos{0.7,0.5},[&]{
        auto& window = self;
        window.open_section("##",[&]{
            // Shown in most->least recent vertical order
            for (const auto& entry: global_logger | ranges::views::reverse){
                auto [log_type, duration, contents] = entry;
                UI::Text("{}",entry);
            }
        });
    });
}
void drawPerChunkLogWindow(WindowConfig& self, Engine* ctx){
    self.setAlpha(0.9f);
    self.setup();
    self.setSize(UVSize{0.4,0.4});
    self.setAlign(WinAlign::TopMid());
    self.setFlags();
    self.start_at(true, UVPos{0.7,0.5},[&]{
        auto& window = self;
        for (auto& [log_type, enabled]: is_log_type_enabled ){
            window.checkbox(std::format("{}",log_type), &enabled);
        }
        window.open_section("Per chunk log:",[&]{
            // Shown in most->least recent vertical order
            WorldChunkCoord cur_chunk = toWorldChunkCoord(ctx->player_cam.pos);
            auto lock = per_chunk_log.lock_guard();
            if (per_chunk_log.contains(cur_chunk)){
                for (const auto& entry: per_chunk_log.at(cur_chunk) | ranges::views::reverse){
                    auto [log_type, duration, contents] = entry;
                    auto it = is_log_type_enabled.find(log_type);
                    if (it == is_log_type_enabled.end()){
                        LOG_WARN("log type '{}' does not exist in the is_log_type_enabled LUT.",log_type);
                    }else if(it->second){
                        UI::Text(DebugLog::entry_tostr(entry));
                    }
                }
            }
        });
    });
}

inline ImColor stroke_color{0,0,0,255};
inline ImColor fill_color{255,255,255,255};

inline bool enable_stroke{false};
inline bool enable_fill{true};

inline f32 font_scale {1.0f};


struct DrawContext{
    ImGuiIO& io = IG::GetIO();
    ImDrawList* d = IG::GetForegroundDrawList();
    ImVec2 screen_size = io.DisplaySize;
    static inline ImU32 fill(){
        if (enable_fill) return ImU32(fill_color);
        else return 0x00'00'00'00;
    }
    static inline ImU32 stroke(){
        if (enable_stroke) return ImU32(stroke_color);
        else return 0x00'00'00'00;
    }
    inline f32 width(){
        return screen_size.width();
    }
    inline f32 height(){
        return screen_size.height();
    }
    static inline f32 font_size(){
        return font_scale * 13.0f;
    }
};
void draw_rect(ImVec2 tl_pos, ImVec2 size, f32 thick=1.0f){
    DrawContext ctx;
    ImVec2 min = tl_pos;
    ImVec2 max = tl_pos+size;
    ctx.d->AddRect(min, max, DrawContext::stroke(),0.0f,0,thick);
    ctx.d->AddRectFilled(min, max, DrawContext::fill(),0.0f,0);
}

void draw_text(std::string text, ImVec2 pos, bool left_align=true){
    DrawContext ctx;
    // if left_align = false, center is true
    ctx.d->AddText(ctx.io.FontDefault, DrawContext::font_size(), pos, DrawContext::stroke(), text.c_str());
}

void set_stroke_rgba( u8 r, u8 g, u8 b, u8 a=255){
    enable_stroke = true;
    stroke_color = {r,g,b,a};
}
void set_fill_rgba( u8 r, u8 g, u8 b, u8 a=255){
    enable_fill = true;
    fill_color = {r,g,b,a};
}

void draw_line(ImVec2 p1, ImVec2 p2, f32 thick=1.0f){
    DrawContext ctx;
    ctx.d->AddLine(p1, p2, DrawContext::stroke(),thick);
}
void draw_guides(){
    DrawContext ctx;
    auto w = ctx.width();
    auto h = ctx.height();
    auto hh = h * 0.5f;
    auto hw = w * 0.5f;
    auto old_stroke = stroke_color;
    set_stroke_rgba(255,0,0);
    draw_line({0,hh},{w,hh}, 1.0f);
    draw_line({hw,0},{hw,h}, 1.0f);
    stroke_color = old_stroke;
}
ImVec2 calc_text_size(std::string str){
    DrawContext ctx;
    return IG::CalcTextSize(str.c_str()) * font_scale / g_UI_SCALE;
}

void drawFullscreenOverlay(WindowConfig& self, Engine* ctx) {
    // TODO: 
    // 1. refactor this slightly to make it less horrific and ugly
    //  - eg. UI::DrawText(str, pos, font_size, bool show_outlines=true)
    // 2. fix pending writes
    auto io = IG::GetIO();
    auto screen_size = io.DisplaySize;
    font_scale = 2.0f;
    if (ctx->baking_starting_chunks){
        font_scale = 3.0f;
        set_fill_rgba(20,20,20,150);
        enable_stroke = false;
        draw_rect({0,0},screen_size);
        enable_stroke = true;
        std::string ch_upd_paused_str = "";
        auto ms = static_cast<i32>(ImGui::GetTime() * 1000.0f);
        if (ms % 1000 <= 250){
            ch_upd_paused_str = "BAKING STARTING CHUNKS";
        }else if (ms %1000 <=500){
            ch_upd_paused_str = "BAKING STARTING CHUNKS.";
        }else if (ms %1000 <=750){
            ch_upd_paused_str = "BAKING STARTING CHUNKS..";
        }else {
            ch_upd_paused_str = "BAKING STARTING CHUNKS...";
        }
        ImVec2 text_size = calc_text_size(ch_upd_paused_str);

        ImVec2 text_pos = ImVec2(
            (screen_size.x - text_size.x) * 0.5f,
            (screen_size.y - text_size.y) * 0.5f
        );

        set_stroke_rgba(255,255,255);
        draw_text(ch_upd_paused_str,text_pos);
        font_scale = 1.5f;
        draw_text("(press ESC to cancel)",text_pos + ImVec2(0,5+DrawContext::font_size()*2));
    }else if (ctx->paused){
        set_fill_rgba(20,20,20,64);
        enable_stroke = false;
        draw_rect({0,0},screen_size);
        enable_stroke = true;

        std::string paused_str = "PAUSED";
        ImVec2 text_size = calc_text_size(paused_str);

        ImVec2 text_pos = ImVec2(
            (screen_size.x - text_size.x) * 0.5f,
            (screen_size.y - text_size.y) * 0.5f
        );

        set_stroke_rgba(255,255,255);
        draw_text(paused_str,text_pos);
    }else if (ctx->chunk_updates_paused){

        set_fill_rgba(20,20,20,16);
        enable_stroke = false;
        draw_rect({0,0},screen_size);
        enable_stroke = true;
        std::string ch_upd_paused_str = "CHUNK UPDATES PAUSED";
        ImVec2 text_size = calc_text_size(ch_upd_paused_str);

        ImVec2 text_pos = ImVec2(
            (screen_size.x - text_size.x) * 0.5f,
            (screen_size.y - text_size.y) * 0.8f
        );

        set_stroke_rgba(255,255,255);
        draw_text(ch_upd_paused_str,text_pos);
    }else if (ctx->mouse_mode){

        set_fill_rgba(20,20,20,16);
        enable_stroke = false;
        draw_rect({0,0},screen_size);
        enable_stroke = true;
        std::string ch_upd_paused_str = "MOUSE MODE ENABLED";
        ImVec2 text_size = calc_text_size(ch_upd_paused_str);

        ImVec2 text_pos = ImVec2(
            (screen_size.x - text_size.x) * 0.5f,
            (screen_size.y - text_size.y) * 0.8f
        );

        f32 x = IG::GetTime()* 2.0f;
        i32 r = (120 + sin(2.11 + x*.4) * 120);
        i32 g = (120 + sin(1.57 + x*.3) * 120);
        i32 b = (120 + sin(1.23 + x*.2) * 120);

        set_stroke_rgba(r,g,b);
        draw_text(ch_upd_paused_str,text_pos);
    }

    _notif_logger.log.update();
    ImVec2 offset = {0, screen_size.y * 0.9f};
    for (const auto& entry: _notif_logger.log){
        auto& [_, __, msg] = entry;
        ImVec2 text_size = calc_text_size(msg);

        ImVec2 text_pos = {
            (screen_size.x*0.5f - text_size.x*.5f),
            (-text_size.y)
        };
        text_pos += offset;
//        auto red = IM_COL32(255,0,0,255);
        auto opacity01= _notif_logger.log.fading_entry_opacity01(entry);
        auto opacity = static_cast<i32>(std::lerp(0.0f,255.0f,opacity01));
        set_stroke_rgba(255,255,255,opacity);
        draw_text(msg,text_pos);
        offset.y -= text_size.y*1.5f; // line spacing
    }

//    draw_guides();
}


void drawSecondCameraWindow(WindowConfig& self, Engine* ctx) {
    self.setAlpha(0.65f);
    self.setup();
    f32 aspect  = ctx->fixedCamTarget.size.x / ctx->fixedCamTarget.size.y;
    f32 xwidth = ctx->fixedCamTarget.size.x/ctx->win.px_w * 2;
    self.setSize(UVSize{xwidth, xwidth * aspect});
    self.setAlign(WinAlign::TopMid());
    self.setFlags(UI::WinFlags::NoResize);
    self.start_at(true, UVPos{0.5,0},[&]{
        self.section("Secondary View:",[&]{
            UI::Text("scr: {}, {}",ctx->fixedCamTarget.pos, ctx->fixedCamTarget.size);
            UI::Text("  w: {}, {}",ctx->drone_cam.pos.raw(), ctx->drone_cam.ortho_zoom);
            UI::DrawTexture(ctx->fixedCamTarget);

        });
    });

    
}

template<JobType JT>
void draw_graph(Engine* ctx) {
    auto draw_sz_and_unique = [&]
        (const std::string name, size_t max, auto& q, auto newcount, const auto& rb){

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
    {
        std::string name = std::format("{}jobQ",JT);
        auto max = ctx->max_upload_per_frame<JT>;
        auto const&  q = ctx->job_queue<JT>();
        auto const this_frame = ctx->get_pf_accums<JT>().enqueues_this_frame;
        auto const& rb = ctx->rb_enqueues<JT>();
        draw_sz_and_unique(name, max,q,this_frame,rb);
    }

    {
        std::string name = std::format("{}resQ",JT);
        auto max = ctx->max_upload_per_frame<JT>;
        auto const&  q = ctx->res_queue<JT>();
        auto const this_frame = ctx->get_pf_accums<JT>().uploads_this_frame;
        auto const& rb = ctx->rb_uploads<JT>();
        draw_sz_and_unique(name, max,q,this_frame,rb);
    }


}
void drawGeneralDebugOverlay(WindowConfig& self, Engine* ctx) {
    self.setAlpha(0.65f);
    self.setup();
    self.setFlags(UI::WinFlags::AlwaysAutoResize);
    self.start_at(UVPos{0,0},[&self, ctx]{
        auto& window = self;
        const auto& fps_rb = AT(ctx->profiler.map,"frame");
        const auto k = std::max(1.0f, fps_rb.n_percent_high(1.0));
        assert(k!=0);
        const auto p99 = 1000.0/k;
        const auto fps = 1000.0/fps_rb.avg();
        const auto ft_ms = fps_rb.avg();

        const auto draw_ms = AT(ctx->profiler.map,"01_draw").avg();
        const auto upd_ms = AT(ctx->profiler.map,"update").avg();
        const auto upd_pcnt = 100.0 * upd_ms / ft_ms;
        const auto draw_pcnt = 100.0 * draw_ms / ft_ms;

        const auto n_chunks_loaded = ctx->rend.opaque_chunk_meshes.size();
        const auto n_chunks_meshing = ctx->mesh_counters.n_on_queue;
        const auto n_chunks_gening = ctx->gen_counters.n_on_queue;

        const auto pos = ctx->player_cam.pos;
        const auto round_pos = glm::ivec3{LM::floor(ctx->player_cam.pos).raw()};
        const auto ch_pos = toWorldChunkCoord(ctx->player_cam.pos);
        const auto cl_pos = glm::vec3{
            LM::ieuclid_mod(static_cast<i32>(pos.x),CHUNK_XWIDTH),
            pos.y,
            LM::ieuclid_mod(static_cast<i32>(pos.z),CHUNK_XWIDTH),
        };//toChunkBlockPos(ctx->player_cam.pos).raw();
        UI::Text("fps: {: 4.1f} (p99: {: 4.1f})",fps, p99);
        UI::Text("frametime: {: 4.1f}ms (upd: {: 3.1f}%, draw: {: 3.1f}%)", ft_ms,upd_pcnt,draw_pcnt);
        UI::Text("tick: {}",ctx->tick_count);
        UI::Text("draw dist: {}",ctx->director.RENDER_DIST);
        UI::Separator();
        UI::Text("meshed: {: 7}, +{: 7} generating, +{: 7} meshing",n_chunks_loaded,n_chunks_gening,n_chunks_meshing);
        UI::Text("entries : {: 7} ",ctx->world.chunkMap.entries.size());
        UI::Separator();
        UI::Text("pos :{: 4.1f},{: 4.1f},{: 4.1f} (B:{: 3},{: 3},{: 3})",
                 pos.x,pos.y,pos.z,std::floor(pos.x),std::floor(pos.y),std::floor(pos.z));
        auto* entry = ctx->director.chunk_map.entries.try_get(ch_pos);
        if (entry){
            ChunkBlockPos cl_pos_ = ChunkBlockPos{cl_pos};
            if (is_in_chunk(cl_pos_)){
                UI::Text("light data: {}",AT(entry->light_data.read(),cl_pos_));
            }else{
                UI::Text("light data: n/a, out of bounds?");
            }
        }else{
            UI::Text("light data: n/a, no entry");
        }
        UI::Text("chunk :{: 3},{: 3}",ch_pos.x,ch_pos.z);
        UI::Text("chunk local: {: 4.1f},{: 4.1f},{: 4.1f} (B:{: 3},{: 3},{: 3})",
                 cl_pos.x,cl_pos.y,cl_pos.z,std::floor(cl_pos.x),std::floor(cl_pos.y),std::floor(cl_pos.z));
        UI::Separator();
        UI::Text("ready4gen :{}",ctx->director.ready_for_gen.size());
        UI::Text("ready4mesh:{}",ctx->director.ready_for_mesh.size());
        UI::Text("ready4light:{}",ctx->director.ready_for_light.size());
        UI::Separator();

        if (!ctx->ui.is_ui_expanded){
            UI::Text("Press '`' to expand.");
            return;
        }
        std::string biome_str = "N/A (no chunk entry)";
        const auto & [wx,_,wz] = pos;
        NoiseParams noise_samples_exact = ctx->world.active_cfg.noise.sample_each(wx,wz);
        auto biome = classify_biome_verbose(noise_samples_exact);

        UI ::Text("{:>10}: {:+9.3f}", "hill", noise_samples_exact.hill);
        UI ::Text("{:>10}: {:+9.3f}", "mountain", noise_samples_exact.mountain);

        auto show_biome_score_table = [&](){
            UI::Separator("Biome Scores:");
            IG::BeginTable("##Table_thing",6,ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg);
            IG::TableNextRow();
            IG::TableNextColumn();
            UI::Text("Biome");
            IG::TableNextColumn();

        #define X(VAR) \
                UI::Text("{:c}({:+4.3f})",(#VAR[0]+('A'-'a')), noise_samples_exact. VAR); \
                IG::TableNextColumn();
            LIST_FUNDAMENTAL_NOISE_PARAMS
        #undef X
            UI::Text("Avg score");

            IG::TableNextRow();
            for (const auto& biome_match: biome_match_tables){
                BiomeClassification clas{};
                clas.update(biome_match,noise_samples_exact);
                IG::TableNextRow();
                IG::TableNextColumn();
                auto biome_str = std::format("{}",biome_match.biome);
                bool chosen = biome_match.biome == biome.id;
                ImGui::Selectable(biome_str.c_str(), chosen, ImGuiSelectableFlags_SpanAllColumns);
                IG::TableNextColumn();
        #define X(VAR) UI::Text("{: 5.1f}",clas.VAR ##_score);\
                IG::TableNextColumn();
            LIST_FUNDAMENTAL_NOISE_PARAMS
        #undef X
                 UI::Text("{: 5.2f}",clas.avg_score());
            }

            IG::EndTable();
        };
        show_biome_score_table();
        UI::Text("Round trip times per chunk (enqueue job to upload of result)");
        plotRingBuf(ctx->mesh_rtt_bencher.duration_ms, 10, "mesh rtt", "6.1lfms", true);
        plotRingBuf(ctx->gen_rtt_bencher.duration_ms, 10, "gen rtt", "6.1lfms", true);

        UI::Text("Work times per chunk (start job to finish job)");
        plot_benchmarker(ctx->mesh_work_bencher, 10, "mesh work", "6.1lfms", true);
        plot_benchmarker(ctx->gen_work_bencher, 10, "gen work", "6.1lfms", true);

        UI::Text("Time spent on job queue awaiting a worker");
        plot_benchmarker(ctx->mesh_job_queue_idle_bencher, 10, "MESH JOB IDLE", "6.1lfms", true);
        plot_benchmarker(ctx->gen_job_queue_idle_bencher, 10, "GNE JOB IDLE", "6.1lfms", true);

        UI::Text("Time spent on res queue awaiting main thread");
        plot_benchmarker(ctx->mesh_res_queue_idle_bencher, 10, "MESH RES IDLE", "6.1lfms", true);
        plot_benchmarker(ctx->gen_res_queue_idle_bencher, 10, "GEN RES IDLE", "6.1lfms", true);

        UI::Text("Time spent on res queue awaiting main thread");
        plotRingBuf(ctx->director.mesh_enqueue_delay_bencher.duration_ms, 10, "MESH ENQUEUE", "6.1lfms", true);
        plotRingBuf(ctx->director.gen_enqueue_delay_bencher.duration_ms, 10, "GEN ENQUEUE", "6.1lfms", true);
        UI::Text("Work times per chunk (start job to finish job)");
        plot_benchmarker(ctx->mesh_work_bencher, 10, "mesh work", "2.2lfms", true);
        plot_benchmarker(ctx->gen_work_bencher, 10, "gen work", "2.2lfms", true);


        UI::Text("Press '`' to minimize.");
        window.open_section("Perf:",[&]{
            UI::Text("Optimization lvl: -O{}",DebugOption::compiler_optimisation_level);
            IG::Text("vsync: %s", ctx->win.enable_vsync ? "enabled" : "disabled");

            const auto& fps_rb = AT(ctx->profiler.map,"frame");
            auto k =std::max(1.0f, fps_rb.n_percent_high(1.0));
            assert(k!=0);
            std::string one_pcnt_low = std::format("1% low: {:2.1f}", 1000.0/k);
            IG::Text("FPS: %2.1lf", 1000.0/fps_rb.avg());
            IG::SameLine(); IG::Text("(%s)",one_pcnt_low.c_str());

            for (const auto& [key, val]: ctx->profiler.map){
                plotRingBuf(val, 10, std::string(key), "2.2lfms", true);
            }

        });
        window.section("Chunk States:",[&]{
            IG::Separator();
            #define X(name) UI::ColoredText(PipelineStateOutlineColor(PipelineState :: name),\
                            "{}: {}", #name, ctx-> CONCAT(gen,_counters).n_##name);
                UI::Text("GenStates:");
                PIPELINE_STATE_LIST

                UI::Text("LightStates:");
                PIPELINE_STATE_LIST

                UI::Text("MeshStates:");
                PIPELINE_STATE_LIST

            #undef X

            UI::Text("Chunk Entries: {}",ctx->world.chunkMap.entries.size());
        });
        window.dropdown.show();


        window.section("Chunk Data:",[&]{
            auto mesh_state_color = DefaultDebugColor();
            auto gen_state_color = DefaultDebugColor();
            auto ch_pos = toWorldChunkCoord(ctx->player_cam.pos);
            std::string gen_state_str{"No state entry."};
            std::string mesh_state_str{"No state entry."};
            auto target_mesh_id = 0uz;
            auto inflight_mesh_id = 0uz;
            auto loaded_mesh_id = 0uz;
            ctx->world.chunkMap.entries.if_contains(
                ch_pos,
                [&](ChunkEntry& entry){
                    target_mesh_id = entry.mesh.target;
                    inflight_mesh_id = entry.mesh.inflight;
                    loaded_mesh_id = entry.mesh.loaded;
                    PipelineState gen_stage{};
                    bool gen_clean{};
                    bool noBlocks{};
                    bool opaque_loaded{};
                    bool transp_loaded{};
                    bool opaque_empty{};
                    bool transp_empty{};
                    bool mesh_clean{};

                    gen_stage = entry.gen_pipeline_state();
                    gen_clean = entry.gen.is_clean();

                    
                    ctx->world.chunkMap.entries.if_contains(
                        ch_pos,
                        [&](ChunkEntry& entry){
                            noBlocks = entry.block_data.read().empty();
                        }
                    );
                    
                    gen_state_str = std::format(
                        "{} ({})  {}",
                        gen_stage,
                        (gen_clean ? "clean"     : "dirty"),
                        (noBlocks ? "(empty)" : "")
                    );
                    gen_state_color = PipelineStateOutlineColor(gen_stage);

                    mesh_clean = entry.mesh.is_clean();

                    
                    ctx->rend.opaque_chunk_meshes.if_contains(
                        ch_pos,
                        [&](Mesh& mesh){
                            opaque_empty = mesh.vertex_count==0;
                            opaque_loaded = mesh.isLoaded();
                        }
                    );
                    ctx->rend.blended_chunk_meshes.if_contains(
                        ch_pos,
                        [&](Mesh& mesh){
                            transp_empty = mesh.vertex_count==0;
                            transp_loaded = mesh.isLoaded();
                        }
                    );
                    
                    mesh_state_str = std::format(
                        "{} ({})  OP:{}{} TR:{}{}",
                        entry.mesh_pipeline_state(),
                        (mesh_clean ? "clean"     : "dirty"    ),
                        (opaque_loaded ? "LOADED" : "UNLOADED" ),
                        (opaque_empty ? "(empty)" : ""         ),
                        (transp_loaded ? "LOADED" : "UNLOADED" ),
                        (transp_empty ? "(empty)" : ""         )
                    );
                    mesh_state_color = PipelineStateOutlineColor(entry.mesh_pipeline_state());
                    // state.gen 
                    // opqMesh: [Loaded|Unloaded] [(empty)] 
                    // trnMesh: [Loaded|Unloaded] [(empty)] 
                    // isDirty
                }
            );
            auto n_pending_writes = 0uz;
            ctx->world.chunkMap.pending_writes.if_contains(
                ch_pos,
                [&](auto & pending_writes){
                    n_pending_writes = pending_writes.size();
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
            ctx->rend.opaque_chunk_meshes.for_each_if([](auto& mesh){return mesh.isLoaded();}, [&loaded_opaque](auto& mesh){loaded_opaque++;});
            ctx->rend.blended_chunk_meshes.for_each_if([](auto& mesh){return mesh.isLoaded();}, [&loaded_trans](auto& mesh){loaded_trans++;});
            UI::Text("Loaded opaque:{}/{}",loaded_opaque,total_opaque);
            UI::Text("Loaded trans:{}/{}",loaded_trans,total_trans);
        });
        window.section("Positions",[&]{
            auto ch_pos = toWorldChunkCoord(ctx->player_cam.pos);
            std::string facing_str = get_facing_str(ctx->player_cam.getFront());
            IG::Text("World: %+03.1f,%+03.1f,%+03.1f", ctx->player_cam.pos.x, ctx->player_cam.pos.y, ctx->player_cam.pos.z);
            UI::Text("WorldChunkCoord:{:+3},{:+3}", ch_pos.x,ch_pos.z);
            IG::Text("cam.pitch|yaw: %03.1f, %03.1f", ctx->player_cam.pitch, ctx->player_cam.yaw);
            IG::Text("Facing: %s", facing_str.c_str());
        });
        window.section("World Data:",[&]{
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
                        if (entry.mesh.has_data()){
                            n_pending_clean_meshed += entry.mesh.is_clean();
                            n_pending_dirty_meshed += entry.mesh.is_dirty();
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


        window.section("Per frame draw info:",[&]{
            size_t n_vtx = ctx->rend.debug.vertex_count;
            size_t n_bytes = n_vtx * sizeof(Vertex);
            f32 kb = n_bytes /1000.0;
            f32 mb = kb/1000.0;
            f32 gb = mb/1000.0;
            IG::Text("Vertex Count: %lu (%7.2lfMB | %6.2lfGB) ",n_vtx, mb,gb); 
            IG::Text("Draw Calls: %llu", ctx->rend.debug.draw_calls);
            IG::Text("Mesh Count: %llu", ctx->rend.debug.mesh_count);
        });

        window.section("Process Metrics:",[]{
            IG::Text("Resident Set Size: %5.2lfmb", unix::rss_bytes()/1024.0/1024.0);
        });

        window.section("Concurrency:",[&]{

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
                ctx->gen_counters.n_on_queue,
                ctx->rb_generating,
                ctx->director.generators.job_queue.capacity +ctx->director.generators.res_queue.capacity,
                "Generating"
            );

            idk(
                ctx->mesh_counters.n_on_queue,
                ctx->rb_meshing,
                ctx->director.meshers.job_queue.capacity + ctx->director.meshers.res_queue.capacity,
                "Meshing"
            );

                
            IG::Separator();

            draw_graph<JobType::Gen>(ctx);
            draw_graph<JobType::Light>(ctx);
            draw_graph<JobType::Mesh>(ctx);
//            drawSizeAndUniqueness(" genJobQ",ctx->maxGenJobsPerFrame,gen.job_queue,ctx->, ctx->rb_genJobsAdded);
//            drawSizeAndUniqueness(" genResQ",ctx->maxGenUploadsPerFrame,gen.res_queue,ctx->gen_res_this_frame,ctx->rb_genJobsAdded);
//
//            drawSizeAndUniqueness("meshJobQ",ctx->maxMeshJobsPerFrame,mesher.job_queue,ctx->mesh_jobs_this_frame,ctx->rb_meshJobsAdded);
//            drawSizeAndUniqueness("meshResQ",ctx->maxMeshUploadsPerFrame, mesher.res_queue,ctx->mesh_results_this_frame,ctx->rb_meshResultsAdded);

            });
        window.section("Padding:",[&]{
            UI::Text("");
            UI::Text("");
            UI::Text("");
            UI::Text("");
        });

    });
}
void DebugUI::init(GLFWwindow* _win_ptr) {
    this->win_ptr = _win_ptr;
    g_UI_SCALE = UI_SCALE;
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(win_ptr, true);
    // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();
    win_configs.insert_range(
        win_configs.begin(),std::vector<WindowConfig>{ 
            {"GENERAL DEBUG OVERLAY", UI::WinFlagGroup::MovableOverlay, drawGeneralDebugOverlay,this},
            {"SECOND CAMERA", UI::WinFlagGroup::MovableOverlay,drawSecondCameraWindow,this},
            {"FULLSCREEN OVERLAY", UI::WinFlagGroup::Overlay,drawFullscreenOverlay,this},
            {"PER CHUNK LOG", UI::WinFlagGroup::MovableOverlay,drawPerChunkLogWindow,this},
            {"GLOBAL LOG", UI::WinFlagGroup::MovableOverlay,drawGlobalLogWindow,this},
            {"DBG OPTS", UI::WinFlagGroup::MovableOverlay,drawDebugSettingsWindow,this},
            {"WORLDGEN", UI::WinFlagGroup::MovableOverlay,draw_worldgen_window,this},
            {"GRAPHICS", UI::WinFlagGroup::MovableOverlay,draw_graphics_window,this},
            {"WORLD", UI::WinFlagGroup::MovableOverlay,draw_world_window,this},
        }
    );
}

void DebugUI::StartFrame(){
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    IG::NewFrame();
}
void DebugUI::destroy() {
    std::println();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    IG::DestroyContext();
}
