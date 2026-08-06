#include "Skybox.hpp"
#include "BufferObjects.hpp"
#include "ChunkMesher_RawData.hpp"
#include "SkyboxState.hpp"
#include "VertexLayoutHelpers.hpp"
#include "Camera.hpp"
#include "glbindingWrapper.hpp"
#include "glm/geometric.hpp"
#include <range/v3/view/enumerate.hpp>
using namespace gl;
constexpr GLenum SkyboxRenderer::PrimitiveType() {
    return GL_TRIANGLES;
}

void SkyboxRenderer::setup(){
    vao.make();
    vbo.make();
    ebo.make();
    prog.load_vtx_and_frag("shaders/skybox_vs.glsl","shaders/skybox_fs.glsl");

    vao.bind();
        vbo.bind();
        apply_vertex_layout<SkyboxRenderer::Vertex>();

        ebo.bind();
        ebo.load_indices(quad_indices);
    vao.unbind();

    prog.use();
    u_base_color_loc = prog.getUniformLoc("u_base_color");
    u_color_horizon_loc = prog.getUniformLoc("u_color_horizon");     // color of the sky at the horizon line
    u_color_sun_loc = prog.getUniformLoc("u_color_sun");
    u_color_sunlight_loc = prog.getUniformLoc("u_color_sunlight");
    u_color_sun_glow_loc = prog.getUniformLoc("u_color_sun_glow");
    u_color_zenith_loc = prog.getUniformLoc("u_color_zenith");      // color of the sky at the highest point (top center)
    u_dir_moon_loc = prog.getUniformLoc("u_dir_moon");
    u_dir_sun_loc = prog.getUniformLoc("u_dir_sun");
    u_sun_intensity_scale_loc = prog.getUniformLoc("u_sun_intensity_scale");
    u_star_alpha_loc = prog.getUniformLoc("u_star_alpha");
    u_glow_scale_loc= prog.getUniformLoc("u_glow_scale");
    u_color_lowsky_loc= prog.getUniformLoc("u_color_lowsky");
    u_color_midsky_loc= prog.getUniformLoc("u_color_midsky");
    prog.stop();
}

void SkyboxRenderer::upload_rays(){
    vao.bind();
    vbo.load_vertices(rays);
    vao.unbind();
}

void SkyboxRenderer::update_rays(Camera const& cam){
    auto half_h = glm::tan(glm::radians(cam.vertical_fov.get()) * 0.5f);
    auto half_w = half_h * cam.aspectRatio;
    auto front = cam.getFront();
    auto right = cam.getRight();
    auto up = cam.getUp();
    auto make_corner_ray = [&](glm::vec2 ndc){
        auto hoz_offset = ndc.x * half_w * right;
        auto ver_offset = ndc.y * half_h * up;
        return glm::normalize(front + hoz_offset + ver_offset);
    };
    for (i32 corner_idx = 0; corner_idx < 4 ; corner_idx++){
        auto v_pos = fullscreen_quad_corners_ndc[corner_idx];
        rays[corner_idx].pos = v_pos;
        rays[corner_idx].ray = make_corner_ray(v_pos);
    }
}
void SkyboxRenderer::per_frame_update(Camera const& cam, SkyboxState const& state){
    update_rays(cam);
    upload_rays();
    prog.use();
    update_uniforms(state);
    prog.setUniform(u_base_color_loc, base_color);
}
void SkyboxRenderer::draw(){
    vao.bind();
    prog.use();
    vao.drawElements(quad_indices.size(),PrimitiveType());
    prog.stop();
    vao.unbind();
}
