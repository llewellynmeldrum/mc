#include "App.hpp"
#include "Block.hpp"
#include "Context.hpp"
#include "DEBUG.hpp"
#define _DEBUG

constexpr const i32 RENDER_DIST = 4;

// TODO: move to src/Context.cpp
void Context::drawScene() {
    if (cam.requestsMeshRegen) {
        auto camera_chunk_pos = World::worldToChunkPos(cam.pos);
        auto dirtyChunks = world.getDirtyChunksInRadius(camera_chunk_pos, RENDER_DIST);
        for (const auto& [chunk_pos, chunk] : dirtyChunks) {
            rend.visibleChunkMeshes.insert({
                chunk_pos,                            //
                rend.mesher.mesh(*chunk, rend.atlas)  //
            });
            world.chunks.makeClean(chunk_pos);
        }
    }
    rend.draw(cam.getViewMatrix(), cam.getProjectionMatrix());
}

void App::setup() {
    for (int x = 0; x < 4; x++) {
        for (int z = 0; z < 4; z++) {
            ctx.world.generateChunk({ x, 0, z });
        }
    }
}

void App::loop() {
    ctx.time.update();
    ctx.input.poll();
    ctx.handleInputs();

    ctx.rend.clear({ 0.25, 0.5, 0.85, 1.0 });
    ctx.drawScene();

    ctx.ui.drawDebugUI();
    ctx.ui.render();
    ctx.win.swapBuffers();
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
    std::exit(exit_code);
}
