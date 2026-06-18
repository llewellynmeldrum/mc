#include "App.hpp"
#include "FmtStyle.hpp"
#include "UnixHelpers.hpp"

void App::setup() {
    sim.setupSimulation();
    g_StyleConfig::disabled = unix::is_debugger_present();
}

void App::loop() {
    sim.loop();
    loop_count++;
}
bool App::shouldClose() {
    return sim.win.shouldClose();
}

i32 App::exit(i32 exit_code) {
    sim.ui.destroy();
    sim.win.terminate();
    std::println("{}", ScopeTimer::summary());
    std::exit(exit_code);
    return exit_code;
}
