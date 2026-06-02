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
#include "DebugChunkRenderer.hpp"

#include "Chunk.hpp"
struct DepthBuffer{
    u32 id;
    DepthBuffer()=delete;

    DepthBuffer(glm::vec2 size);
    ~DepthBuffer();
    void bind();
    void unbind();

};
struct FrameBuffer{
    u32 id;

    void attach_depth_buffer(DepthBuffer& depth_buffer);
    void attach_texture2d(Texture2D& texture2d,i32 idx=0);
    FrameBuffer();
    ~FrameBuffer();
    void bind();
    void unbind();

};
enum struct RenderTargetType{
    Default,
    Texture,
};
struct RenderTargetView{
    glm::ivec2 pos;
    glm::ivec2 size;
    u32 framebuffer_id{0}; // 0 = screen
    bool isOverlay{false};
    void use();
    void stop();
private:
    void bind();
    void unbind();
};

struct ScreenTarget{
    glm::ivec2 pos;
    glm::ivec2 size;
    inline RenderTargetView view(){
        return {
            .pos = pos,
            .size = size,
            .framebuffer_id=0,
            .isOverlay=true,
        };
    }
};
struct TextureTarget{
    TextureTarget(glm::vec2 size, glm::vec2 pos);
    TextureTarget();
    ~TextureTarget()=default;

    Texture2D texture;
    FrameBuffer framebuffer;
    DepthBuffer depthbuffer;
    glm::vec2 pos;
    glm::vec2 size;

    inline RenderTargetView view(){
        return {
            .pos = pos,
            .size = size,
            .framebuffer_id=framebuffer.id,
        };
    }
};
struct Renderer {
    Renderer();
    ~Renderer() = default;

    TextureAtlas atlas{ "resources/textures/new_textures.png" };
    ChunkMesher  meshers;
    DebugChunkRenderer dbg_rend;
    glm::vec4 clear_color = {0.25, 0.5, 0.85, 1.0};

    void draw_to(Camera& cam, RenderTargetView target);
    void drawOpaque(Camera& cam);
    void drawTransparent(Camera& cam);
    void draw_debugChunks(Camera& cam, World& world);
    void clear(const glm::vec4 clear_color);

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
    ShaderProgram prog{};
};
