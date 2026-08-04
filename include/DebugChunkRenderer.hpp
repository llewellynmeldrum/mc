#pragma once 
#include "AttributeTraits.hpp"
#include "BufferObjects.hpp"
#include "ChunkEntry.hpp"
#include "Shaders.hpp"
#include "VertexLayoutHelpers.hpp"
#include "cppslop.hpp"
#include "Colors01.hpp"
#include "DebugOptions.hpp"

struct DebugChunkVertex{
    glm::ivec3 pos; // Chunk local vertex position
};
struct DebugChunkInstance{
    glm::ivec3 origin; // world pos of the chunk (i.e instance origin)
    glm::vec4 color; // the color of the debug overlay (i.e instance color )
    static constexpr auto instanced_layout() {
        return VertexLayout<2>{ 
            .stride = sizeof(DebugChunkInstance),
            .attrs = {
               make_attr_t<glm::ivec3, 1, offsetof(DebugChunkInstance, origin)>(1),
               make_attr_t<glm::vec4, 2, offsetof(DebugChunkInstance, color)>(1),
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

#define state_color_match(Enum, name, col) case Enum :: name: return col(DebugOption::ChunkDebugFillOpacity); break;
inline glm::vec4 PipelineStateColor(PipelineState stage){
    using namespace Color01;
    switch (stage){
        state_color_match(PipelineState, pending , GREY_50_a)
        state_color_match(PipelineState, ready_for_enqueue     , RED_a)
        state_color_match(PipelineState, on_queue              , ORANGE_a)
        state_color_match(PipelineState, done                  , GREEN_a)
    }
    return {};
}
#undef state_color_match

inline glm::vec4 PipelineStateOutlineColor(PipelineState state) { 
    return {glm::vec3{PipelineStateColor(state)},DebugOption::ChunkDebugOutlineOpacity};
}



inline glm::vec4 DefaultDebugColor(){ 
    return PipelineStateColor(PipelineState::pending);
}

inline glm::vec4 DefaultDebugOutlineColor() { 
    return PipelineStateOutlineColor(PipelineState::pending);
}
