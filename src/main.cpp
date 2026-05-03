#include "App.hpp"
#include "Block.hpp"
#include "Context.hpp"
#include "DEBUG.hpp"
#define _DEBUG
#include "Profiler.hpp"
#include <ranges>

constexpr const i32 RENDER_DIST = 4;

// TODO: make blocks take in extra color info, which you can mix in the shader.
// Make it easy to set to different colors, so you can use for debugging (say temperature/humidity
// maps), and for the shadow hack from yt:jdh
void Context::drawScene() {
    bool remesh_this_frame = false;
    if (cam.requestsMeshRegen) {
        ScopeTimer mesh_chunks{ "Chunk meshing", "chunk" };
        ivec3      camera_chunk_pos = World::worldToChunkCoord(cam.pos);
        auto       dirtyChunks = world.getDirtyChunksInRadius(camera_chunk_pos, RENDER_DIST);
        for (const auto& [chunk_pos, chunk] : dirtyChunks) {
            auto chunk_meta = world.chunks.metadata.at(chunk_pos).get();
            rend.visibleChunkMeshes.try_emplace(
                chunk_pos,                                                          //
                rend.mesher.mesh(&world, chunk, chunk_meta, chunk_pos, rend.atlas)  //
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

void App::setup() {
    constexpr i64 chunk_hoz_radius = 10;
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
    timer_log_ms_avg_us("World Gen", pow(chunk_hoz_radius * 2, 3));

    ivec3 spawn_pos = { -61, +130, -83 };
    ctx.cam.pos = spawn_pos;
    ctx.cam.pitch = -23.4;
    ctx.cam.yaw = 56.3;
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
    std::println("{}", ScopeTimer::summary());
    std::exit(exit_code);
}
