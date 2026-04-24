#pragma once
#include "Types.h"
#include "MirroredRingBuf.hpp"
// src/Timer.cpp
struct Timer {
    Timer() = default;
    ~Timer() = default;

    void setupTimer();
    void update();

    f64 elapsed_s = 0.0;

    u64 framecount = 0;

    f64 dt_s = 0.0f;

    MirroredRingBuf<f32, 200> framerate_ringbuf;
};
