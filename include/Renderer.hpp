#pragma once 
#include "Shaders.hpp"
#include "Mesh.hpp"
#include "TextureAtlas.hpp"
#include "ChunkMesher.hpp"
#include <unordered_map>

#include "Chunk.hpp"
// src/Renderer.cpp
struct Renderer{
    Renderer() = default;
    ~Renderer() = default;
    void setupRenderer();
    void draw(const mat4& view, const mat4& proj);
    void clear(const vec4 clear_color);
    void swap();
    void present();

    void updateViewport(int x, int y, int w, int h);
    std::unordered_map<ChunkCoordinate, Mesh, ChunkCoordHasher> visibleChunkMeshes;
    
    struct {
        bool wireframe{false};
        bool showChunkBoundaries{true};
        u64 vertex_count{0};
        u64 draw_calls{0};
        u64 mesh_count{0};
        void reset_per_frame(){
            vertex_count=0;
            draw_calls=0;
            mesh_count=0;
        }
    }debug;
    TextureAtlas atlas{"resources/textures/test.png"};
    ChunkMesher mesher;
private:
    ShaderProgram prog{};

};
