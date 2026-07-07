#include "ChunkConcurrency.hpp"
#include "CoordTypes.hpp"
#include "DebugChunkLog.hpp"
#include "FormatSpecs.hpp"
#include "Renderer.hpp"

#include "Profiler.hpp"
#include "Engine.hpp"
#include "glbinding/gl/functions.h"
#include "glbindingWrapper.hpp"
#include "glmWrapper.hpp"
#include "World.hpp"
#include <algorithm>
#include <ranges>
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

void Renderer::update_player_cam_frustum_lines(Engine* sim){
    // always update the debug rends' radius and chunks with the player camera
    auto make_frustum_lines_for = [](const Frustum& frustum, glm::vec4 color){
        std::vector<Line3D> frustum_lines;
        frustum.path.publish(frustum_lines,color);
        frustum_lines.append_range(frustum.extra_lines);
        return frustum_lines;
    };
    player_cam_frustum_lines = make_frustum_lines_for(sim->player_cam.getCullFrustum(),glm::vec4{1,1,1,1});
    player_cam_frustum_lines.append_range(make_frustum_lines_for(sim->player_cam.getViewFrustum(),glm::vec4{1,1,1,0.5}));
}
void Renderer::draw_debugChunks_to(Camera& cam, Engine* sim, RenderTargetView target){
    target.use();
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(-1.0f, -1.0f); // Fix z-fighting 

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    beginTransparentPass();
    dbg_rend.draw(cam);

    glPolygonMode(GL_FRONT_AND_BACK, debug.wireframe ? GL_LINE : GL_FILL);

    glPolygonOffset(0.0f, 0.0f);
    glDisable(GL_POLYGON_OFFSET_FILL);
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
//    for (const auto& [idx, coord]: std::views::enumerate(sorted)){
//        log_to_chunk(coord,"priority: {}/{}",idx,sorted.size());
//    }
    uploadMeshListToGpu(transparentChunkMeshes,sorted);
}

void Renderer::drawOpaque(Camera& cam){
    //TODO: cache this sort between frames? 
    //This would require some extra fuckery im not bothered with atm
    auto sorted = sorted_opaque(cam);
    uploadMeshListToGpu(opaqueChunkMeshes,sorted);
}

void Renderer::uploadMeshListToGpu(const slot_map<WorldChunkCoord,Mesh>& meshList, 
                                   std::span<WorldChunkCoord>sortedCoords){
    using std::views::filter;
    using std::views::transform;
    auto is_mesh_loaded = [](const Mesh& mesh){
        return mesh.isLoaded(); 
    };
    auto is_mesh_non_empty = [](const Mesh& mesh){ 
        return mesh.vertex_count>0; 
    };
    auto get_mesh_at_coord = [&meshList](WorldChunkCoord coord)->const Mesh& {
        return meshList.at(coord); 
    };
    auto filtered_meshes = sortedCoords |
                        transform(get_mesh_at_coord) |
                        filter(is_mesh_loaded) |
                        filter(is_mesh_non_empty);
    for (const auto& mesh : filtered_meshes) {
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
    auto closer = [source_chunk](const WorldChunkCoord& lhs, const WorldChunkCoord& rhs){
        return sq_dist(source_chunk,lhs) < sq_dist(source_chunk,rhs);
    };
    return opaqueChunkMeshes.sorted_keys(closer);
}

std::vector<WorldChunkCoord> Renderer::sorted_transparent(Camera& cam){
    // we want furthest -> nearest for correct transparency
    auto source_chunk = toWorldChunkCoord(cam.pos);
    using LM::sq_dist;
    auto further= [source_chunk](const WorldChunkCoord& lhs, const WorldChunkCoord& rhs){
        return sq_dist(source_chunk,lhs) > sq_dist(source_chunk,rhs);
    };

    return transparentChunkMeshes.sorted_keys(further);
    //BUG: 
    //Optimizations are disabled and the meshes are not being correctly marked as completed on the mesh queue.
    // Add a debug ui thing to track the mesh generation of the current mesh. See how that moves
}


void Renderer::clear(const vec4 clear_color) {
    enableDepthMask();
    glPolygonMode(GL_FRONT_AND_BACK /*hollow purple reference*/, debug.wireframe ? GL_LINE : GL_FILL);
    glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
