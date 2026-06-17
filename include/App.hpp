#pragma once 

#include "Simulation.hpp"
#include "Types.h"
#include "UnixHelpers.hpp"
#include <memory>

FORWARD_DECL_STRUCT(window)

struct App{
    App()=default;
    ~App()=default;
public:
    void setup();
    void loop();

    Simulation sim{};
    i64 loop_count{};
    i32 exit(i32 exit_code);

    bool shouldClose();
private:
};



