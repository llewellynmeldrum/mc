#pragma once 
#include "CoordTypes.hpp"
#include "DebugChunkLog.hpp"
#include "cppslop.hpp"
#include <chrono>
#include <format>
FORWARD_DECL_STRUCT(Simulation)
FORWARD_DECL_STRUCT(GLFWwindow)

struct DebugViewSettings{
    bool showGenState = false;
    bool showMeshState = false;
    bool showNeighbours = true;
};
struct DebugUI{
     DebugUI();
    ~DebugUI();
    void init(GLFWwindow* win_ptr);
    void destroy();

    template <typename ...Args>
    void log(std::format_string<Args...> fmt, Args&& ...vargs){
        dbg_log.make_entry(std::format(fmt,std::forward<Args>(vargs)...));
    }
    DebugViewSettings dbg_view;
    DebugLog dbg_log;
    void draw();
    void update();

    f32 UI_SCALE = 1.25;
private:
    GLFWwindow* win_ptr;
    static void StartFrame();
};
