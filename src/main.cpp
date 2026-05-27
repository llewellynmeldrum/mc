#include "App.hpp"
#include "Profiler.hpp"
#define _DEBUG


void App::setup() {
    auto spawn_pos = WorldFloatPos{ -61, +130, -83 };
    sim.cam.pos = spawn_pos;
    sim.cam.pitch = -23.4;
    sim.cam.yaw = 56.3;
}

void App::loop() {
    sim.loop();
}

bool App::shouldClose() {
    return sim.win.shouldClose();
}

int main(int argc, char** argv) {

    App app;
    app.sim.setupContext();
    app.setup();
    while (!app.shouldClose()) {
        app.loop();
        app.frameCount++;
    }
    app.exit(EXIT_SUCCESS);
}


void App::exit(i32 exit_code) {
    sim.ui.destroyDebugUI();
    sim.win.terminate();
    std::println("{}", ScopeTimer::summary());
    std::exit(exit_code);
}
