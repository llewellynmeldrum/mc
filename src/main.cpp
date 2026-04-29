#include "App.hpp"
#include "Block.hpp"
#include "Context.hpp"
#include "DEBUG.hpp"
#define _DEBUG
#include "Profiler.hpp"
#include <ranges>

constexpr const i32 RENDER_DIST = 4;

void
Context::drawScene() {
    bool remesh_this_frame = false;
    if (cam.requestsMeshRegen) {
        ScopeTimer mesh_chunks{ "Chunk meshing", "chunk" };
        ivec3      camera_chunk_pos = World::worldToChunkCoord(cam.pos);
        auto       dirtyChunks = world.getDirtyChunksInRadius(camera_chunk_pos, RENDER_DIST);
        for (const auto& [chunk_pos, chunk] : dirtyChunks) {
            rend.visibleChunkMeshes.emplace(
                chunk_pos,                                               //
                rend.mesher.mesh(&world, *chunk, chunk_pos, rend.atlas)  //
            );
            remesh_this_frame = true;
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
    constexpr i64 chunk_hoz_radius = 6;
    {
        ScopeTimer world_gen("World Gen", "chunk");
        for (i64 x = -chunk_hoz_radius; x <= chunk_hoz_radius; x++) {
            for (i64 z = -chunk_hoz_radius; z <= chunk_hoz_radius; z++) {
                for (i64 y = 0; y <= World::NUM_VERTICAL_CHUNKS; y++) {
                    ctx.world.generateChunk({ x, y, z });
                }
            }
        }
    }
    timer_log_ms_avg_us("World Gen", pow(6 * 2, 3));
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
