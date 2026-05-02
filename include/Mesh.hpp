#pragma once
#include "CommonUtils.hpp"
#include "Shaders.hpp"
#include "Types.h"
#include "glm/ext/matrix_float4x4.hpp"
#include <vector>
#include "CommonUtils.hpp"
#include "Vertex.hpp"

struct VertexArray {
    VertexArray() = default;
    ~VertexArray() = default;
    void setupVertexArray();

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
    VertexBuffer() = default;
    ~VertexBuffer() = default;
    void setupVertexBuffer();

    u32              id;
    i32              size;  // size in bytes
    i32              location = {};
    static const i32 buffer_type;
    void             bind();
    void             load(const std::vector<Vertex>& c, i32 usage_type, i32 offset);
};
struct ElementBuffer {
    ElementBuffer() = default;
    ~ElementBuffer() = default;
    void setupElementBuffer();
    u32  id;
    i32  size;  // size in bytes
    // perhaps implement some sort of cast_buffer<T target>()
    static const i32 buffer_type;
    i32              location = {};

    void bind();

    template <ContiguousContainer C>
    void load(C c, i32 usage_type);
};
// src/Mesh.cpp
struct Mesh {
    Mesh() = default;
    ~Mesh() = default;  // TODO: add gl destroy stuff?

    void setup(std::vector<Vertex>& vertices);
    // returns number of draw calls emitted
    i64 draw(ShaderProgram& prog, const mat4& model, const mat4& view, const mat4& proj,
             const f32& blockOverlayOpacity) const;

    VertexBuffer vbo;
    VertexArray  vao;
    i64          vertex_count{ 0 };
};
