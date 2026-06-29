#pragma once 

#include "CoordTypes.hpp"
#include <chrono>
#include <format>
#include <mutex>
#include <thread>

struct DebugLog{
    using Clock = std::chrono::steady_clock;
    using TimePoint= Clock::time_point;
    using Duration = Clock::duration;
    using DebugEntry = std::pair<Duration,std::string>;



    std::vector<DebugEntry> log;
    
    inline auto begin(){return log.begin();}
    inline auto end(){return log.end();}
    inline auto rbegin(){return log.rbegin();}
    inline auto rend(){return log.rend();}
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
        return std::format("[{:02} : {:06.3f}] {}",mins.count(),secs.count(),txt);
    }
};
inline std::unordered_map<WorldChunkCoord, DebugLog> per_chunk_log;
// thread safe logger
extern DebugLog global_log;
inline bool pause_logging = false;

template <typename ...Args>
inline void log_to_chunk(WorldChunkCoord key, std::format_string<Args...> fmt, Args&& ...vargs){
    if (!pause_logging)
        per_chunk_log[key].make_entry(std::format(fmt,std::forward<Args>(vargs)...));
}


void log_to_all_impl(std::string s);
inline std::mutex global_log_write;
template <typename ...Args>
inline void log_to_all(std::format_string<Args...> fmt, Args&& ...vargs){
    if (!pause_logging){
        log_to_all_impl(std::format(fmt,std::forward<Args>(vargs)...));
    }
}

