#include "ChunkConstants.hpp"
#include "GeometryConstants.hpp"
#include "ChunkEntry.hpp"
#include "Line3D.hpp"
#include "Renderer.hpp"
#include "Line3DRenderer.hpp"
#include "World.hpp"
#include <concepts>
using namespace gl;



// we create a 'line segment' base quad, shared among all instances.

// This quad has 4 vertices, (6 indices form the two triangles in CCW order)
//
// the x axis encodes `t`, i.e whether or not the vertex occurs at the 'start' of the line, or the 'end'.
// t (is within){0,1}
//t=    0.......1
//      -------->
//
// The y axis encodes the 'perpendicular offset' of the vertex.
//perp  (is within){-1,1}
//
// For a line of thickness 3 (terminal rows):
//perp=1    |
//          -------->
//perp=-1   |
// These two could both be encoded differently, perhaps as bools, but this format allows
// for a (closer to at least) branchless vtx shader.
// i.e we can get the current pos of any vertex with mix(w_start, w_end, t)

constexpr std::array<Line3DVertex, VTX_PER_QUAD> Line3DVertices={
    Line3DVertex{{0, -1}},
    Line3DVertex{{1, -1}},
    Line3DVertex{{1,  1}},
    Line3DVertex{{0, 1}},
};
constexpr std::array<u32,INDICES_PER_QUAD> Line3DIndices{
    0,1,2,2,3,0
};

void Line3DRenderer::setup(){
    vao.make();
    quad_vbo.make();
    quad_ebo.make();
    instance_vbo.make();

    prog.load_vtx_and_frag("shaders/line3d_vs.glsl", "shaders/line3d_fs.glsl");

    vao.bind();

    quad_vbo.bind();
    quad_vbo.load<Line3DVertex>(Line3DVertices);
    vao.apply_layout(Line3DVertex::layout());

    instance_vbo.bind();
    vao.apply_layout(Line3DInstance::layout());

    quad_ebo.bind();
    quad_ebo.load(Line3DIndices);
    vao.unbind();
}

GLenum Line3DRenderer::PrimitiveType(){
    return GL_TRIANGLES;
}
void Line3DRenderer::draw(Camera& cam){
    prog.use();
    prog.setUniform("view",cam.getViewMatrix());
    prog.setUniform("proj",cam.getProjectionMatrix());
    vao.bind();
    vao.drawElementsInstanced(INDICES_PER_QUAD, instances.size(), PrimitiveType());
    vao.unbind();
    prog.stop();
}

void Line3DRenderer::update(Camera& cam, std::span<Line3D> lines){
    updateInstances(cam,lines);

    vao.bind();
    instance_vbo.load<Line3DInstance>(instances);
    vao.unbind();
}
decltype(auto) makeLine3DInstance(const Line3D& val){
    return Line3DInstance{
        .w_start=val.p0,
        .w_end=val.p1,
        .thickness=val.thickness,
        .color=val.color,
    };
}
void Line3DRenderer::updateInstances(Camera& cam, std::span<Line3D> lines){
    instances.clear();
    for (auto& line: lines){
//        line.color=cam.lineColor;
        instances.emplace_back(makeLine3DInstance(line));
    }
}
