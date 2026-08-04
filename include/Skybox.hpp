#pragma once 
#include <array>
#include "BufferObjects.hpp"
#include "LM.hpp"
#include "RenderTargets.hpp"
#include "Shaders.hpp"
#include "Types.hpp"
#include "cppslop.hpp"
#include "glbinding/gl/enum.h"


FORWARD_DECL_STRUCT(Camera)
struct SkyboxRenderer{
    static constexpr std::array fullscreen_quad_corners{
        glm::vec2{-1,-1},
        glm::vec2{+1,-1},
        glm::vec2{+1,+1},
        glm::vec2{-1,+1},
    };
    static constexpr std::array<size_t,6> fullscreen_quad_indices{
        // bottom right tri
        0, 1, 2,
        // top left tri
        2, 3, 0
    };
    struct Vertex{
        glm::vec2 pos;
        glm::vec3 ray;
    };

    glm::vec3 base_color;
    std::array<Vertex,fullscreen_quad_corners.size()> rays{};

    VertexArray vao{LM::deferred_init};
    VertexBuffer vbo{LM::deferred_init};
    ShaderProgram prog;
    i32 u_base_color_loc;
    constexpr gl::GLenum PrimitiveType();


    void update_rays(Camera const& cam);
    void setup();
    void per_tick_update(Camera const& cam);
    void draw();
};

