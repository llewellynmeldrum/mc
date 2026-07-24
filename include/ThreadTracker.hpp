#pragma once 
#include "Breakpoints.hpp"
#include "Logger.hpp"
#include <mutex>
#include <thread>
extern const std::thread::id MAIN_THREAD_ID;

enum struct ThreadType{
    unknown,
    main,
    gen,
    mesh,
};
template<>
struct std::formatter<ThreadType>{
	constexpr auto parse(std::format_parse_context& ctx) const noexcept{return ctx.begin();}
	auto format(const ThreadType& val, std::format_context& ctx)const noexcept{
        std::string s{};
        switch(val){
        case ThreadType::main: s="main"; break;
        case ThreadType::gen: s="gen"; break;
        case ThreadType::mesh: s="mesh"; break;
        case ThreadType::unknown: s="unknown"; break;
            break;
        }
        return format_to(ctx.out(), "{}",s);
    }
};
struct ThreadTracker{
    inline static std::mutex mut;
    inline static std::unordered_map<std::thread::id, ThreadType> thread_types;
    static inline auto my_id(){
        return std::this_thread::get_id();
    }

    static inline void init(){
        assert(is_main_thread() && "Must be called from main thread!!!");
        thread_types = std::unordered_map<std::thread::id, ThreadType>{};
        assign_my_thread_type(ThreadType::main);
    }
    static inline void assign_my_thread_type(ThreadType type){
        std::lock_guard lock(mut);
        auto [_, success] = thread_types.try_emplace(my_id(),type);
        assert(success);
    }
    static inline auto my_thread_type(){
        std::lock_guard lock(mut);
        auto id = my_id();
        auto thread_type = ThreadType::unknown;
        auto it = thread_types.find(id);
        if (it == thread_types.end()){
            std::println(stderr, "THREAD {} QUERIED ITS TYPE, BUT NEVER REGISTERED!",id);
        }else{
            thread_type = it->second;
        }
        return thread_type;
    }
    static void assert_main_thread();
    static inline bool is_main_thread(){
        return(my_id() == MAIN_THREAD_ID);
    }
};
