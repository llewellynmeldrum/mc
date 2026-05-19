
#include "DebugFormat.hpp"
#include "DebugFormatSpecializations.hpp"

#include "App.hpp"
#include "Block.hpp"
#include "Chunk.hpp"
#include "Context.hpp"
#include "DEBUG.hpp"
#include "Profiler.hpp"
#include <deque>
#include <mutex>
#include <optional>
#define _DEBUG
#include <ranges>



/*
 ideal shape:
*/

void App::setup() {
    ivec3 spawn_pos = { -61, +130, -83 };
    ctx.cam.pos = spawn_pos;
    ctx.cam.pitch = -23.4;
    ctx.cam.yaw = 56.3;
}

void App::loop() {
    ctx.time.update();

    // INPUT
    {
        ctx.input.poll();
        ctx.handleInputs(); 
        ctx.time.bench_input();
    }
    // UPDATE:
    {
        ctx.update();
        ctx.time.bench_update();
    }

    // DRAWING
    {
        ctx.draw(); 
        ctx.ui.drawDebugUI(); 
        ctx.time.bench_draw();
    }

    {
        ctx.ui.render();
        ctx.win.swapBuffers();
        ctx.time.bench_render();
    }
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

#include "GLFWWrapper.hpp"

void App::exit(i32 exit_code) {
    ctx.ui.destroyDebugUI();
    glfwTerminate();
    std::println("{}", ScopeTimer::summary());
    std::exit(exit_code);
}
