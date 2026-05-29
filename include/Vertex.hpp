#pragma once
#include "Types.h"
#include "glmWrapper.hpp"
#include "AttributeTraits.hpp"
#include <span>
#include <string_view>
#include <type_traits>


struct Vertex {
    // start by adding new data to here.
    // Should each face have a global color uniform?
    // Maybe each face gets a blendFactor uniform, (which can be manipulated to change shadow
    // direction), and each block gets a color? Chunks could just include an extra DEBUG_COLOR field
    // which conditionally compiles or something.
    glm::vec3 pos;  // local
    glm::vec2 txCoords;
    glm::vec3 overlayColor{ 1.0f };
    i32  face_direction;
    float faceOpacity;

    static constexpr auto layout() {
        return VertexLayout<5>{ 
            .stride = sizeof(Vertex),
            .attrs = {
               make_attr<glm::vec3>(0, offsetof(Vertex, pos)),
               make_attr<glm::vec2>(1, offsetof(Vertex, txCoords)),
               make_attr<glm::vec3>(2, offsetof(Vertex, overlayColor)),
               make_attr<i32>      (3, offsetof(Vertex, face_direction)),
               make_attr<f32>      (4, offsetof(Vertex, faceOpacity)),
            }, 
        };
    }
};
static_assert(std::is_standard_layout_v<Vertex>,
              "Must be true for valid use of offsetof() in vtx attributes");
static_assert(std::is_trivially_copyable_v<Vertex>, "Must be true for upload to vertex buffer");

// each vertex has an overlay color
// global uniform float blend_BlockOverlayColor
//
