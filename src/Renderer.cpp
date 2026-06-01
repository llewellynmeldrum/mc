
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
    prog.setup("shaders/vs.glsl", "shaders/fs.glsl");
    prog.use();
    prog.setUniform("texture1", (int)0);
    prog.stop();
    meshers.launch();

    dbg_rend.setup();
    glEnable(GL_DEPTH_TEST);  // perform depth testing, i.e refuse draw calls which would cause a
                              // vertex further away to overwrite a closer one
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
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
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    dbg_rend.draw(cam);
    glPolygonMode(GL_FRONT_AND_BACK, debug.wireframe ? GL_LINE : GL_FILL);
}
// TODO: we need to resort chunk meshes/ update cam pos to get transparency 100% correct

void Renderer::drawChunks(Camera& cam){
    auto& view = cam.getViewMatrix();
    const auto& proj = cam.getProjectionMatrix();
    prog.use();
    atlas.texture.bind();
    prog.setUniform("view", view);
    prog.setUniform("proj", proj);
//    prog.setUniform("overlayOpacity", debug.blockOverlayOpacity);
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE); // Enable depth mask for opaque
        glDisable(GL_BLEND);
        drawOpaque(cam);

        //glDepthMask(GL_FALSE); // Disable depth mask for transparent
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
        drawTransparent(cam);

        glDepthMask(GL_TRUE); // Enable depth mask for opaque
    prog.stop();

}
void Renderer::drawTransparent(Camera& cam){
    ScopeTimer draw_timer{ "Renderer::draw", "draw call" };
    // sort the vector by nearest chunks (std::less)
    auto sqdist = [](const auto& a, const auto& b) -> i32{
        auto sq = [](i32 v){ return v*v;};
        return sq(b.x-a.x) + sq(b.y-a.y) + sq(b.z-a.z);
    };
    auto source = toWorldChunkCoord(cam.pos);
    //NOTE: SORTS FURTHEST TO NEAREST (transparent)
    std::ranges::sort(transparentChunkMeshes, [sqdist, source](const auto& lhs, const auto& rhs){
        // sort ascending, i.e mesh closest first.
        // Will have to flip when i do transperancy
        return sqdist(source,lhs.chunkCoord) > sqdist(source,rhs.chunkCoord);
    });

    for (const auto& mesh : transparentChunkMeshes) {
        const auto chunkFloatWorldPos = WorldFloatPos{toWorldOrigin(mesh.chunkCoord).raw()};
//           const auto chunkDist = glm::distance(chunkWorldPos,cam.pos);
//            std::println("{}",chunkDist);
        auto model = mat4(1.0f);
        model = glm::translate(model, chunkFloatWorldPos.raw());
        prog.setUniform("model", model);
        mesh.draw();
        debug.vertex_count += mesh.offset_count;
        debug.mesh_count++;
        debug.draw_calls++;
    }
//        timer_log_avg_us("Renderer::draw", debug.draw_calls);
}

void Renderer::drawOpaque(Camera& cam){
    ScopeTimer draw_timer{ "Renderer::draw", "draw call" };
    // sort the vector by nearest chunks (std::less)
    auto sqdist = [](const auto& a, const auto& b) -> i32{
        auto sq = [](i32 v){ return v*v;};
        return sq(b.x-a.x) + sq(b.y-a.y) + sq(b.z-a.z);
    };
    auto source = toWorldChunkCoord(cam.pos);
    //NOTE: SORTS CLOSEST TO FURTHEST (opaque)
    std::ranges::sort(opaqueChunkMeshes, [sqdist, source](const auto& lhs, const auto& rhs){
        return sqdist(source,lhs.chunkCoord) < sqdist(source,rhs.chunkCoord);
    });
    for (const auto& mesh : opaqueChunkMeshes) {
        const auto chunkFloatWorldPos = WorldFloatPos{toWorldOrigin(mesh.chunkCoord).raw()};
//           const auto chunkDist = glm::distance(chunkWorldPos,cam.pos);
//            std::println("{}",chunkDist);
        auto model = mat4(1.0f);
        model = glm::translate(model, chunkFloatWorldPos.raw());
        prog.setUniform("model", model);
        mesh.draw();
        debug.vertex_count += mesh.offset_count;
        debug.mesh_count++;
        debug.draw_calls++;
    }
//        timer_log_avg_us("Renderer::draw", debug.draw_calls);
}
