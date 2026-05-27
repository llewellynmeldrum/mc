#pragma once
#include "Camera.hpp"
#include "CoordTypes.hpp"
#include "Shaders.hpp"
#include "Mesh.hpp"
#include "TextureAtlas.hpp"
#include "ChunkMesher.hpp"
#include <map>
#include <optional>
#include <unordered_map>

#include "Chunk.hpp"
struct DebugChunkVertex{
    glm::ivec3 pos; // Chunk local vertex position
    static constexpr auto layout() {
        return VertexLayout<1>{ 
            .stride = sizeof(DebugChunkVertex),
            .attrs = {
               make_attr<glm::ivec3>(0, offsetof(DebugChunkVertex, pos)),
            }, 
        };
    }
};
struct DebugChunkInstance{
    glm::ivec3 origin; // world pos of the chunk (i.e instance origin)
    glm::vec4 color; // the color of the debug overlay (i.e instance color )
    static constexpr auto layout() {
        return VertexLayout<2>{ 
            .stride = sizeof(DebugChunkInstance),
            .attrs = {
               make_attr<glm::ivec3>(1, offsetof(DebugChunkInstance, origin), 1),
               make_attr<glm::vec4>(2, offsetof(DebugChunkInstance, color), 1),
            }, 
        };
    }
};
FORWARD_DECL_ENUM_STRUCT_NS(gl, GLenum, unsigned int)
struct DebugChunkRenderer{
    static gl::GLenum PrimitiveType();
    void setup();
    void draw(Camera& cam);
    void update(Camera& cam, World& world);
    std::vector<DebugChunkInstance> instances;

private:
    void updateInstances(Camera& cam, World& world);
    VertexArray   vao{std::nullopt};
    VertexBuffer  cube_vbo{std::nullopt};
    ElementBuffer cube_ebo{std::nullopt};

    VertexBuffer  instance_vbo{std::nullopt};
    ShaderProgram prog{};
};
struct Renderer {
    Renderer() = default;
    ~Renderer() = default;

    TextureAtlas atlas{ "resources/textures/new_textures.png" };
    ChunkMesher  mesher;
    DebugChunkRenderer dbg_rend;

    void setup();
    void draw(Camera& cam);
    void draw_debugChunks(Camera& cam, World& world);
    void clear(const glm::vec4 clear_color);

    template <typename ...Args>
    inline void uploadMesh(Args&&... args) {
        visibleChunkMeshes.emplace_back(std::forward<Args>(args)...);
    }


    std::vector<Mesh> visibleChunkMeshes;

    struct {
        bool        wireframe{ false };
        bool        showChunkBoundaries{ true};  
        bool        showDebugUI{ false };  
        i64         vertex_count{ 0 };
        i64         draw_calls{ 0 };
        i64         mesh_count{ 0 };
        inline void reset_per_frame() {
            vertex_count = 0;
            draw_calls = 0;
            mesh_count = 0;
        }
        f32 blockOverlayOpacity{ 0.0 };
    } debug;

    void updateViewport(int x, int y, int w, int h);
  private:
    ShaderProgram prog{};
};
