#pragma once 
#include "Types.h"
struct Timer{
    void setup();
    void update();
    u64 framecount = 0;
    f64 dt= 0.0f;       // seconds inbetween frames
    f64 elapsed_s = 0.0;
};
