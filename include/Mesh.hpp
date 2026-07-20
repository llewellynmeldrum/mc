#pragma once
#include <vector>

#include "CoordTypes.hpp"
#include "Types.h"
#include "CommonUtils.hpp"
#include "CommonConcepts.hpp"
#include "cppslop.hpp"
#include "ChunkConstants.hpp"

#include "BufferObjects.hpp"
#include "Shaders.hpp"
#include "Vertex.hpp"
#include "glm/gtx/norm.hpp"

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

struct QuadIndices{
    std::array<u32,INDICES_PER_QUAD> arr;
    auto begin(){return arr.begin();};
    auto end(){return arr.end();};
};
static_assert(c_style_layout<QuadIndices>);

struct IndexedMesh {
    DECL_MOVE_ONLY(IndexedMesh);
    WorldChunkCoord chunkCoord;
    IndexedMesh() = default;
    ~IndexedMesh() = default;
    IndexedMesh(WorldChunkCoord _chunkCoord, const_span<Vertex> vertices, const_span<u32> offsets, bool is_cutout=false);

    inline void unload() noexcept{loaded=false; }
    inline void load() noexcept{loaded=true; }
    inline bool isLoaded()const noexcept{return loaded; }


    // =========
    // cpu side cache of the vertices and offsets, for the purpose of sorting transparent faces
    // =========
    std::vector<QuadIndices> ebo_cache; // this has the same byte layout as a regular ebo (std::span<u32>).
    std::vector<glm::vec3> center_points; 

    // sort the cpu side ebo cache
    void resort_quad_indices(WorldFloatPos src_world, bool near_to_far=false);

    void setup_mesh(const_span<Vertex> vertices, const_span<u32> offsets);
    void reupload_indices(const_span<QuadIndices> new_offsets);
    void draw() const;
    void draw_nobind() const ;

    i32             offset_count{ 0 };
    i32             vertex_count{ 0 };
    VertexArray   vao;
    VertexBuffer  vbo;
    ElementBuffer ebo;

    bool is_cutout{false};
    i32 chunk_dist_to_cam{0};
  private:
    constexpr static auto PrimitiveType() -> gl::GLenum;

    bool loaded=true;   // whether or not it will be uploaded to the gpu this frame
                        // meshes are 'unloaded' if they are outside the frustum (later obscured by other meshes)
                        // Meaning that they exist, and are maybe clean, but dont need to be sent to gpu
};


// by default, im using indexed meshes
using Mesh = IndexedMesh;
