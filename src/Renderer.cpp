#include "ChunkConcurrency.hpp"
#include "CoordTypes.hpp"
#include "DebugFormatSpecializations.hpp"
#include "Renderer.hpp"

#include "Profiler.hpp"
#include "Simulation.hpp"
#include "glbinding/gl/functions.h"
#include "glbindingWrapper.hpp"
#include "glmWrapper.hpp"
#include "World.hpp"
#include <algorithm>
#include "LM.hpp"

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

void Renderer::draw_debugChunks_to(Camera& cam, Simulation* sim, RenderTargetView target){
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
    auto sorted = sorted_transparent(cam);
    uploadMeshListToGpu(transparentChunkMeshes,sorted);
}

void Renderer::drawOpaque(Camera& cam){
    //TODO: cache this sort between frames? 
    //This would require some extra fuckery im not bothered with atm
    auto sorted = sorted_opaque(cam);
    uploadMeshListToGpu(opaqueChunkMeshes,sorted);
}

void Renderer::uploadMeshListToGpu(const slot_map<WorldChunkCoord,Mesh>& meshList, std::span<WorldChunkCoord>sortedCoords){
    using std::views::filter;
    using std::views::transform;
    auto isLoaded = [](const Mesh& mesh){ return mesh.isLoaded(); };
    auto isNonEmpty = [](const Mesh& mesh){ return mesh.vertex_count>0; };
    auto meshAt = [&meshList](WorldChunkCoord coord)->const Mesh&{ return meshList.at(coord); };
    auto sorted_meshes = sortedCoords | transform(meshAt);
    auto loadedOpaque = sorted_meshes    |
                        filter(isLoaded) |
                        filter(isNonEmpty);
    for (const auto& mesh : loadedOpaque) {
        auto model = mat4(1.0f);
        const glm::vec3 chunkFloatWorldPos = toWorldOrigin(mesh.chunkCoord).raw();
        model = glm::translate(model, chunkFloatWorldPos);
        prog.setUniform("model", model);
        mesh.draw();
        debug.vertex_count += mesh.offset_count;
        debug.mesh_count++;
        debug.draw_calls++;
    }
}

std::vector<WorldChunkCoord> Renderer::sorted_opaque(Camera& cam){
    // we want nearest -> furthest for early Z culling
    auto source_chunk = toWorldChunkCoord(cam.pos);
    using LM::sq_dist;
    auto closer = [source_chunk](Mesh& lhs, Mesh& rhs){
        return sq_dist(source_chunk,lhs.chunkCoord) < sq_dist(source_chunk,rhs.chunkCoord);
    };
    return transparentChunkMeshes.sorted_keys(closer);
}

std::vector<WorldChunkCoord> Renderer::sorted_transparent(Camera& cam){
    // we want furthest -> nearest for correct transparency
    auto source_chunk = toWorldChunkCoord(cam.pos);
    using LM::sq_dist;
    auto further = [source_chunk](Mesh& lhs, Mesh& rhs){
        return sq_dist(source_chunk,lhs.chunkCoord) > sq_dist(source_chunk,rhs.chunkCoord);
    };
    return transparentChunkMeshes.sorted_keys(further);
}


void Renderer::clear(const vec4 clear_color) {
    enableDepthMask();
    glPolygonMode(GL_FRONT_AND_BACK /*hollow purple reference*/, debug.wireframe ? GL_LINE : GL_FILL);
    glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
