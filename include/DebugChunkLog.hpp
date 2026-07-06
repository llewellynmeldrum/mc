#pragma once 

#include "CommonUtils.hpp"
#include "CoordTypes.hpp"
#include "Assertion.hpp"
#include "NothrowLookup.hpp"
#include <chrono>
#include <format>

inline std::unordered_map<std::string /*i.e DebugLog::LogType*/, bool> is_log_type_enabled = {
    {"all",true},
    {"mesh_state_change",false},
    {"gen_state_change",false},
    {"mesh_endirtying",true},
    {"pending_write",true},

};

struct DebugLog{
public:
    using Clock = std::chrono::steady_clock;
    using TimePoint= Clock::time_point;
    using Duration = Clock::duration;
    using LogType = std::string;
    using DebugEntry = std::tuple<LogType, Duration, std::string>;

public:
    static_assert(same_type<decltype(is_log_type_enabled)::key_type, LogType>);
    DebugLog(Clock::time_point _epoch):
        epoch(_epoch)
    {}

    std::vector<DebugEntry> log;
    
    inline auto begin(){return log.begin();}
    inline auto end(){return log.end();}
    inline auto rbegin(){return log.rbegin();}
    inline auto rend(){return log.rend();}

    TimePoint epoch;
    inline auto since_epoch() const noexcept {
        return Clock::now() - epoch;
    }

    inline void make_entry(std::string_view log_type, std::string_view msg) noexcept{
        log.emplace_back(log_type, since_epoch(), msg);
    }
    inline void make_entry(std::string_view msg) noexcept{
        make_entry("all",msg);
    }

    static std::string entry_tostr(DebugEntry entry) noexcept{
        using namespace std::chrono;
        const auto& [log_type, elapsed, msg]=entry;

        duration<double> elapsed_seconds = elapsed;

        auto mins = duration_cast<minutes>(elapsed);
        auto secs = elapsed_seconds - mins;
        return std::format("[{:02} : {:06.3f}] {}",mins.count(),secs.count(),msg);
    }
};
inline std::unordered_map<WorldChunkCoord, DebugLog> per_chunk_log;
inline bool pause_logging = false;
struct DebugChunkLogger{
    using LogType = DebugLog::LogType;
    DebugLog::Clock::time_point global_epoch;

    DebugChunkLogger(){
        global_epoch = DebugLog::Clock::now();
    }

    inline void add_log_type(LogType log_type) noexcept{
        auto [it, inserted] = is_log_type_enabled.try_emplace(log_type,true);
        assert(inserted);
    }
    inline void enable_log_type(LogType log_type) noexcept{
        bool& b =AT(is_log_type_enabled,log_type);
        b = true;
    }
    inline void disable_log_type(LogType log_type) noexcept{
        bool& b = AT(is_log_type_enabled,log_type);
        b = false;
    }
    inline bool& get_log_type(LogType log_type) noexcept{
        bool& b = AT(is_log_type_enabled,log_type);
        return b;
    }
    inline void log_to_chunk(WorldChunkCoord key, std::string_view msg){
        log_to_chunk("all",key,msg);
    }

    inline void log_to_chunk(std::string_view log_type, WorldChunkCoord key, std::string_view msg){
        if (!pause_logging){
            auto [it, inserted] = per_chunk_log.try_emplace(key, global_epoch);
            it->second.make_entry(log_type,msg);
        }
    }
};

inline DebugChunkLogger _dbg_chunk_logger;

template <typename ...Args>
inline void log_to_chunk(WorldChunkCoord key, std::format_string<Args...> fmt, Args&& ...vargs){
    log_to_chunk("all",key,fmt,std::forward<Args>(vargs)...);
}

template <typename ...Args>
inline void log_to_chunk(std::string_view log_type, WorldChunkCoord key, std::format_string<Args...> fmt, Args&& ...vargs){
    _dbg_chunk_logger.log_to_chunk(log_type,key,std::format(fmt,std::forward<Args>(vargs)...));
}

