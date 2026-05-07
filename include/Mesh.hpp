#pragma once
#include <vector>

#include "Types.h"
#include "CommonUtils.hpp"
#include "cppslop.hpp"

#include "BufferObjects.hpp"
#include "Shaders.hpp"
#include "Vertex.hpp"

FORWARD_DECL_ENUM_STRUCT(gl, GLenum, unsigned int)

// meshes own PrimitiveMode() -> GL_TRIANGLES, GL_LINES, GL_STRIP
// VBO/EBO's own StorageUsage() -> GL_ELEMENT_ARRAY_BUFFER, GL_ARRAY_BUFFER
// VBO/EBO's own BufferTarget() ->  GL_STATIC_DRAW, GL_DYNAMIC_DRAW
struct MeshBase {
    DECL_MOVE_ONLY(MeshBase);
    ~MeshBase() = default;
    MeshBase(std::vector<Vertex> vertices) { MeshBase::setupMesh(vertices); }

    void setupMesh(std::vector<Vertex> vertices);
    void draw() const;

    i64          vertex_count{ 0 };  // if EBO, index count.
    VertexBuffer vbo;
    VertexArray  vao;

  private:
    constexpr static gl::GLenum PrimitiveType();
};

struct IndexedMesh {
    DECL_MOVE_ONLY(IndexedMesh);
    ~IndexedMesh() = default;
    IndexedMesh(std::vector<Vertex> vertices, std::vector<u32> offsets) {
        IndexedMesh::setupMesh(vertices, offsets);
    }

    void setupMesh(std::vector<Vertex> vertices, std::vector<u32> offsets);
    void draw() const;

    i64           offset_count{ 0 };  // if EBO, index count.
    VertexBuffer  vbo;
    VertexArray   vao;
    ElementBuffer ebo;

  private:
    constexpr static gl::GLenum PrimitiveType();
};

// by default, im using indexed meshes
using Mesh = IndexedMesh;
