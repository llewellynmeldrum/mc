#include "Timer.hpp"
#include "GLFW/glfw3.h"
#include "Logger.hpp"

u64 program_epoch_ns = 0;
void Timer::setupTimer() {
    glfwSetTime(0.0);
}

void Timer::update() {
    f64 prev_elapsed_s = elapsed_s;
    elapsed_s = glfwGetTime();
    dt = elapsed_s - prev_elapsed_s;
}

