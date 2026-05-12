
#include "DebugFormat.hpp"
#include "DebugFormatSpecializations.hpp"

#include "App.hpp"
#include "Block.hpp"
#include "Chunk.hpp"
#include "Context.hpp"
#include "DEBUG.hpp"
#include <deque>
#include <mutex>
#include <optional>
#define _DEBUG
#include "Profiler.hpp"
#include <ranges>

constexpr const i32 RENDER_DIST = 16;

inline auto needs_meshing = [](const World& world){
    return [&world](const ChunkView & pair) {
        const auto& [chunk_coord, ptr] = pair;
        return world.chunkMap.getChunkState(chunk_coord) == ChunkStatus::DIRTY;
    };
};

inline auto in_frustum = [](const Frustum& frustum, const World& world) {
    return [frustum, &world](const ChunkView & pair) {
        const auto& [chunk_coord, ptr] = pair;
        return frustum.isAABBInside(*world.chunkMap.getBoundingBox(chunk_coord));
    };
};

void Context::drawScene() {
    ScopeTimer t_mesh_chunks{ "Chunk meshing", "chunk" };

    auto candidate_chunkviews = 
        world.chunksInRadius(World::worldToChunkCoord(cam.pos), RENDER_DIST) 
        | std::views::filter(needs_meshing(world)) 
//        | std::views::filter(in_frustum(cam.getFrustum(), world))
        | std::ranges::to<std::vector<ChunkView>>();

    // BUG: i have not verified that the frustum is working properly
    std::size_t enqueued_for_meshing = 0;
    for (const auto& [chunk_pos, chunk]: candidate_chunkviews){
        if (world.chunkMap.isMeshing(chunk_pos)){
            continue;
        }
        auto surrounding = world.chunkMap.getSurroundingChunks(chunk_pos);
        auto* meta = world.chunkMap.getMetadata(chunk_pos);
        bool success = rend.mesher.toBeMeshed.try_emplace(chunk_pos, chunk, surrounding, meta);
        if (success){
            enqueued_for_meshing++;
            world.chunkMap.markMeshing(chunk_pos);
        }else{
            // stop submitting this frame
            break;
        }
    }
    LOG_DEBUG("enqueued_for_meshing this frame:{}",enqueued_for_meshing);
    constexpr std::size_t MaxMeshUploadsPerFrame = 12;
    for (std::size_t mesh_count = 0; mesh_count < MaxMeshUploadsPerFrame; mesh_count++){
        auto meshData = rend.mesher.chunkMeshData.try_dequeue();
        if (!meshData){
            break;
        }
        const auto [chunk_pos, vertices, indices] = *meshData;
        // im pretty sure im replacing a bunch of the same meshes
        // I need to guarantee meshes are not yet in the queue, before enqueing them.
        rend.uploadMesh(chunk_pos, vertices, indices);
        world.chunkMap.markClean(chunk_pos);
    }
    rend.draw(cam.getViewMatrix(), cam.getProjectionMatrix());
    static bool first_draw = true;
    if (first_draw) {
        LOG_DEBUG("Finished first draw");
        first_draw = false;
    }
}

void App::setup() {

    constexpr i64 chunk_hoz_radius = 16;
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
    timer_log_ms_avg_us("World Gen", pow(chunk_hoz_radius * 2, 2) * World::NUM_VERTICAL_CHUNKS);

    ivec3 spawn_pos = { -61, +130, -83 };
    ctx.cam.pos = spawn_pos;
    ctx.cam.pitch = -23.4;
    ctx.cam.yaw = 56.3;
}

void App::loop() {
    ctx.time.update();

    {
        ctx.input.poll();
        ctx.handleInputs(); 
        ctx.time.bench_input();
    }

    {
        ctx.rend.clear({ 0.25, 0.5, 0.85, 1.0 });
        ctx.drawScene(); 
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
