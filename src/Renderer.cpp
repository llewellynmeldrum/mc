
#include "ChunkConcurrency.hpp"
#include "DebugFormatSpecializations.hpp"
#include "Renderer.hpp"

#include "Profiler.hpp"
#include "glbindingWrapper.hpp"
#include "glmWrapper.hpp"
#include "World.hpp"
#include <algorithm>

using namespace gl;
using namespace glm;
void Renderer::updateViewport(int x, int y, int w, int h) {
    glViewport(x, y, w, h);
}
void Renderer::setup() {
    atlas.setupTextureAtlas();
    prog.setupShaderProgram("shaders/vs.glsl", "shaders/fs.glsl");
    prog.use();
    prog.setUniform("texture1", (int)0);
    prog.stop();
    mesher.launchChunkMeshers();

    dbg_rend.setup();
}

void Renderer::clear(const vec4 clear_color) {
    debug.reset_per_frame();
    // hollow purple reference
    glPolygonMode(GL_FRONT_AND_BACK, debug.wireframe ? GL_LINE : GL_FILL);
    glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::draw_debugChunks(Camera& cam, World& world){
    dbg_rend.update(cam,world);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    dbg_rend.draw(cam);
    glPolygonMode(GL_FRONT_AND_BACK, debug.wireframe ? GL_LINE : GL_FILL);
}

void Renderer::draw(Camera& cam){
    auto& view = cam.getViewMatrix();
    const auto& proj = cam.getProjectionMatrix();
    prog.use();
    atlas.texture.bind();
    prog.setUniform("view", view);
    prog.setUniform("proj", proj);
    prog.setUniform("overlayOpacity", debug.blockOverlayOpacity);
    {
        ScopeTimer draw_timer{ "Renderer::draw", "draw call" };
        // sort the vector by nearest chunks (std::less)
        auto sqdist = [](const auto& a, const auto& b) -> i32{
            auto sq = [](i32 v){ return v*v;};
            return sq(b.x-a.x) + sq(b.y-a.y) + sq(b.z-a.z);
        };
        auto source = static_cast<WorldChunkCoord>(cam.pos);
        std::ranges::sort(visibleChunkMeshes, [sqdist, source](const auto& lhs, const auto& rhs){
            // sort ascending, i.e mesh closest first.
            // Will have to flip when i do transperancy
            return sqdist(source,lhs.chunkCoord) < sqdist(source,rhs.chunkCoord);
        });
        for (const auto& mesh : visibleChunkMeshes) {
            const vec3 chunkWorldPos = toWorldBlockPos(mesh.chunkCoord);
 //           const auto chunkDist = glm::distance(chunkWorldPos,cam.pos);
//            std::println("{}",chunkDist);
            mat4       model = mat4(1.0f);
            model = translate(model, chunkWorldPos);
            prog.setUniform("model", model);
            mesh.draw();
            debug.vertex_count += mesh.offset_count;
            debug.mesh_count++;
            debug.draw_calls++;
        }
//        timer_log_avg_us("Renderer::draw", debug.draw_calls);
    }
    prog.stop();
}
