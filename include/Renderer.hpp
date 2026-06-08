#pragma once
#include "Camera.hpp"
#include "CoordTypes.hpp"
#include "Line3DRenderer.hpp"
#include "Shaders.hpp"
#include "Mesh.hpp"
#include "TextureAtlas.hpp"
#include "ChunkMesher.hpp"
#include <map>
#include <optional>
#include <unordered_map>
#include "DebugChunkRenderer.hpp"
#include "RenderTargets.hpp"

#include "Chunk.hpp"
FORWARD_DECL_STRUCT(Simulation)
struct Renderer {
    Renderer();
    ~Renderer() = default;

    TextureAtlas atlas{ "resources/textures/new_textures.png" };
    ChunkMesher  meshers;
    DebugChunkRenderer dbg_rend;
    Line3DRenderer line3d_rend;
    glm::vec4 clear_color = {0.25, 0.5, 0.85, 1.0};

    void draw_to(Camera& cam, RenderTargetView target);
    void drawOpaque(Camera& cam);
    void drawTransparent(Camera& cam);
    void draw_debugChunks(Camera&cam, Simulation* sim, RenderTargetView target);
    void draw_3DLines_to(Camera& cam, std::span<Line3D> lines, RenderTargetView target);
    void clear(const glm::vec4 clear_color);
    void clear_to(RenderTargetView target);

    inline void uploadMesh(WorldChunkCoord coord, OpaqueMeshData mesh_data) {
        opaqueChunkMeshes.emplace_back(coord, std::move(mesh_data.vertices),std::move(mesh_data.indices));
    }

    inline void uploadMesh(WorldChunkCoord coord, TransparentMeshData mesh_data) {
        transparentChunkMeshes.emplace_back(coord, std::move(mesh_data.vertices),std::move(mesh_data.indices));
    }


    std::vector<Mesh> opaqueChunkMeshes;
    std::vector<Mesh> transparentChunkMeshes;

    struct {
        bool        wireframe{ false };
        bool        showChunkBoundaries{ false};  
        bool        showDebugUI{ true};  
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
    void beginTransparentPass();
    void beginOpaquePass();
    void enableBackfaceCulling();
    void disableBackfaceCulling();
    void enableDepthTesting();
    void disableDepthTesting();
    ShaderProgram prog{};
};
