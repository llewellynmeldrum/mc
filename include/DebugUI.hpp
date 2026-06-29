#pragma once 
#include "ChunkEntry.hpp"
#include "CoordTypes.hpp"
#include "DebugChunkLog.hpp"
#include "DebugChunkRenderer.hpp"
#include "cppslop.hpp"
#include <chrono>
#include <format>
FORWARD_DECL_STRUCT(Engine)
FORWARD_DECL_STRUCT(GLFWwindow)

struct DebugParams{
    int opt_lvl{-1};
};
struct DebugViewSettings{
    bool showGenState = false;
    bool showNeighbours = true;
    bool        showChunkBoundaries{ false};  
    bool        showDebugUI{ true};  
    glm::vec4 resolve_debug_outline_color(const ChunkState& state){
        if (showGenState){
            return GenDebugOutlineColor(state.gen);
        } else{
            return MeshDebugOutlineColor(state.mesh);
        }
    }
};
struct DebugUI{
     DebugUI();
    ~DebugUI();
    void init(GLFWwindow* win_ptr);
    void destroy();
    bool is_ui_expanded{false};

    template <typename ...Args>
    void log(std::format_string<Args...> fmt, Args&& ...vargs){
        dbg_log.make_entry(std::format(fmt,std::forward<Args>(vargs)...));
    }
    DebugViewSettings dbg_view;
    DebugLog dbg_log;
    DebugParams dbg_params;
    void draw();
    void update();

    f32 UI_SCALE = 1.25;
private:
    GLFWwindow* win_ptr;
    static void StartFrame();
};
