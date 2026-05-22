#pragma once
#include "Camera.hpp"
#include "Shaders.hpp"
#include "Mesh.hpp"
#include "TextureAtlas.hpp"
#include "ChunkMesher.hpp"
#include <map>
#include <unordered_map>

#include "Chunk.hpp"
// src/Renderer.cpp
struct Renderer {
    Renderer() = default;
    ~Renderer() = default;
    TextureAtlas atlas{ "resources/textures/new_textures.png" };
    ChunkMesher  mesher;
    void setupRenderer();
void draw(Camera& cam);
    void clear(const glm::vec4 clear_color);
    void swap();
    void present();
    template <typename ...Args>
    inline void uploadMesh(Args&&... args) {
        visibleChunkMeshes.emplace_back(Mesh{std::forward<Args>(args)...});
    }
    inline void uploadEmptyMesh(WorldChunkCoord chunkCoord) {
        // does nothing right now , will do something after unify chunk meshes
    }

    void updateViewport(int x, int y, int w, int h);

    std::vector<Mesh> visibleChunkMeshes;

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
