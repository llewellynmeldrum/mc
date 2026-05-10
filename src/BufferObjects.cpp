#include "BufferObjects.hpp"
#include "glbindingWrapper.hpp"

using namespace gl;
// TODO: Refactor these as type traits. you know they should be.
//  NOTE: In gl, known as 'usage' param to glBufferData(target, ...)
//  eg: GL_ELEMENT_ARRAY_BUFFER, GL_ARRAY_BUFFER
constexpr GLenum ElementBuffer::BufferUsage() {
    return GL_STATIC_DRAW;
}
// NOTE: In gl, known as 'usage' param to glBufferData(target, ...)
// eg: GL_ELEMENT_ARRAY_BUFFER, GL_ARRAY_BUFFER
constexpr GLenum VertexBuffer::BufferUsage() {
    return GL_STATIC_DRAW;
}

// NOTE: In gl, known as 'target' param to glBufferData(target, ...) and glBindBuffer(target, ...)
// eg: GL_STATIC_DRAW, GL_DYNAMIC_DRAW
constexpr GLenum ElementBuffer::BufferTarget() {
    return GL_ELEMENT_ARRAY_BUFFER;
}
// NOTE: In gl, known as 'target' param to glBufferData(target, ...) and glBindBuffer(target, ...)
// eg: GL_STATIC_DRAW, GL_DYNAMIC_DRAW
constexpr GLenum VertexBuffer::BufferTarget() {
    return GL_ARRAY_BUFFER;
}
void ElementBuffer::load(std::span<const u32> indices, i32 offset) {
    this->bind();
    assert(indices.size() != 0);
    glBufferData(BufferTarget(), indices.size_bytes(), indices.data() + offset, BufferUsage());
}

void VertexBuffer::load(std::span<const Vertex> c, i32 offset) {
    this->bind();
    glBufferData(BufferTarget(), c.size_bytes(), c.data() + offset, BufferUsage());
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

void VertexArray::make() {
    glGenVertexArrays(1, &id);
}
void ElementBuffer::make() {
    glGenBuffers(1, &id);
}
void VertexBuffer::make() {
    glGenBuffers(1, &id);
}

void VertexArray::destroy() {
    if (id) {
        glDeleteVertexArrays(1, &id);
        id = 0;
    }
}
void ElementBuffer::destroy() {
    if (id) {
        glDeleteBuffers(1, &id);
        id = 0;
    }
}
void VertexBuffer::destroy() {
    if (id) {
        glDeleteBuffers(1, &id);
        id = 0;
    }
}

void VertexArray::bind() const {
    glBindVertexArray(id);
}

void VertexBuffer::bind() const {
    glBindBuffer(BufferTarget(), id);
}
void ElementBuffer::bind() const {
    glBindBuffer(BufferTarget(), id);
}

void VertexBuffer::unbind() const {
    glBindBuffer(BufferTarget(), 0);
}
void ElementBuffer::unbind() const {
    glBindBuffer(BufferTarget(), 0);
}
void VertexArray::unbind() const {
    glBindVertexArray(0);
}

void VertexArray::drawElements(i32 num, GLenum usage_hint) const {
    assert(num != 0);
    glDrawElements(usage_hint, num, GL_UNSIGNED_INT, nullptr);
}
void VertexArray::drawArrays(i32 count, GLenum usage_hint, i32 offset) const {
    glDrawArrays(usage_hint, offset, count);
}
