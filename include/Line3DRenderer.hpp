#pragma once 
#include "AttributeTraits.hpp"
#include "BufferObjects.hpp"
#include "Line3D.hpp"
#include "Shaders.hpp"
#include "cppslop.hpp"

struct Line3DVertex{
    glm::vec2 corner; // Encodes the side of the line and the perpendicular offset that the quad should have
    static constexpr auto layout() {
        return VertexLayout<1>{ 
            .stride = sizeof(Line3DVertex),
            .attrs = {
               make_attr<decltype(corner)>(0, offsetof(Line3DVertex, corner)),
            }, 
        };
    }
};
struct Line3DInstance{
    glm::vec3 w_start; // world pos of the chunk (i.e instance origin)
    glm::vec3 w_end; // the color of the debug overlay (i.e instance color )
    f32 thickness;
    glm::vec4 color;
    static constexpr auto layout() {
        return VertexLayout<4>{ 
            .stride = sizeof(Line3DInstance),
            .attrs = {
               make_attr<decltype(w_start)>(1, offsetof(Line3DInstance, w_start), 1),
               make_attr<decltype(w_end)>(2, offsetof(Line3DInstance, w_end), 1),
               make_attr<decltype(color)>(3, offsetof(Line3DInstance, color), 1),
               make_attr<decltype(thickness)>(4, offsetof(Line3DInstance, thickness), 1),
            }, 
        };
    }
};

FORWARD_DECL_STRUCT(Camera)
FORWARD_DECL_STRUCT(World)
FORWARD_DECL_ENUM_STRUCT_NS(gl, GLenum, unsigned int)
struct Line3DRenderer{
    static gl::GLenum PrimitiveType();
    void setup();
    void draw(Camera& cam);
    void update(Camera& cam, std::span<Line3D> lines);
    std::vector<Line3DInstance> instances;

private:
    void updateInstances(Camera& cam, std::span<Line3D> lines);
    VertexArray   vao{std::nullopt};
    VertexBuffer  quad_vbo{std::nullopt};
    ElementBuffer quad_ebo{std::nullopt};

    VertexBuffer  instance_vbo{std::nullopt};
    ShaderProgram prog{};
};
