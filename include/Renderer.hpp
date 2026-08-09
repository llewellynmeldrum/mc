#pragma once
#include <map>
#include <optional>
#include <unordered_map>
#include <string_view>

#include "Camera.hpp"
#include "ChunkConcurrency.hpp"
#include "DebugOptions.hpp"
#include "CoordTypes.hpp"
#include "Line3DRenderer.hpp"
#include "Shaders.hpp"
#include "Mesh.hpp"
#include "SlotMap.hpp"
#include "TextureAtlas.hpp"
#include "DebugChunkRenderer.hpp"
#include "RenderTargets.hpp"
#include "Skybox.hpp"

#include "Chunk.hpp"
FORWARD_DECL_STRUCT(Engine)
FORWARD_DECL_TEMPLATE_STRUCT(BenchmarkMap, typename K, bool ordered)
using FrameProfiler = BenchmarkMap<std::string_view,true>;

struct Renderer {
    Renderer();
    ~Renderer() = default;

    SkyboxRenderer skybox{{0.25,0.5,0.85}};
    TextureAtlas cube_atlas;
    TextureAtlas cross_atlas;
    TextureAtlas cactus_atlas;
    TextureAtlas half_slab_atlas;
    void set_global_sun_color(glm::vec3 col){
        prog.setUniform(u_sunlight_rgb_loc, col);
    }
    void set_global_sun_intensity(f32 scale){
        prog.use();
        prog.setUniform(u_global_sun_intensity_scale_loc, scale);
    }
    void set_fog_color(glm::vec3 fog_color){
        prog.use();
        prog.setUniform(u_fog_color_loc, fog_color);
    }
    void set_fog_start(float world_fog_start){
        prog.use();
        prog.setUniform(u_world_fog_start_loc, world_fog_start);
    }
    void set_fog_end(float world_fog_end){
        prog.use();
        prog.setUniform(u_world_fog_end_loc, world_fog_end);
    }
    std::vector <TextureAtlas*> atlas_list;
    // these arent really 'renderers' but more like 'render devices' which do a certain thing. poor naming
    DebugChunkMesher dbg_rend;
    Line3DRenderer line3d_rend;
    std::vector<Line3D> player_cam_frustum_lines;

    slot_map<WorldChunkCoord,Mesh> opaque_chunk_meshes;
    std::vector<WorldChunkCoord> sorted_opaque_coords;

    slot_map<WorldChunkCoord, Mesh> blended_chunk_meshes;
    std::vector<WorldChunkCoord> sorted_blended_coords;

    slot_map<WorldChunkCoord, Mesh> cutout_chunk_meshes;
    std::vector<WorldChunkCoord> sorted_cutout_coords;

    i32 cutout_enable_radius {0};
    // SECTION: UNIFORMS
    i32 u_texture_atlases_loc{};
    i32 u_enable_cutout_loc{};
    i32 u_model_loc{};
    i32 u_chunk_opacity_loc{};
    i32 u_fog_color_loc{};
    i32 u_global_sun_intensity_scale_loc{};
    i32 u_world_fog_start_loc{};
    i32 u_world_fog_end_loc{};
    i32 u_proj_loc{};
    i32 u_view_loc{};
    i32 u_sunlight_rgb_loc{};


    bool enable_smooth_light_falloff = true;
    f32 blocklight_smooth_falloff_factor = 0.840f;
    f32 gamma = 0.8f;
    f32 sunlight_smooth_falloff_factor = 0.150f;

    void per_frame_update(Camera const& player_cam, SkyboxState const& skybox_state);
    void update_debug_uniforms();
    void sort_opaque_chunks(WorldFloatPos cam_pos);
    void sort_blended_chunks(WorldFloatPos cam_pos);
    void sort_cutout_chunks(WorldFloatPos cam_pos);

    void draw_to(Camera& cam, RenderTargetView target, FrameProfiler* prof);

    void prepare_blended_pass();
    void prepare_skybox_pass();
    void prepare_opaque_pass();
    void prepare_cutout_pass();

    void draw_opaque_pass(Camera& cam);
    void draw_blended_pass(Camera& cam);
    void draw_cutout_pass(Camera& cam);

    void draw_debugChunks_to(Camera&cam, Engine* sim, RenderTargetView target);
    void draw_3DLines_to(Camera& cam, std::span<Line3D> lines, RenderTargetView target);
    void clear(const glm::vec4 clear_color);
    void draw_skybox(RenderTargetView target);

    void update_player_cam_frustum_lines(Engine* sim);
    template<typename T>
    inline void upload_mesh(WorldChunkCoord key, T&& mesh_data, bool is_first_upload, bool is_cutout) {
        auto* cont = &opaque_chunk_meshes;
        if constexpr(std::same_as<T,OpaqueMeshData>){
            cont = &opaque_chunk_meshes;
        }else if constexpr(std::same_as<T,BlendedMeshData>){
            cont = &blended_chunk_meshes;
        }else if  constexpr(std::same_as<T,CutoutMeshData>){
            cont = &cutout_chunk_meshes;
        }else{
            static_assert(false);
        }
        cont->insert_or_assign(key,Mesh{key,mesh_data.vertices,mesh_data.indices,is_cutout});
    }
    inline void uploadMesh(WorldChunkCoord coord, OpaqueMeshData&& mesh_data, bool is_first_upload) {
        constexpr static bool is_cutout = false;
        //upload_mesh(coord,std::move(mesh_data),is_first_upload,is_cutout);
        opaque_chunk_meshes.emplace_or_assign(
            coord,
            coord,
            std::move(mesh_data.vertices),
            std::move(mesh_data.indices),
            is_cutout,
            is_first_upload
        );
    }

    inline void uploadMesh(WorldChunkCoord coord, BlendedMeshData&& mesh_data, bool is_first_upload) {
        constexpr static bool is_cutout = false;
        //upload_mesh(coord,std::move(mesh_data),is_first_upload,is_cutout);
        blended_chunk_meshes.emplace_or_assign(
            coord,
            coord,
            std::move(mesh_data.vertices),
            std::move(mesh_data.indices),
            is_cutout,
            is_first_upload
        );
        sorted_blended_coords.emplace_back(coord);
    }
    inline void uploadMesh(WorldChunkCoord coord, CutoutMeshData&& mesh_data, bool is_first_upload) {
        constexpr static bool is_cutout = true;
        cutout_chunk_meshes.emplace_or_assign(
            coord, coord,
            std::move(mesh_data.vertices),
            std::move(mesh_data.indices),
            is_cutout,
            is_first_upload
        );
//        sorted_cutout_coords.emplace_back(coord);
    }


    // overload for unsorted meshes
    void draw_cutout_meshes_unsorted(const slot_map<WorldChunkCoord,Mesh>& meshList);
    void draw_meshes_unsorted(const slot_map<WorldChunkCoord,Mesh>& meshList);

    void draw_mesh(const Mesh& mesh);


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
