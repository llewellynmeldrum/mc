

#include "DEBUG.hpp"
#include "DebugFormatSpecializations.hpp"
#include <memory>
#include "Context.hpp"
#include "Vertex.hpp"
#include "Shaders.hpp"
#include "Mesh.hpp"

#include "VertexAttributeType.hpp"
#include "glbindingWrapper.hpp"

using namespace gl;
const i32 ElementBuffer::buffer_type = static_cast<i32>(GL_ELEMENT_ARRAY_BUFFER);
const i32 VertexBuffer::buffer_type = static_cast<i32>(GL_ARRAY_BUFFER);

template <ContiguousContainer Cont>
void ElementBuffer::load(Cont c, i32 usage_type) {
    bind();
    using Item = Cont::value_type;
    static_assert(std::same_as<i32, Item>);

    glBufferData(to_glenum(buffer_type), sizeof(i32), c.data(), to_glenum(usage_type));
}

void VertexBuffer::load(const std::vector<Vertex>& c, i32 usage_type, i32 offset = 0) {
    bind();
    glBufferData(to_glenum(buffer_type), sizeof(Vertex) * c.size(), c.data() + offset,
                 to_glenum(usage_type));
}

void VertexArray::apply_layout_impl(i32 stride, std::span<const VertexAttribute> attrs) {
    int i = 0;
    for (const auto& attr : attrs) {
        if (attr.is_integer) {
            glVertexAttribIPointer(      //
                attr.location,           //
                attr.count,              //
                gl_type_from(attr.vat),  //
                stride,                  //
                attr.offset_ptr          //
            );
        } else {
            glVertexAttribPointer(       //
                attr.location,           //
                attr.count,              //
                gl_type_from(attr.vat),  //
                attr.normalized,         //
                stride,                  //
                attr.offset_ptr          //
            );
        }
        glEnableVertexAttribArray(attr.location);
    }
}

Mesh::Mesh(std::vector<Vertex> vertices) {
    vertex_count = vertices.size();
    vao.bind();
    vbo.load(vertices, to_i32(GL_STATIC_DRAW));
    vao.apply_layout(Vertex::layout());
    vao.unbind();
}

i64 Mesh::draw() const {
    vao.bind();
    vao.drawArrays(vertex_count, static_cast<i32>(GL_TRIANGLES), 0);
    vao.unbind();
    return 1;
}

// |||||||||||||||||||||||||||||||||||
//  below are all 1 line wrappers
// |||||||||||||||||||||||||||||||||||

VertexArray::VertexArray() {
    glGenVertexArrays(1, &id);
}
ElementBuffer::ElementBuffer() {
    glGenBuffers(1, &id);
}
VertexBuffer::VertexBuffer() {
    glGenBuffers(1, &id);
}

VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &id);
}
ElementBuffer::~ElementBuffer() {
    glDeleteBuffers(1, &id);
}
VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &id);
}

void VertexArray::bind() const {
    glBindVertexArray(id);
}

void VertexBuffer::bind() {
    glBindBuffer(to_glenum(buffer_type), id);
}
void ElementBuffer::bind() {
    glBindBuffer(to_glenum(buffer_type), id);
}

void VertexArray::unbind() const {
    glBindVertexArray(0);
}

void VertexArray::drawElements(i32 num, i32 elem_t) {
    glDrawElements(to_glenum(elem_t), num, GL_UNSIGNED_INT, nullptr);
}
void VertexArray::drawArrays(i32 count, i32 elem_t, i32 offset) const {
    glDrawArrays(to_glenum(elem_t), offset, count);
}
