
#include <memory>
#include "Context.hpp"
#include "Vertex.hpp"
#include "Shaders.hpp"
#include "Mesh.hpp"

#include "glbindingWrapper.hpp"
using namespace gl;

void ElementBuffer::setupElementBuffer() {
    glGenBuffers(1, &this->id);
}
void ElementBuffer::bind() {
    glBindBuffer(static_cast<GLenum>(buffer_type), id);
}

template <ContiguousContainer C>
void ElementBuffer::load(C c, i32 usage_type){
    bind();
    using T = C::value_type;
    static_assert(std::same_as<T,i32>);

    glBufferData(buffer_type, sizeof(c), c.data(), static_cast<GLenum>(usage_type));
}
const i32 ElementBuffer::buffer_type=static_cast<i32>(GL_ELEMENT_ARRAY_BUFFER);
const i32 VertexBuffer::buffer_type=static_cast<i32>(GL_ARRAY_BUFFER);

void VertexBuffer::setupVertexBuffer() {
    glGenBuffers(1, &this->id);
}
void VertexBuffer::bind() {
    glBindBuffer(static_cast<GLenum>(buffer_type), id);
}

void VertexBuffer::load(const std::vector<Vertex>& c, i32 usage_type, i32 offset=0){
    bind();
    glBufferData(static_cast<GLenum>(buffer_type), sizeof(c)* c.size(), c.data()+offset, static_cast<GLenum>(usage_type));
}

void VertexArray::setupVertexArray(){
    glGenVertexArrays(1, &id);
}
void VertexArray::bind() const {
    glBindVertexArray(id);
}

void VertexArray::unbind() const {
    glBindVertexArray(0);
}
void VertexArray::drawElements(i32 num, i32 elem_t) {
    glDrawElements(static_cast<GLenum>(elem_t), num, GL_UNSIGNED_INT, nullptr);
}
void VertexArray::drawArrays(i32 count, i32 elem_t, i32 offset) const {
    glDrawArrays(static_cast<GLenum>(elem_t), offset, count);
}

template <class VT>
void VertexArray::set_vtx_attributes(i32 location, i32 count, i64 offset){
    // assumed packed non normalized vertex data
    void* offset_ptr = (void*)(offset*sizeof(VT));
    i32 stride  = buffer_cols * sizeof(VT);
    GLenum type = gl_type<VT>();
    glVertexAttribPointer(location, count, type, false,stride, offset_ptr);
    glEnableVertexAttribArray(location);
    sum+=count;
}




enum CubeFace{
    CubeFaceBACK,
    CubeFaceFRONT,
    CubeFaceLEFT,
    CubeFaceRIGHT,
    CubeFaceBOT,
    CubeFaceTOP,
};


void Mesh::setup(std::vector<Vertex>& vertices) {
    vao.setupVertexArray();
    vbo.setupVertexBuffer();
   
    vertex_count = vertices.size();
    vao.bind();
    vbo.load(vertices, to_i32(GL_STATIC_DRAW));
    vao.buffer_cols = 5;                    // x,y,z, s,t
    vao.set_vtx_attributes<f32>(0, 3, +0);  // x,y,z
    vao.set_vtx_attributes<f32>(1, 2, +3);  // s,t
    vao.unbind();
}

i64 Mesh::draw(ShaderProgram& prog, const mat4& model, const mat4& view, const mat4& proj) const {
    prog.use();
    vao.bind();
    prog.setUniform("model", model);
    prog.setUniform("view", view);
    prog.setUniform("proj", proj);
    vao.drawArrays(vertex_count, static_cast<i32>(GL_TRIANGLES), 0);  // front
    vao.unbind();
    prog.stop();
    return 1;
}

