
#include "ChunkConcurrency.hpp"
#include "DebugFormatSpecializations.hpp"
#include "Renderer.hpp"

#include "Profiler.hpp"
#include "Simulation.hpp"
#include "glbinding/gl/functions.h"
#include "glbindingWrapper.hpp"
#include "glmWrapper.hpp"
#include "World.hpp"
#include <algorithm>

using namespace gl;
using namespace glm;




void Renderer::updateViewport(int x, int y, int w, int h) {
    glViewport(x, y, w, h);
}
Renderer::Renderer() {
    prog.setup("shaders/vs.glsl", "shaders/fs.glsl");
    prog.use();
    prog.setUniform("texture1", (int)0);
    prog.stop();
    meshers.launch();

    dbg_rend.setup();
    line3d_rend.setup();

    enableDepthTesting();
    enableBackfaceCulling();
}

void Renderer::enableDepthTesting(){
    glEnable(GL_DEPTH_TEST);
}

// NOTE: ONLY REQUIRED TO DISABLE FOR XRAY LIKE DRAWS
void Renderer::disableDepthTesting(){
    glDisable(GL_DEPTH_TEST);
}

void Renderer::enableBackfaceCulling(){
    glEnable(GL_CULL_FACE);
}
void Renderer::disableBackfaceCulling(){
    glDisable(GL_CULL_FACE);
}
void enableColorBlending(){
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
}
void disableColorBlending(){
    glDisable(GL_BLEND);
}
void enableDepthMask(){
    glDepthMask(GL_TRUE);
}
void disableDepthMask(){
    glDepthMask(GL_FALSE);
}
void Renderer::beginTransparentPass(){
    enableDepthTesting();

    enableColorBlending();
    disableDepthMask();
}
void Renderer::beginOpaquePass(){
    enableDepthTesting();

    enableDepthMask();
    disableColorBlending();
}

void Renderer::draw_debugChunks(Camera& cam, Simulation* sim, RenderTargetView target){
    target.use();
    dbg_rend.update(sim->playerCam,sim);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    beginTransparentPass();
    dbg_rend.draw(cam);

    glPolygonMode(GL_FRONT_AND_BACK, debug.wireframe ? GL_LINE : GL_FILL);
    target.stop();
}
void Renderer::draw_3DLines_to(Camera& cam, std::span<Line3D> lines, RenderTargetView target){
    target.use();
    line3d_rend.update(cam,lines);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    beginTransparentPass();
    line3d_rend.draw(cam);

    glPolygonMode(GL_FRONT_AND_BACK, debug.wireframe ? GL_LINE : GL_FILL);
    target.stop();
}
// TODO: we need to resort chunk meshes/ update cam pos to get transparency 100% correct

void Renderer::clear_to(RenderTargetView target){
    target.use();
    clear(clear_color);
    target.stop();
}
void Renderer::draw_to(Camera& cam, RenderTargetView target){
    target.use();

    prog.use();
    atlas.texture.bind();
    prog.setUniform("view", cam.getViewMatrix());
    prog.setUniform("proj", cam.getProjectionMatrix());
        beginOpaquePass();
        drawOpaque(cam);

        beginTransparentPass();
        drawTransparent(cam);

    prog.stop();

    target.stop();

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
    auto loadedTransparent= std::views::filter(transparentChunkMeshes,[](const auto& mesh){
        return mesh.isLoaded();
    });

    for (const auto& mesh : loadedTransparent) {
        const auto chunkFloatWorldPos = WorldFloatPos{toWorldOrigin(mesh.chunkCoord).raw()};
//           const auto chunkDist = glm::distance(chunkWorldPos,cam.pos);
//            std::println("{}",chunkDist);
        auto model = mat4(1.0f);
        model = glm::translate(model, chunkFloatWorldPos.raw());
        prog.setUniform("model", model);
        mesh.draw();
        debug.vertex_count += mesh.vertex_count;
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
    auto loadedOpaque = std::views::filter(opaqueChunkMeshes,[](const auto& mesh){
        return mesh.isLoaded();
    });
    for (const auto& mesh : loadedOpaque) {
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

void Renderer::clear(const vec4 clear_color) {
    // hollow purple reference
    enableDepthMask();
    glPolygonMode(GL_FRONT_AND_BACK, debug.wireframe ? GL_LINE : GL_FILL);
    glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
