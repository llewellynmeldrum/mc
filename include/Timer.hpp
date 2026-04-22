#pragma once 
#include "Types.h"
// src/Timer.cpp
struct Timer{
    Timer()=default;
    ~Timer()=default;

    void   setupTimer();
    void update();
    u64 framecount = 0;
    f64 dt= 0.0f;       // seconds inbetween frames
    f64 elapsed_s = 0.0;
};
