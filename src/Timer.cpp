#include "Timer.hpp"
#include "GLFW/glfw3.h"
#include "Logger.hpp"

i64  program_epoch_ns = 0;
void Timer::setupTimer() {
    glfwSetTime(0.0);
}

void Timer::update() {
    f64 before = total_elapsed_s;
    total_elapsed_s = glfwGetTime();
    this->dt_s = total_elapsed_s - before;
    framerate_ringbuf.write(static_cast<f32>(1.0 / dt_s));
    t0_mid_frame = total_elapsed_s;
}

void Timer::bench_input() {
    f64 before = t0_mid_frame;
    t0_mid_frame = glfwGetTime();
    f64 dt_s = t0_mid_frame - before;
    inp_framerate_ringbuf.write(static_cast<f32>(1.0 / dt_s));
}
void Timer::bench_update() {
    f64 before = t0_mid_frame;
    t0_mid_frame = glfwGetTime();
    f64 dt_s = t0_mid_frame - before;
    upd_framerate_ringbuf.write(static_cast<f32>(1.0 / dt_s));
}
// does include the time taken to draw the debug ui.
void Timer::bench_draw() {
    f64 before = t0_mid_frame;
    t0_mid_frame = glfwGetTime();
    f64 dt_s = t0_mid_frame - before;
    drw_framerate_ringbuf.write(static_cast<f32>(1.0 / dt_s));
}

void Timer::bench_render() {
    f64 before = t0_mid_frame;
    t0_mid_frame = glfwGetTime();
    f64 dt_s = t0_mid_frame - before;
    ren_framerate_ringbuf.write(static_cast<f32>(1.0 / dt_s));
}
