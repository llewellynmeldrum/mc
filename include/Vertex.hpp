#pragma once
#include "Types.h"
#include "glmWrapper.hpp"
#include "AttributeTraits.hpp"
#include <span>
#include <string_view>
#include <type_traits>

struct VertexAttribute {
    u32         location;
    i32         count;
    const void* offset_ptr;
    bool        normalized;
    bool        is_integer;

    VertexAttributeType vat;
};

template <typename T>
constexpr VertexAttribute make_attr(u32 location, std::intptr_t offset) {
    using value_type = attribute_traits<T>::value_type;

    // cast to u32 to reduce use of GLenum in headers
    return VertexAttribute{
        .location = location,
        .count = attribute_traits<T>::count,
        .offset_ptr = reinterpret_cast<void*>(offset),
        .normalized = attribute_traits<T>::normalized,
        .is_integer = attribute_traits<T>::is_integer,
        .vat = attribute_traits<T>::vertex_attribute_type,
    };
}

template <std::size_t SZ>
struct VertexLayout {
    i32                                   stride;
    const std::array<VertexAttribute, SZ> attrs;
};

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

    static constexpr auto layout() {
        return VertexLayout<4>{ 
            .stride = sizeof(Vertex),
            .attrs = {
               make_attr<glm::vec3>(0, offsetof(Vertex, pos)),
               make_attr<glm::vec2>(1, offsetof(Vertex, txCoords)),
               make_attr<glm::vec3>(2, offsetof(Vertex, overlayColor)),
               make_attr<i32>(3, offsetof(Vertex, face_direction)),
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
