#pragma once
#include <map>
#include <optional>
#include <unordered_map>
#include <string_view>

#include "Camera.hpp"
#include "DebugOptions.hpp"
#include "CoordTypes.hpp"
#include "Line3DRenderer.hpp"
#include "Shaders.hpp"
#include "Mesh.hpp"
#include "SlotMap.hpp"
#include "TextureAtlas.hpp"
#include "ChunkMesher.hpp"
#include "DebugChunkRenderer.hpp"
#include "RenderTargets.hpp"

#include "Chunk.hpp"
FORWARD_DECL_STRUCT(Engine)
FORWARD_DECL_TEMPLATE_STRUCT(BenchmarkMap, typename K, bool ordered)
using FrameProfiler = BenchmarkMap<std::string_view,true>;

struct Renderer {
    Renderer();
    ~Renderer() = default;

    TextureAtlas cube_atlas;
    TextureAtlas cross_atlas;
    TextureAtlas cactus_atlas;
    TextureAtlas half_slab_atlas;
    std::vector <TextureAtlas*> atlas_list;
    ChunkMesher  meshers;
    // these arent really 'renderers' but more like 'render devices' which do a certain thing. poor naming
    DebugChunkMesher dbg_rend;
    Line3DRenderer line3d_rend;
    std::vector<Line3D> player_cam_frustum_lines;
    glm::vec4 clear_color = {0.25, 0.5, 0.85, 1.0};

    slot_map<WorldChunkCoord,Mesh> opaque_chunk_meshes;
    std::vector<WorldChunkCoord> sorted_opaque_coords;

    slot_map<WorldChunkCoord, Mesh> blended_chunk_meshes;
    std::vector<WorldChunkCoord> sorted_blended_coords;

    slot_map<WorldChunkCoord, Mesh> cutout_chunk_meshes;
    std::vector<WorldChunkCoord> sorted_cutout_coords;

    i32 cutout_enable_radius {0};
    // UNIFORMS
    i32 u_textures_loc{};
    i32 u_enable_cutout_loc{};
    i32 u_model_loc{};
    i32 u_proj_loc{};
    i32 u_view_loc{};

    void sort_opaque_chunks(WorldFloatPos cam_pos);
    void sort_blended_chunks(WorldFloatPos cam_pos);
    void sort_cutout_chunks(WorldFloatPos cam_pos);

    void draw_to(Camera& cam, RenderTargetView target, FrameProfiler* prof);

    void prepare_blended_pass();
    void prepare_opaque_pass();
    void prepare_cutout_pass();

    void draw_opaque_pass(Camera& cam);
    void draw_blended_pass(Camera& cam);
    void draw_cutout_pass(Camera& cam);

    void draw_debugChunks_to(Camera&cam, Engine* sim, RenderTargetView target);
    void draw_3DLines_to(Camera& cam, std::span<Line3D> lines, RenderTargetView target);
    void clear(const glm::vec4 clear_color);
    void clear_to(RenderTargetView target);

    void update_player_cam_frustum_lines(Engine* sim);
    inline void uploadMesh(WorldChunkCoord coord, OpaqueMeshData mesh_data) {
        opaque_chunk_meshes.emplace_or_assign(coord,coord, std::move(mesh_data.vertices),std::move(mesh_data.indices));
//        sorted_opaque_coords.emplace_back(coord);
    }

    inline void uploadMesh(WorldChunkCoord coord, BlendedMeshData mesh_data) {
        blended_chunk_meshes.emplace_or_assign(coord,coord, std::move(mesh_data.vertices),std::move(mesh_data.indices));
        sorted_blended_coords.emplace_back(coord);
    }
    inline void uploadMesh(WorldChunkCoord coord, CutoutMeshData mesh_data) {
        cutout_chunk_meshes.emplace_or_assign(
            coord, coord,
            std::move(mesh_data.vertices),std::move(mesh_data.indices),
            true
        );
//        sorted_cutout_coords.emplace_back(coord);
    }


    // overload for unsorted meshes
    void draw_cutout_meshes_unsorted(const slot_map<WorldChunkCoord,Mesh>& meshList);
    void draw_meshes_unsorted(const slot_map<WorldChunkCoord,Mesh>& meshList);

    void draw_mesh(const Mesh& mesh);
    void draw_cutout_mesh(const Mesh& mesh);


    // overload for sorted meshes
    void draw_meshes(const slot_map<WorldChunkCoord,Mesh>& meshList, std::span<WorldChunkCoord>);

    // TODO: move to DebugOption namespace
    struct {
        bool        wireframe{ false };
        i64         vertex_count{ 0 };
        i64         draw_calls{ 0 };
        i64         mesh_count{ 0 };
        void reset_per_frame() noexcept{
            vertex_count = 0;
            draw_calls = 0;
            mesh_count = 0;
        }
        f32 blockOverlayOpacity{ 0.0 };
    } debug;

    void updateViewport(int x, int y, int w, int h);
  private:
    void enableBackfaceCulling();
    void disableBackfaceCulling();
    void enableDepthTesting();
    void disableDepthTesting();
    ShaderProgram prog{};
};
