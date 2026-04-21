#include "Timer.hpp"
#include "GLFW/glfw3.h"

void Timer::setup() {
    glfwSetTime(0.0);
}

void Timer::update() {
    f64 prev_elapsed_s = elapsed_s;
    elapsed_s = glfwGetTime();
    dt = elapsed_s - prev_elapsed_s;
}

