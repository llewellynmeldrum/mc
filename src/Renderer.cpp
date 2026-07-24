#include <algorithm>
#include "cpp23_ranges.hpp"
#include <utility>

#include "ChunkConcurrency.hpp"
#include "CoordTypes.hpp"
#include "DebugChunkLog.hpp"
#include "DebugOptions.hpp"
#include "FormatSpecs.hpp"
#include "Renderer.hpp"

#include "BenchmarkMap.hpp"
#include "Engine.hpp"
#include "glbinding/gl/functions.h"
#include "glbindingWrapper.hpp"
#include "glmWrapper.hpp"
#include "World.hpp"
#include "LM.hpp"

using namespace gl;
using namespace glm;

static bool is_mesh_loaded (const Mesh& mesh){
    return mesh.isLoaded(); 
};
static bool is_mesh_non_empty (const Mesh& mesh){ 
    return mesh.vertex_count>0; 
};

void Renderer::updateViewport(int x, int y, int w, int h) {
    glViewport(x, y, w, h);
}

Renderer::Renderer() {
    prog.load_vtx_and_frag("shaders/vs.glsl", "shaders/fs.glsl");
    prog.use();

    cube_atlas.load_texture("resources/textures/cube_atlas.png");
    cross_atlas.load_texture("resources/textures/cross_atlas.png");
    cactus_atlas.load_texture("resources/textures/cactus_atlas.png");
    half_slab_atlas.load_texture("resources/textures/half_slab_atlas.png");

    atlas_list.push_back(&cube_atlas);
    atlas_list.push_back(&cross_atlas);
    atlas_list.push_back(&cactus_atlas);
    atlas_list.push_back(&half_slab_atlas);

    u_textures_loc = prog.getUniformLoc("textures");
    u_enable_cutout_loc = prog.getUniformLoc("u_enable_cutout");

    u_model_loc = prog.getUniformLoc("model");
    u_proj_loc = prog.getUniformLoc("proj");
    u_view_loc = prog.getUniformLoc("view");
    
    prog.setUniform(
        u_textures_loc,
        std::vector{
            std::to_underlying(BlockShape::CUBE),
            std::to_underlying(BlockShape::CROSS),
            std::to_underlying(BlockShape::CACTUS),
            std::to_underlying(BlockShape::BOT_HALF_SLAB),
            // NOTE: REMINDER!!!!
            // If you are adding a new block type, make sure to change the BLOCK_SHAPE_COUNT in the fragment shader!!!!
            // for the vertices, change only the position ones but not the scale
        }
    );
    prog.stop();
    meshers.launch();

    dbg_rend.setup();
    line3d_rend.setup();

}

// enables depth test, i.e early discard of fragments which are occluded by nearer frags already present
void Renderer::enableDepthTesting(){
    glEnable(GL_DEPTH_TEST);
}

// enables depth test, i.e early discard of fragments which are occluded by nearer frags already present
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
// Disables writes to the depth buffer
void enableDepthMask(){
    glDepthMask(GL_TRUE);
}
// Disables writes to the depth buffer
void disableDepthMask(){
    glDepthMask(GL_FALSE);
}
void Renderer::prepare_blended_pass(){
    enableDepthTesting();
    enableDepthMask();

    enableColorBlending();
    enableBackfaceCulling();
}
void Renderer::prepare_opaque_pass(){
    enableDepthTesting();
    enableDepthMask();


    disableColorBlending();
    enableBackfaceCulling();
}
void Renderer::prepare_cutout_pass(){
    enableDepthTesting();
    enableDepthMask();
    prog.setUniform(u_enable_cutout_loc, true);


    disableColorBlending();
    enableBackfaceCulling();
}
void Renderer::draw_to(Camera& cam, RenderTargetView target, FrameProfiler* prof){
    {
        prof->bench_start("02_rendinit");

        target.use();
        prog.use();
        for (auto* atlas: atlas_list){
            atlas->texture.bind();
        }
        prog.setUniform(u_view_loc, cam.getViewMatrix());
        prog.setUniform(u_proj_loc, cam.getProjectionMatrix());
        prof->bench_end("02_rendinit");
    }

    {
        prof->bench_start("03_opaque");
        prepare_opaque_pass();
        draw_opaque_pass(cam);
        prof->bench_end("03_opaque");
    }

    {
        prof->bench_start("04_cutout");
        prepare_cutout_pass();
        draw_cutout_pass(cam);
        prof->bench_end("04_cutout");
    }

    {
        prof->bench_start("05_blended");
        prepare_blended_pass();
        draw_blended_pass(cam);
        prof->bench_end("05_blended");
    }

    
    prog.stop();
    target.stop();

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

    prepare_blended_pass();
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

    prepare_blended_pass();
    line3d_rend.draw(cam);

    glPolygonMode(GL_FRONT_AND_BACK, debug.wireframe ? GL_LINE : GL_FILL);
    target.stop();
}

void Renderer::clear_to(RenderTargetView target){
    target.use();
    clear(clear_color);
    target.stop();
}


void Renderer::draw_blended_pass(Camera& cam){
    draw_meshes(blended_chunk_meshes,sorted_blended_coords);
}

void Renderer::draw_cutout_pass(Camera& cam){
    if (DebugOption::enable_opaque_sorting){
        draw_meshes(cutout_chunk_meshes, sorted_cutout_coords);
    }else{
        draw_cutout_meshes_unsorted(cutout_chunk_meshes);
    }
}

void Renderer::draw_opaque_pass(Camera& cam){
    if (DebugOption::enable_opaque_sorting){
        draw_meshes(opaque_chunk_meshes,sorted_opaque_coords);
    }else{
        draw_meshes_unsorted(opaque_chunk_meshes);
    }
}

void Renderer::draw_meshes(const slot_map<WorldChunkCoord,Mesh>& mesh_list, 
                                   std::span<WorldChunkCoord>sorted_coords){
    static_assert(map_like<slot_map<WorldChunkCoord,Mesh>>);
    using views::filter;
    using views::transform;
    if (sorted_coords.empty()){
        draw_meshes_unsorted(mesh_list);
        return;
    }
    assert_eq(mesh_list.size(),sorted_coords.size());


    auto get_mesh_at_coord = [&mesh_list](WorldChunkCoord coord)->const Mesh& {
        return AT(mesh_list,coord); 
    };
    auto filtered_meshes = sorted_coords |
            transform(get_mesh_at_coord) |
            filter(is_mesh_loaded) |
            filter(is_mesh_non_empty);

    for (const auto& mesh : filtered_meshes) {
        draw_mesh(mesh);
    }
}

void Renderer::draw_cutout_mesh(const Mesh& mesh){
    if (DebugOption::enable_cutout_optimisation && mesh.is_cutout){
        prog.setUniform(u_enable_cutout_loc, false);
       if (mesh.chunk_dist_to_cam > cutout_enable_radius){
           prog.setUniform(u_enable_cutout_loc, false);
       }else{
           prog.setUniform(u_enable_cutout_loc, true);
       }
    }
    auto model = mat4(1.0f);
    const glm::vec3 chunkFloatWorldPos = toWorldOrigin(mesh.chunkCoord).raw();
    model = glm::translate(model, chunkFloatWorldPos);
    prog.setUniform(u_model_loc, model);
    mesh.draw();
    debug.vertex_count += mesh.offset_count;
    debug.mesh_count++;
    debug.draw_calls++;
}
void Renderer::draw_mesh(const Mesh& mesh){
    auto model = mat4(1.0f);
    const glm::vec3 chunkFloatWorldPos = toWorldOrigin(mesh.chunkCoord).raw();
    model = glm::translate(model, chunkFloatWorldPos);
    prog.setUniform(u_model_loc, model);
    mesh.draw();
    debug.vertex_count += mesh.offset_count;
    debug.mesh_count++;
    debug.draw_calls++;
}

void Renderer::draw_meshes_unsorted(const slot_map<WorldChunkCoord,Mesh>& meshList){
    using views::filter;

    auto filtered_meshes = meshList |
                        filter(is_mesh_loaded) |
                        filter(is_mesh_non_empty);

    for (const auto& mesh : filtered_meshes) {
        draw_mesh(mesh);
    }
}
void Renderer::draw_cutout_meshes_unsorted(const slot_map<WorldChunkCoord,Mesh>& meshList){
    using views::filter;

    auto filtered_meshes = meshList |
                        filter(is_mesh_loaded) |
                        filter(is_mesh_non_empty);

    for (const auto& mesh : filtered_meshes) {
        draw_cutout_mesh(mesh);
    }
}

void Renderer::sort_cutout_chunks(WorldFloatPos cam_pos){
    // we want nearest -> furthest for early Z culling
    auto source_chunk = toWorldChunkCoord(cam_pos);
    using LM::sq_dist;
    auto closer = [source_chunk](const WorldChunkCoord& lhs, const WorldChunkCoord& rhs){
        return sq_dist(source_chunk,lhs) < sq_dist(source_chunk,rhs);
    };
    sorted_cutout_coords = cutout_chunk_meshes.sorted_keys(closer);
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

void Renderer::sort_blended_chunks(WorldFloatPos cam_pos){
    // we want furthest -> nearest for correct transparency
    auto source_chunk = toWorldChunkCoord(cam_pos);
    using LM::sq_dist;
    auto further= [source_chunk](const WorldChunkCoord& lhs, const WorldChunkCoord& rhs){
        return sq_dist(source_chunk,lhs) > sq_dist(source_chunk,rhs);
    };

    sorted_blended_coords = blended_chunk_meshes.sorted_keys(further);
    assert(sorted_blended_coords.size()==blended_chunk_meshes.size());
}


void Renderer::clear(const vec4 clear_color) {
    enableDepthMask();
    glPolygonMode(GL_FRONT_AND_BACK /*hollow purple reference*/, debug.wireframe ? GL_LINE : GL_FILL);
    glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
