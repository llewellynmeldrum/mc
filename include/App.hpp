#pragma once 

#include "Simulation.hpp"
#include "Types.h"
#include <memory>

struct Window;

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

inline void App::setup() {
    sim.setupSimulation();
}

inline void App::loop() {
    sim.loop();
    loop_count++;
}

inline bool App::shouldClose() {
    return sim.win.shouldClose();
}
inline i32 App::exit(i32 exit_code) {
    sim.ui.destroy();
    sim.win.terminate();
    std::println("{}", ScopeTimer::summary());
    std::exit(exit_code);
    return exit_code;
}
