#include "Assertion.hpp"
#include "Timer.hpp"
#include "GLFW/glfw3.h"
#include "Logger.hpp"

i64  program_epoch_ns = 0;

void Timer::setupTimer_impl(){
    glfwSetTime(0.0);
}
void Timer::start_frame() {
    f64 now = glfwGetTime();
    start_of_frame = now;
}

void Timer::bench_start(std::string_view key){
    ASSERT_TRUE(ringbufs.contains(key));
    f64 now = glfwGetTime();
    start_time.insert_or_assign(key,now);
}

void Timer::bench_end(std::string_view key){
    ASSERT_TRUE(ringbufs.contains(key));
    f64 now = glfwGetTime();
    f64 before = start_time.at(key);
    f64 dt_s = now - before;
    ringbufs.at(key).write(static_cast<f32>(dt_s * 1000.0));
}

void Timer::end_frame(){
    f64 now = glfwGetTime();
    f64 before = start_of_frame;
    dt_s = now - before;
}
