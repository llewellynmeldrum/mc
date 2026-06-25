#include "App.hpp"
#include "FmtStyle.hpp"
#include "UnixHelpers.hpp"

void App::setup() {
    sim.setupSimulation();
    cpptrace::register_terminate_handler(); // gives us stack traces in std::terminate
    g_StyleConfig::disabled = unix::is_debugger_present();
    auto optimization_level = unix::get_env<int>("OPT_LEVEL");
    if (optimization_level){
        std::println("{}",*optimization_level);
        sim.ui.dbg_params.opt_lvl = *optimization_level;
    }else{
        std::println("No OPT_LEVEL env found.");
    }
    if (g_StyleConfig::isEnabled()){
        std::println("Debugger detected, disabling ansi styling.");
    }
}

void App::loop() {
    while (!shouldClose()) {
        sim.loop();
        loop_count++;
    }
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
