#pragma once 
#include "Shaders.hpp"
#include "Mesh.hpp"
#include "TextureAtlas.hpp"
#include "ChunkMesher.hpp"

struct Renderer{
    Renderer() = default;
    ~Renderer() = default;
    void setupRenderer();
    void draw(const mat4& view, const mat4& proj);
    void swap();
    void present();

    void updateViewport(int x, int y, int w, int h);
    ShaderProgram prog{};
    TextureAtlas atlas{"resources/textures/test.png"};
    ChunkMesher mesher;
    std::vector<Mesh> chunkMeshes; // simple debug version

    struct {
        bool wireframe{false};
        bool showChunkBoundaries{true};
    }debug;
};
