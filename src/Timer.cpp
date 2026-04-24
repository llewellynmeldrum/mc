#include "Timer.hpp"
#include "GLFW/glfw3.h"
#include "Logger.hpp"

i64  program_epoch_ns = 0;
void Timer::setupTimer() {
    glfwSetTime(0.0);
}

void Timer::update() {
    f64 prev_elapsed_s = elapsed_s;
    elapsed_s = glfwGetTime();
    dt_s = elapsed_s - prev_elapsed_s;
    framerate_ringbuf.write(static_cast<f32>(1.0 / dt_s));
}
