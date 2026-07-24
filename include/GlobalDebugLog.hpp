#pragma once
#include <utility>
#include <chrono>
#include <unordered_map>
#include "ThreadTracker.hpp"
struct Logger{
public:
    using clock = std::chrono::steady_clock;
    struct Entry{
        Entry(clock::duration t, std::string_view msg) 
            : thread_t(ThreadTracker::my_thread_type())
            , dur(t)
            , msg(msg)
        {}

        ThreadType thread_t;
        clock::duration    dur;
        std::string msg;
    };

public:

    std::vector<Entry> data;
    clock::time_point epoch;
    
    inline auto begin(){return data.begin();}
    inline auto end(){return data.end();}
    inline auto rbegin(){return data.rbegin();}
    inline auto rend(){return data.rend();}

    inline auto since_epoch() const noexcept {
        return clock::now() - epoch;
    }

    template <typename ...Args>
    inline void log(std::format_string<Args...> fmt, Args&& ...vargs){
        data.emplace_back(
            since_epoch(),
            std::format(fmt,std::forward<Args>(vargs)...)
        );
    }

    static std::string elapsed_tostr(clock::duration dt){
        std::chrono::duration<double> elapsed_seconds = dt;

        auto mins = std::chrono::duration_cast<std::chrono::minutes>(dt);
        auto secs = elapsed_seconds - mins;
        return std::format("[{:02} : {:06.3f}]",mins.count(),secs.count());
    }
};
inline Logger global_logger;
template <typename ...Args>
inline void log_to_ui(std::format_string<Args...> fmt, Args&& ...vargs){
    global_logger.log("{}",std::format(fmt,std::forward<Args>(vargs)...));
}

template<>
struct std::formatter<Logger::Entry>{
	constexpr auto parse(std::format_parse_context& ctx){return ctx.begin();}
	auto format(const Logger::Entry& val, std::format_context& ctx)const {
        auto elapsed_tostr = [](Logger::clock::duration dt){
            using namespace std::chrono;
            duration<double> elapsed_seconds = dt;
            auto mins = duration_cast<minutes>(dt);
            auto secs = elapsed_seconds - mins;
            return std::format("{:02} : {:06.3f}",mins.count(),secs.count());
        };
    
		return std::format_to( ctx.out(), "[{:>4} : {}]->{}",std::format("{}",val.thread_t),elapsed_tostr(val.dur),val.msg);
    }
};
