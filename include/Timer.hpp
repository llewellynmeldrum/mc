#pragma once
#include "Types.h"
#include "MirroredRingBuf.hpp"
#include <map>
#include <string_view>
// src/Timer.cpp
struct Profiler {
public:
    struct Scope{
        Profiler& prof;
        std::string_view key;
        Scope(Profiler& _prof, std::string_view _key) : 
            prof(_prof),
            key(_key) 
        { prof.bench_start(key);}

        ~Scope() { prof.bench_end(key);}
    private: 
        int i;
    };
    Profiler() = default;
    ~Profiler() = default;

    u64 framecount = 0;
    void init(std::vector<std::string_view> e) {
        add_each(e);
        setupTimer_impl();
    }
    void add_each(std::span<const std::string_view> e){
        for (const auto& v: e){
            auto [it, inserted] = ringbufs.try_emplace(v);
            assert(inserted);
        }
    }

    f64 start_of_frame = 0.0;
    f64 dt_s = 0.016;   // for the entire period between start and end frame
    void start_frame();
    void bench_start(std::string_view key);
    void bench_end(std::string_view key);
    Profiler::Scope bench_scope(std::string_view key){return Profiler::Scope{*this, key};}
    void end_frame();

    std::map<std::string_view, MirroredRingBuf<f32, 200>> ringbufs{};
    std::map<std::string_view, f64> start_time{};
private:
    void setupTimer_impl();
};

