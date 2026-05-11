#pragma once
#include "Shaders.hpp"
#include "Mesh.hpp"
#include "TextureAtlas.hpp"
#include "ChunkMesher.hpp"
#include <unordered_map>

#include "Chunk.hpp"
// src/Renderer.cpp
struct Renderer {
    Renderer() = default;
    ~Renderer() = default;
    TextureAtlas atlas{ "resources/textures/new_textures.png" };
    ChunkMesher  mesher;
    void setupRenderer();
    void draw(const mat4& view, const mat4& proj);
    void clear(const vec4 clear_color);
    void swap();
    void present();
    template <typename ...Args>
    void uploadMesh(vec3 chunk_pos, Args&&... args) {
        visibleChunkMeshes.insert_or_assign(chunk_pos, Mesh{std::forward<Args>(args)...});
    }

    void updateViewport(int x, int y, int w, int h);

    std::unordered_map<ivec3, Mesh> visibleChunkMeshes;

    struct {
        bool        wireframe{ false };
        bool        showChunkBoundaries{ false };  // TODO: IMPLEMENT
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

  private:
    ShaderProgram prog{};
};
