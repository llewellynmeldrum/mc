#pragma once
#include <vector>

#include "CoordTypes.hpp"
#include "Types.h"
#include "CommonUtils.hpp"
#include "cppslop.hpp"

#include "BufferObjects.hpp"
#include "Shaders.hpp"
#include "Vertex.hpp"

FORWARD_DECL_ENUM_STRUCT_NS(gl, GLenum, unsigned int)

// meshes own PrimitiveMode() -> GL_TRIANGLES, GL_LINES, GL_STRIP
// VBO/EBO's own StorageUsage() -> GL_ELEMENT_ARRAY_BUFFER, GL_ARRAY_BUFFER
// VBO/EBO's own BufferTarget() ->  GL_STATIC_DRAW, GL_DYNAMIC_DRAW
struct MeshBase {
    DECL_MOVE_ONLY(MeshBase);
    ~MeshBase() = default;
    MeshBase(std::vector<Vertex> vertices) { MeshBase::setupMesh(vertices); }

    void setupMesh(std::vector<Vertex> vertices);
    void draw() const;

    i64          vertex_count{ 0 };  
    VertexBuffer vbo;
    VertexArray  vao;

  private:
    constexpr static auto PrimitiveType() -> gl::GLenum;
};

struct IndexedMesh {
    DECL_MOVE_ONLY(IndexedMesh);
    WorldChunkCoord chunkCoord;
    IndexedMesh() = default;
    ~IndexedMesh() = default;
    IndexedMesh(WorldChunkCoord _chunkCoord, std::vector<Vertex> vertices, std::vector<u32> offsets) {
        chunkCoord = _chunkCoord;
        IndexedMesh::setupMesh(vertices, offsets);
    }

    void setupMesh(std::vector<Vertex> vertices, std::vector<u32> offsets);
    void draw() const;
    inline void unload() noexcept{loaded=false; }
    inline void load() noexcept{loaded=true; }
    inline bool isLoaded()const noexcept{return loaded; }

    i32             offset_count{ 0 };
    i32             vertex_count{ 0 };
    VertexArray   vao;
    VertexBuffer  vbo;
    ElementBuffer ebo;

  private:
    constexpr static auto PrimitiveType() -> gl::GLenum;

    bool loaded=true;   // whether or not it will be uploaded to the gpu this frame
                        // meshes are 'unloaded' if they are outside the frustum (later obscured by other meshes)
                        // Meaning that they exist, and are maybe clean, but dont need to be sent to gpu
};

// by default, im using indexed meshes
using Mesh = IndexedMesh;
