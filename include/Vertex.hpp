#pragma once
#include <span>
#include <string_view>
#include <type_traits>
#include "Block.hpp"
#include "Types.h"
#include "glmWrapper.hpp"
#include "AttributeTraits.hpp"
#include "Assertion.hpp"


struct Vertex {
    constexpr Vertex(glm::vec3 pos, glm::vec2 txCoords, i32 face_direction, BlockShape block_shape)
        : pos(pos)
        , txCoords(txCoords)
        , face_direction(face_direction)
        , texture_atlas_id(block_shape_to_texture_atlas.at(block_shape))
    {}
    // MUST HAVE CHUNK WORLD OFFSET ADDED AT MESH TIME!
    glm::vec3 pos;          
    // MUST HAVE TEXTURE APPLIED AT MESH TIME!
    glm::vec2 txCoords;
    // (can) be set at mesh time.
    glm::vec4 overlayColor = glm::vec4(0.0f); 
    
    i32  face_direction;

    // MUST BE SET AT MESH TIME!
    float faceOpacity{0.0f};

    i32 texture_atlas_id;

    static constexpr auto layout() {
        return VertexLayout<6>{ 
            .stride = sizeof(Vertex),
            .attrs = {
               make_attr<glm::vec3>(0, offsetof(Vertex, pos)),                  // 12 bytes (4*3)
               make_attr<glm::vec2>(1, offsetof(Vertex, txCoords)),             // 08 bytes (4*2)
               make_attr<glm::vec4>(2, offsetof(Vertex, overlayColor)),         // 16 bytes (4*4)
               make_attr<i32>      (3, offsetof(Vertex, face_direction)),       // 4 bytes ()
               make_attr<f32>      (4, offsetof(Vertex, faceOpacity)),
               make_attr<i32>      (5, offsetof(Vertex, texture_atlas_id)),
            }, 
        };
    }
};
static_assert(std::is_standard_layout_v<Vertex>,
              "Must be true for valid use of offsetof() in vtx attributes");
static_assert(std::is_trivially_copyable_v<Vertex>, "Must be true for upload to vertex buffer");
