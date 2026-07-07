#pragma once 

#include "CommonUtils.hpp"
#include "CoordTypes.hpp"
#include "Assertion.hpp"
#include "Logger.hpp"
#include "NothrowLookup.hpp"
#include <chrono>
#include <format>

inline constexpr std::string_view default_log_type = "all";
inline std::unordered_map<std::string_view, bool> is_log_type_enabled = {
    {"mesh_uploads",false},
    {"mesh_state_change",false},
    {"gen_state_change",false},
    {"mesh_endirtying",true},
    {"pending_write",true},
    {default_log_type,false},

};

struct DebugLog{
public:
    using Clock = std::chrono::steady_clock;
    using TimePoint= Clock::time_point;
    using Duration = Clock::duration;
    using LogType = std::string_view;
    using DebugEntry = std::tuple<LogType, Duration, std::string>;

public:
    static_assert(same_type<decltype(is_log_type_enabled)::key_type, LogType>, 
                  "DebugLog::LogType must be the same as the key type of the is_log_type_enabled map");
    DebugLog(Clock::time_point _epoch, bool _is_fading=false):
        epoch(_epoch),
        is_fading(_is_fading)
    {}

    std::vector<DebugEntry> log;
    TimePoint epoch;
    bool is_fading{false};
    
    inline auto begin(){return log.begin();}
    inline auto end(){return log.end();}
    inline auto rbegin(){return log.rbegin();}
    inline auto rend(){return log.rend();}

    inline auto since_epoch() const noexcept {
        return Clock::now() - epoch;
    }
    inline auto update(){
        if (is_fading){
            std::erase_if(log, [this](DebugEntry entry){
                bool erase = fading_entry_time_elapsed(entry);
                return erase;
            });
        }
    }

    inline void make_entry(std::string_view log_type, std::string_view msg) noexcept{
        log.emplace_back(log_type, since_epoch(), msg);
    }
    inline void make_entry(std::string_view msg) noexcept{
        make_entry(default_log_type,msg);
    }

    static std::string elapsed_tostr(Duration dt){
        using namespace std::chrono;
        duration<double> elapsed_seconds = dt;

        auto mins = duration_cast<minutes>(dt);
        auto secs = elapsed_seconds - mins;
        return std::format("[{:02} : {:06.3f}]",mins.count(),secs.count());
    }
    static std::string entry_tostr(DebugEntry entry) noexcept{
        const auto& [log_type, elapsed, msg]=entry;

        return std::format("{} {}",elapsed_tostr(elapsed),msg);
    }
    static constexpr std::chrono::milliseconds fade_duration_ms = std::chrono::milliseconds(1500);
    bool fading_entry_time_elapsed(DebugEntry entry) noexcept{
        using namespace std::chrono;
        const auto& [log_type, entry_creation_time, msg]=entry;

        auto entry_age = since_epoch() - entry_creation_time;
        milliseconds entry_age_ms = duration_cast<milliseconds>(entry_age);
        return entry_age_ms >= fade_duration_ms;
    }
    f32 fading_entry_opacity01(DebugEntry entry) noexcept{
        using namespace std::chrono;
        const auto& [log_type, entry_creation_time, msg]=entry;
        milliseconds elapsed_ms = duration_cast<milliseconds>(entry_creation_time);
        auto entry_age = since_epoch() - entry_creation_time;
        milliseconds entry_age_ms = duration_cast<milliseconds>(entry_age);

        f32 t_elapsed = static_cast<f32>(entry_age_ms.count() )/ fade_duration_ms.count();
        f32 opacity = 1.0f - t_elapsed; // elapsed of 0 = 1.0f opacity, elapsed of 1 = 0.0f opacity
        return opacity;
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
        log_to_chunk(default_log_type,key,msg);
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
    log_to_chunk(default_log_type,key,fmt,std::forward<Args>(vargs)...);
}

template <typename ...Args>
inline void log_to_chunk(std::string_view log_type, WorldChunkCoord key, std::format_string<Args...> fmt, Args&& ...vargs){
    _dbg_chunk_logger.log_to_chunk(log_type,key,std::format(fmt,std::forward<Args>(vargs)...));
}

struct NotificationLogger{
    NotificationLogger(): 
        log(DebugLog::Clock::now(),true)
    {}
    DebugLog log;
};
inline NotificationLogger _notif_logger;
template <typename ...Args>
inline void push_notification(std::format_string<Args...> fmt, Args&& ...vargs){
    _notif_logger.log.make_entry(std::format(fmt, std::forward<Args>(vargs)...));
}

