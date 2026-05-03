#pragma once
#include "CommonUtils.hpp"
#include "Shaders.hpp"
#include "Types.h"
#include "glm/ext/matrix_float4x4.hpp"
#include <vector>
#include "CommonUtils.hpp"
#include "Vertex.hpp"

struct VertexArray {
    VertexArray();

    VertexArray(const VertexArray&) = delete;
    VertexArray(VertexArray&&) = default;
    VertexArray& operator=(const VertexArray&) = delete;
    VertexArray& operator=(VertexArray&&) = default;
    ~VertexArray();

    template <std::size_t SZ>
    void apply_layout(const VertexLayout<SZ>& layout) {
        apply_layout_impl(layout.stride, std::span<const VertexAttribute, SZ>(layout.attrs));
    }

    u32  id;
    void bind() const;
    void unbind() const;

    void drawElements(i32 num, i32 elem_t);
    void drawArrays(i32 count, i32 elem_t, i32 offset = 0) const;

  private:
    void apply_layout_impl(i32 stride, std::span<const VertexAttribute> attrs);
};

struct VertexBuffer {
    VertexBuffer();
    ~VertexBuffer();

    // move only type
    VertexBuffer(const VertexBuffer&) = delete;
    VertexBuffer& operator=(const VertexBuffer&) = delete;
    VertexBuffer& operator=(VertexBuffer&&) = default;
    VertexBuffer(VertexBuffer&&) = default;

    u32 id;
    i32 size;

    i32              location = {};
    static const i32 buffer_type;
    void             bind();
    void             load(const std::vector<Vertex>& c, i32 usage_type, i32 offset);
};

struct ElementBuffer {
    ElementBuffer();
    ~ElementBuffer();

    // move only type
    ElementBuffer(const ElementBuffer&) = delete;
    ElementBuffer& operator=(const ElementBuffer&) = delete;
    ElementBuffer(ElementBuffer&&) = default;
    ElementBuffer& operator=(ElementBuffer&&) = default;

    static const i32 buffer_type;
    i32              location = {};

    void bind();

    template <ContiguousContainer C>
    void load(C c, i32 usage_type);

    u32 id;
    i32 size;  // size in bytes
};

// src/Mesh.cpp
struct Mesh {
    Mesh(std::vector<Vertex> vertices);
    ~Mesh() = default;

    // move only
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;
    Mesh(Mesh&&) = default;
    Mesh& operator=(Mesh&&) = default;

    // returns number of draw calls emitted
    i64 draw() const;

    VertexBuffer vbo;
    VertexArray  vao;
    i64          vertex_count{ 0 };
};
