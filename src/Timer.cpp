#include "Assertion.hpp"
#include "Timer.hpp"
#include "Logger.hpp"
#include <chrono>
#include <tracy/Tracy.hpp>

i64  program_epoch_ns = 0;
using steady_clock = std::chrono::steady_clock;
steady_clock::time_point epoch = steady_clock::now();

f64 s_now(){
    auto dt = steady_clock::now() - epoch;
    auto seconds = std::chrono::duration<float>(dt);
    return seconds.count();
}

void Profiler::setupTimer_impl(){
    epoch = steady_clock::now();
}
void Profiler::start_frame() {
    f64 now = s_now();
    start_of_frame = now;
}

void Profiler::bench_start(std::string_view key){
    ASSERT_TRUE(ringbufs.contains(key));
    f64 now = s_now();
    start_time.insert_or_assign(key,now);
}

void Profiler::bench_end(std::string_view key){
    ASSERT_TRUE(ringbufs.contains(key));
    f64 now = s_now();
    f64 before = start_time.at(key);
    f64 dt_s = now - before;
    ringbufs.at(key).write(static_cast<f32>(dt_s * 1000.0));
}

void Profiler::end_frame(){
    f64 now = s_now();
    f64 before = start_of_frame;
    dt_s = now - before;
    FrameMark;
}
