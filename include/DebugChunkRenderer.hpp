#pragma once 
#include "AttributeTraits.hpp"
#include "BufferObjects.hpp"
#include "ChunkEntry.hpp"
#include "Shaders.hpp"
#include "cppslop.hpp"
#include "Colors01.hpp"
#include "DebugOptions.hpp"

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
FORWARD_DECL_STRUCT(Engine)
FORWARD_DECL_ENUM_STRUCT_NS(gl, GLenum, unsigned int)
struct DebugChunkMesher{
    static gl::GLenum PrimitiveType();
    void setup();
    void draw(Camera& cam);
    void update(Camera& cam, Engine* sim);
    std::vector<DebugChunkInstance> instances;

    std::vector<Line3D> chunk_outlines;
private:
    void updateInstances(Camera& cam, Engine* sim);
    VertexArray   vao{LM::deferred_init};
    VertexBuffer  cube_vbo{LM::deferred_init};
    ElementBuffer cube_ebo{LM::deferred_init};

    VertexBuffer  instance_vbo{LM::deferred_init};
    ShaderProgram prog{};
};
inline glm::vec4 NeighbourDebugColor(){
    using namespace Color01;
    return GREY_50_a(64);
}

inline glm::vec4 NeighbourDebugOutlineColor(u8 opacity_255=212){
    using namespace Color01;
    return PURPLE_a(opacity_255);
}

#define state_color_match(Enum, name, col) case Enum :: name: return col(DebugOption::ChunkDebugFillOpacity); break;

inline glm::vec4 MeshDebugColor(MeshState stage){
    using namespace Color01;
    switch (stage){
        state_color_match(MeshState, awaiting_generation   , GREY_50_a)
        state_color_match(MeshState, ready_for_enqueue     , RED_a)
        state_color_match(MeshState, on_queue              , ORANGE_a)
        state_color_match(MeshState, done                  , GREEN_a)
    }
    return {};
}
inline glm::vec4 GenDebugColor(GenState stage){
    using namespace Color01;
    switch (stage){
        state_color_match(GenState, on_queue, RED_a)
        state_color_match(GenState, done, GREEN_a)
    }
    return {};
}
#undef state_color_match




inline glm::vec4 GenDebugOutlineColor(GenState state) { 
    return {glm::vec3{GenDebugColor(state)},DebugOption::ChunkDebugOutlineOpacity};
}
inline glm::vec4 MeshDebugOutlineColor(MeshState state) { 
    return {glm::vec3{MeshDebugColor(state)},DebugOption::ChunkDebugOutlineOpacity};
}



// Default states
inline glm::vec4 DefaultDebugColor(){ 
    return Color01::GREY_50_a(DebugOption::ChunkDebugFillOpacity); 
}

inline glm::vec4 DefaultDebugOutlineColor() { 
    return {glm::vec3{DefaultDebugColor()},DebugOption::ChunkDebugOutlineOpacity};
}
