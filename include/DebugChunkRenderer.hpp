#pragma once 
#include "AttributeTraits.hpp"
#include "BufferObjects.hpp"
#include "Shaders.hpp"
#include "cppslop.hpp"

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

FORWARD_DECL_STRUCT(Camera)
FORWARD_DECL_STRUCT(World)
FORWARD_DECL_STRUCT(Simulation)
FORWARD_DECL_ENUM_STRUCT_NS(gl, GLenum, unsigned int)
struct DebugChunkRenderer{
    static gl::GLenum PrimitiveType();
    void setup();
    void draw(Camera& cam);
    void update(Camera& cam, Simulation* sim);
    std::vector<DebugChunkInstance> instances;

    static constexpr bool HIDE_AIR_CHUNKS = true;
    static constexpr bool HIDE_CLEAN_CHUNKS = true;
private:
    void updateInstances(Camera& cam, Simulation* sim);
    VertexArray   vao{std::nullopt};
    VertexBuffer  cube_vbo{std::nullopt};
    ElementBuffer cube_ebo{std::nullopt};

    VertexBuffer  instance_vbo{std::nullopt};
    ShaderProgram prog{};
};
