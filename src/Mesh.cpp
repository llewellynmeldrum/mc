

#include "DEBUG.hpp"
#include "DebugFormatSpecializations.hpp"
#include <memory>
#include "Simulation.hpp"
#include "Vertex.hpp"
#include "Shaders.hpp"
#include "Mesh.hpp"

#include "VertexAttributeType.hpp"

#include "glbindingWrapper.hpp"
using namespace gl;

// NOTE: in gl, known as GL_TRIANGLES, GL_LINES, GL_STRIP
constexpr GLenum MeshBase::PrimitiveType() {
    return GL_TRIANGLES;
}
constexpr GLenum IndexedMesh::PrimitiveType() {
    return GL_TRIANGLES;
}

void IndexedMesh::setupMesh(std::vector<Vertex> vertices, std::vector<u32> indices) {
    offset_count = indices.size();
    vao.bind();
    assert((indices.size() / 6) * 4 == vertices.size());
    vbo.load<Vertex>(vertices);
    ebo.load(indices);
    vao.apply_layout(Vertex::layout());
    vao.unbind();
}
void IndexedMesh::draw() const {
    assert(offset_count != 0);
    vao.bind();
    vao.drawElements(offset_count, PrimitiveType());
    vao.unbind();
}

void MeshBase::setupMesh(std::vector<Vertex> vertices) {
    vertex_count = vertices.size();
    vao.bind();
    vbo.load<Vertex>(vertices);
    vao.apply_layout(Vertex::layout());
    vao.unbind();
}
void MeshBase::draw() const {
    vao.bind();
    vao.drawArrays(vertex_count, PrimitiveType(), 0);
    vao.unbind();
}

//
//
//
//
//
//
// |||||||||||||||||||||||||||||||||||
//  below are all 1 line wrappers
// |||||||||||||||||||||||||||||||||||
