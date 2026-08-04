#include "Skybox.hpp"
#include "BufferObjects.hpp"
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
    prog.load_vtx_and_frag("shaders/skybox_vs.glsl","shaders/skybox_fs.glsl");
    prog.use();

    vbo.make();
    vao.make();

    vao.bind();
        vbo.bind();
        apply_vertex_layout<SkyboxRenderer::Vertex>();
    vao.unbind();


    u_base_color_loc = prog.getUniformLoc("u_base_color");
    prog.stop();
}
void SkyboxRenderer::update_rays(Camera const& cam){
    auto half_h = glm::tan(glm::radians(cam.vertical_fov)) * 0.5f;
    auto half_w = half_h * cam.aspectRatio;
    auto front = cam.getFront();
    auto right = cam.getRight();
    auto up = cam.getUp();
    auto make_corner_ray = [&](glm::vec2 ndc){
        return glm::normalize(
            front + ndc.x * half_h * right
                  + ndc.y * half_h * up);
    };
    for (i32 corner_idx = 0; corner_idx < 4 ; corner_idx++){
        auto v_pos = fullscreen_quad_corners[corner_idx];
        rays[corner_idx].pos = v_pos;
        rays[corner_idx].ray = make_corner_ray(v_pos);
    }
}
void SkyboxRenderer::per_tick_update(Camera const& cam){
    prog.use();
    prog.setUniform(u_base_color_loc, base_color);
    vbo.bind();
    update_rays(cam);
    vbo.load(const_span{rays});
    vbo.unbind();
}
void SkyboxRenderer::draw(){
    vao.bind();
    prog.use();
    vao.drawArrays(rays.size(),PrimitiveType());
    prog.stop();
    vao.unbind();
}
