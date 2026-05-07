#pragma once
#include "Types.h"
#include "Vertex.hpp"
#include "cppslop.hpp"
#include <cstddef>

FORWARD_DECL_ENUM_STRUCT(gl, GLenum, unsigned int)

struct VertexArray {
    DECL_MOVE_ONLY(VertexArray);
    VertexArray();
    ~VertexArray();

    void bind() const;
    void unbind() const;

    void drawElements(i32 num, gl::GLenum usage_hint) const;
    void drawArrays(i32 count, gl::GLenum usage_hint, i32 offset = 0) const;

    template <std::size_t AttrCount>
    void apply_layout(const VertexLayout<AttrCount>& layout) {
        using VertexAttrSpan = std::span<const VertexAttribute, AttrCount>;
        apply_layout_impl(layout.stride, VertexAttrSpan(layout.attrs));
    }

  private:
    void apply_layout_impl(i32 stride, std::span<const VertexAttribute> attrs);
    u32  id;
};

struct VertexBuffer {
    DECL_MOVE_ONLY(VertexBuffer);
    VertexBuffer();
    ~VertexBuffer();

    void bind() const;
    void unbind() const;

    void load(std::span<const Vertex> c, i32 offset = 0);

  private:
    constexpr static gl::GLenum BufferUsage();
    constexpr static gl::GLenum BufferTarget();
    u32                         id;
};

struct ElementBuffer {
    DECL_MOVE_ONLY(ElementBuffer);
    ElementBuffer();
    ~ElementBuffer();

    void bind() const;
    void unbind() const;

    void load(std::span<const u32> indices, i32 offset = 0);

  private:
    constexpr static gl::GLenum BufferUsage();
    constexpr static gl::GLenum BufferTarget();
    u32                         id;
};
