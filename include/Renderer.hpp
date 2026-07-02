#pragma once
#include "Camera.hpp"
#include "CoordTypes.hpp"
#include "Line3DRenderer.hpp"
#include "Shaders.hpp"
#include "Mesh.hpp"
#include "SlotMap.hpp"
#include "TextureAtlas.hpp"
#include "ChunkMesher.hpp"
#include <map>
#include <optional>
#include <unordered_map>
#include "DebugChunkRenderer.hpp"
#include "RenderTargets.hpp"

#include "Chunk.hpp"
FORWARD_DECL_STRUCT(Engine)
struct Renderer {
    Renderer();
    ~Renderer() = default;

    TextureAtlas atlas{ "resources/textures/new_textures.png" };
    ChunkMesher  meshers;
    // these arent really 'renderers' but more like 'render devices' which do a certain thing. poor naming
    DebugChunkMesher dbg_rend;
    Line3DRenderer line3d_rend;
    std::vector<Line3D> player_cam_frustum_lines;
    glm::vec4 clear_color = {0.25, 0.5, 0.85, 1.0};

    slot_map<WorldChunkCoord,Mesh> opaqueChunkMeshes;
    slot_map<WorldChunkCoord, Mesh> transparentChunkMeshes;

    std::vector<WorldChunkCoord> sorted_opaque(Camera& cam);
    std::vector<WorldChunkCoord> sorted_transparent(Camera& cam);
    void draw_to(Camera& cam, RenderTargetView target);
    void drawOpaque(Camera& cam);
    void drawTransparent(Camera& cam);
    void draw_debugChunks_to(Camera&cam, Engine* sim, RenderTargetView target);
    void draw_3DLines_to(Camera& cam, std::span<Line3D> lines, RenderTargetView target);
    void clear(const glm::vec4 clear_color);
    void clear_to(RenderTargetView target);

    void update_player_cam_frustum_lines(Engine* sim);
    inline void uploadMesh(WorldChunkCoord coord, OpaqueMeshData mesh_data) {
        opaqueChunkMeshes.emplace_or_assign(coord,coord, std::move(mesh_data.vertices),std::move(mesh_data.indices));
    }

    inline void uploadMesh(WorldChunkCoord coord, TransparentMeshData mesh_data) {
        transparentChunkMeshes.emplace_or_assign(coord,coord, std::move(mesh_data.vertices),std::move(mesh_data.indices));
    }


    void uploadMeshListToGpu(const slot_map<WorldChunkCoord,Mesh>& meshList, std::span<WorldChunkCoord>);

    struct {
        bool        wireframe{ false };
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
