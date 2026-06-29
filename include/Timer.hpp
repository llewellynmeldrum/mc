#pragma once
#include "Types.h"
#include "MirroredRingBuf.hpp"
#include "cppslop.hpp"
#include <map>
// src/Timer.cpp
//
FORWARD_DECL_STRUCT(Profiler)
struct ScopeProfiler{
    Profiler* profiler;
    std::string_view key;
    ScopeProfiler(Profiler* p, std::string_view k);
    ~ScopeProfiler();
};
struct Profiler {
public:
    Profiler() = default;
    ~Profiler() = default;

    u64 framecount = 0;
    template<typename ...Args>
    void init(Args... vargs) {
        (ringbufs.try_emplace(vargs), ...); 
        setupTimer_impl();
    }

    f64 start_of_frame = 0.0;
    f64 dt_s = 0.016;   // for the entire period between start and end frame
    void start_frame();
    void bench_start(std::string_view key);
    void bench_end(std::string_view key);
    void end_frame();
    ScopeProfiler bench_scope(std::string_view key);

    std::map<std::string_view, MirroredRingBuf<f32, 200>> ringbufs{};
    std::map<std::string_view, f64> start_time{};
private:
    void setupTimer_impl();
};

