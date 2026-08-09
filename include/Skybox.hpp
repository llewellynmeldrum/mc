#pragma once 
#include <array>
#include "BufferObjects.hpp"
#include "SkyboxState.hpp"
#include "LM.hpp"
#include "RenderTargets.hpp"
#include "Shaders.hpp"
#include "Types.hpp"
#include "cppslop.hpp"
#include "glbinding/gl/enum.h"


FORWARD_DECL_STRUCT(Camera)
struct SkyboxRenderer{
    static constexpr std::array fullscreen_quad_corners_ndc{
        glm::vec2{-1,-1},
        glm::vec2{+1,-1},
        glm::vec2{+1,+1},
        glm::vec2{-1,+1},
    };
    struct Vertex{
        glm::vec2 pos;
        glm::vec3 ray;
    };

    glm::vec3 base_color;
    std::array<Vertex,fullscreen_quad_corners_ndc.size()> rays{};

    void upload_rays();
    VertexArray vao{LM::deferred_init};
    VertexBuffer vbo{LM::deferred_init};
    ElementBuffer ebo{LM::deferred_init};
    ShaderProgram prog;
    i32 u_base_color_loc;

    i32 u_color_horizon_loc;     // color of the sky at the horizon line
    i32 u_color_sun_loc;
    i32 u_color_sunlight_loc;
    i32 u_color_sun_glow_loc;
    i32 u_belt_of_venus_intensity_loc{};
    i32 u_horizon_hug_intensity_loc{};
    i32 u_color_outer_sun_glow_loc;
    i32 u_color_zenith_loc;      // color of the sky at the highest point (top center)
    i32 u_dir_moon_loc;
    i32 u_dir_sun_loc;
    i32 u_sun_intensity_scale_loc;
    i32 u_star_alpha_loc;
    i32 u_color_lowsky_loc;      // color of the sky ~y=0.3
    i32 u_color_midsky_loc;      // color of the sky ~y=0.5
    i32 u_glow_scale_loc;
    SkyboxState current_state;
    void update_uniforms(SkyboxState const& state)noexcept;

    constexpr gl::GLenum PrimitiveType();


    void update_rays(Camera const& cam);
    void setup();
    void per_frame_update(Camera const& cam, SkyboxState const& state);
    void draw();
};

