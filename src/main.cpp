#include "App.hpp"
#include "Block.hpp"
#include "Context.hpp"
#include "DEBUG.hpp"
#define _DEBUG
#include "Profiler.hpp"

constexpr const i32 RENDER_DIST = 4;

// TODO: move to src/Context.cpp

void
Context::drawScene() {
    bool remesh_this_frame = false;
    if (cam.requestsMeshRegen) {
        auto mesh_chunks = ScopeTimer("Chunk meshing", "chunk");
        auto camera_chunk_pos = World::worldToChunkCoord(cam.pos);
        auto dirtyChunks = world.getDirtyChunksInRadius(camera_chunk_pos, RENDER_DIST);
        for (const auto& [chunk_pos, chunk] : dirtyChunks) {
            remesh_this_frame = true;
            rend.visibleChunkMeshes.insert({
                chunk_pos,                                               //
                rend.mesher.mesh(&world, *chunk, chunk_pos, rend.atlas)  //
            });
            world.chunks.makeClean(chunk_pos);
        }
        cam.requestsMeshRegen = false;
    }
    rend.draw(cam.getViewMatrix(), cam.getProjectionMatrix());
    if (remesh_this_frame) {
        LOG_EXPR(rend.debug.mesh_count);
        timer_log_avg_us("Chunk meshing", rend.debug.mesh_count);
        timer_log_ms("Chunk meshing");
    }
    static bool first_draw = true;
    if (first_draw) {
        LOG_DEBUG("Finished first draw");
        first_draw = false;
    }
}

void
App::setup() {
    constexpr i64 chunk_radius = 6;
    for (i64 x = -chunk_radius; x <= chunk_radius; x++) {
        for (i64 z = -chunk_radius; z <= chunk_radius; z++) {
            for (i64 y = -chunk_radius; y <= chunk_radius; y++) {
                ctx.world.generateChunk({ x, y, z });
            }
        }
    }
    LOG_DEBUG("Finished chunk generation");
}

void
App::loop() {
    ctx.time.update();
    ctx.input.poll();
    ctx.handleInputs();

    ctx.rend.clear({ 0.25, 0.5, 0.85, 1.0 });
    ctx.drawScene();

    ctx.ui.drawDebugUI();
    ctx.ui.render();
    ctx.win.swapBuffers();
}

bool
App::shouldClose() {
    return ctx.win.shouldClose();
}

int
main(int argc, char** argv) {

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

void
App::exit(i32 exit_code) {
    ctx.ui.destroyDebugUI();
    glfwTerminate();
    std::exit(exit_code);
}
