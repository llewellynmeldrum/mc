#include "App.hpp"
#include "Profiler.hpp"
#define _DEBUG


void App::setup() {
    glm::ivec3 spawn_pos = { -61, +130, -83 };
    ctx.cam.pos = spawn_pos;
    ctx.cam.pitch = -23.4;
    ctx.cam.yaw = 56.3;
}

void App::loop() {
    ctx.time.bench_start("frame");

    // INPUT
    {
        ctx.time.bench_start("input");
        ctx.input.poll();
        ctx.handleInputs(); 
        ctx.time.bench_end("input");
    }

    // UPDATE:
    {
        ctx.time.bench_start("update");
        ctx.update();
        ctx.time.bench_end("update");
    }

    // DRAWING
    {
        ctx.time.bench_start("draw");
        ctx.draw(); 
        ctx.ui.drawDebugUI(); 
        ctx.time.bench_end("draw");
    }

    {
        ctx.time.bench_start("render");
        ctx.ui.render();
        ctx.win.swapBuffers();
        ctx.time.bench_end("render");
    }
    ctx.time.bench_end("frame");
}

bool App::shouldClose() {
    return ctx.win.shouldClose();
}

int main(int argc, char** argv) {

    App app;
    app.ctx.setupContext();
    app.setup();
    while (!app.shouldClose()) {
        app.loop();
        app.frameCount++;
    }
    app.exit(EXIT_SUCCESS);
}


void App::exit(i32 exit_code) {
    ctx.ui.destroyDebugUI();
    ctx.win.terminate();
    std::println("{}", ScopeTimer::summary());
    std::exit(exit_code);
}
