#pragma once 
#include "cppslop.hpp"
#include <chrono>
#include <format>
FORWARD_DECL_STRUCT(Simulation)
FORWARD_DECL_STRUCT(GLFWwindow)
struct DebugLog{
    using Clock = std::chrono::steady_clock;
    using TimePoint= Clock::time_point;
    using Duration = Clock::duration;
    using DebugEntry = std::pair<Duration,std::string>;



    std::vector<DebugEntry> log;
    
    inline auto begin(){return log.begin();}
    inline auto end(){return log.end();}
    TimePoint epoch;
    DebugLog():epoch(Clock::now()){}
    inline auto sinceEpoch() const noexcept {
        return Clock::now() - epoch;
    }
    inline void make_entry(std::string_view sv) noexcept{
        log.emplace_back(sinceEpoch(), sv);
    }

    static std::string entry_tostr(DebugEntry entry) noexcept{
        using namespace std::chrono;
        const auto& [elapsed, txt]=entry;

        duration<double> elapsed_seconds = elapsed;

        auto ms_precision = std::chrono::floor<std::chrono::milliseconds>(elapsed);
        auto mins = duration_cast<minutes>(elapsed);
        auto secs = elapsed_seconds-mins;
        return std::format("[{:02} : {:05.2f}] {}",mins.count(),secs.count(),txt);
    }
};

struct DebugViewSettings{
    bool showGenState = false;
    bool showMeshState = true;
};
struct DebugUI{
     DebugUI();
    ~DebugUI();
    void init(GLFWwindow* win_ptr);
    void destroy();

    void log_write(std::string_view sv){
        dbg_log.make_entry(sv);
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
