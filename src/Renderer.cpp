#include "ChunkConcurrency.hpp"
#include "CoordTypes.hpp"
#include "DebugChunkLog.hpp"
#include "DebugOptions.hpp"
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
    draw_meshes(transparent_chunk_meshes,sorted_transparent_coords);
}

void Renderer::drawOpaque(Camera& cam){

    if (DebugOption::enable_opaque_sorting){
        draw_meshes(opaque_chunk_meshes,sorted_opaque_coords);
    }else{
        draw_meshes_unsorted(opaque_chunk_meshes);
    }
}

void Renderer::draw_meshes(const slot_map<WorldChunkCoord,Mesh>& mesh_list, 
                                   std::span<WorldChunkCoord>sorted_coords){
    static_assert(map_like<slot_map<WorldChunkCoord,Mesh>>);
    using std::views::filter;
    using std::views::transform;
//    if (sorted_coords.empty() || meshList.size() != sorted_coords.size()){
    if (sorted_coords.empty()){
        draw_meshes_unsorted(mesh_list);
        return;
    }
    assert_eq(mesh_list.size(),sorted_coords.size());


    auto is_mesh_loaded = [](const Mesh& mesh){
        return mesh.isLoaded(); 
    };
    auto is_mesh_non_empty = [](const Mesh& mesh){ 
        return mesh.vertex_count>0; 
    };
    auto get_mesh_at_coord = [&mesh_list](WorldChunkCoord coord)->const Mesh& {
        return AT(mesh_list,coord); 
    };
    auto filtered_meshes = sorted_coords |
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
void Renderer::draw_meshes_unsorted(const slot_map<WorldChunkCoord,Mesh>& meshList){
    using std::views::filter;
    using std::views::transform;

    auto is_mesh_loaded = [](const Mesh& mesh){
        return mesh.isLoaded(); 
    };
    auto is_mesh_non_empty = [](const Mesh& mesh){ 
        return mesh.vertex_count>0; 
    };
    auto filtered_meshes = meshList |
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

void Renderer::sort_opaque_chunks(WorldFloatPos cam_pos){
    // we want nearest -> furthest for early Z culling
    auto source_chunk = toWorldChunkCoord(cam_pos);
    using LM::sq_dist;
    auto closer = [source_chunk](const WorldChunkCoord& lhs, const WorldChunkCoord& rhs){
        return sq_dist(source_chunk,lhs) < sq_dist(source_chunk,rhs);
    };
    sorted_opaque_coords = opaque_chunk_meshes.sorted_keys(closer);
}

void Renderer::sort_transparent_chunks(WorldFloatPos cam_pos){
    // we want furthest -> nearest for correct transparency
    auto source_chunk = toWorldChunkCoord(cam_pos);
    using LM::sq_dist;
    auto further= [source_chunk](const WorldChunkCoord& lhs, const WorldChunkCoord& rhs){
        return sq_dist(source_chunk,lhs) > sq_dist(source_chunk,rhs);
    };

    sorted_transparent_coords = transparent_chunk_meshes.sorted_keys(further);
    assert(sorted_transparent_coords.size()==transparent_chunk_meshes.size());
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
