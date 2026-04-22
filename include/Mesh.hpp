#pragma once 
#include "CommonUtils.hpp"
#include "Shaders.hpp"
#include "Types.h"
#include "glm/ext/matrix_float4x4.hpp"
#include <vector>
#include "CommonUtils.hpp"
#include "Vertex.hpp"

struct VertexArray{
    VertexArray()=default;
    ~VertexArray()=default;
    void setupVertexArray();

    u32 id;
    i32 buffer_cols;

    void bind() const;
    void unbind() const;

    u32 sum{};
    template <class VT>
    void set_vtx_attributes(u32 location, i32 count, u64 offset);
    // draws *num* vertices, chosen via whichever EBO is bound to this VAO
    void drawElements(u32 num, u32 elem_t);
    void drawArrays(u32 count, u32 elem_t, u32 offset = 0) const;
};
struct VertexBuffer{
    VertexBuffer()=default;
    ~VertexBuffer()=default;
    void setupVertexBuffer();

    u32 id;
    u32 size; // size in bytes
    u32 location={};
    static const u32 buffer_type;
    void bind();
    void load(const std::vector<Vertex>& c, u32 usage_type, u32 offset);
};
struct ElementBuffer{
    ElementBuffer()=default;
    ~ElementBuffer()=default;
    void setupElementBuffer();
    u32 id;
    u32 size; // size in bytes
    // perhaps implement some sort of cast_buffer<T target>()
    static const u32 buffer_type;
    u32 location = {};

    void bind();

    template <ContiguousContainer C>
    void load(C c, u32 usage_type);
};
struct Mesh{
    Mesh()=default;
    ~Mesh()=default; // TODO: add gl destroy stuff?


    void setup(std::vector<Vertex> vertices);
    void draw(ShaderProgram& prog, const mat4& model, const mat4& view, const mat4& proj) const;

    
    VertexBuffer vbo;
    VertexArray vao;
    u64 vertex_count{0};
};
