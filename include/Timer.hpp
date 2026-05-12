#pragma once
#include "Logger.hpp"
#include "Types.h"
#include "MirroredRingBuf.hpp"
// src/Timer.cpp
struct Timer {
    Timer() = default;
    ~Timer() = default;

    u64 framecount = 0;
    void setupTimer();
    void update();

    void bench_input();
    void bench_update();
    void bench_draw();
    void bench_render();
    f64 total_elapsed_s = 0.0; // set to the current elapsed second count AT THE START OF THE FRAME.
    f64 dt_s = 0.016; // for the entire frame.
    f64 t0_mid_frame = 0;

    MirroredRingBuf<f32, 200> framerate_ringbuf;
    MirroredRingBuf<f32, 200> inp_framerate_ringbuf;
    MirroredRingBuf<f32, 200> upd_framerate_ringbuf;
    MirroredRingBuf<f32, 200> drw_framerate_ringbuf;
    MirroredRingBuf<f32, 200> ren_framerate_ringbuf;
};
