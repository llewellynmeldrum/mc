#pragma once
#include "Types.h"
#include "glmWrapper.hpp"
struct [[gnu::packed]] Vertex {
    // start by adding new data to here.
    // Should each face have a global color uniform?
    // Maybe each face gets a blendFactor uniform, (which can be manipulated to change shadow
    // direction), and each block gets a color? Chunks could just include an extra DEBUG_COLOR field
    // which conditionally compiles or something.
    vec3 pos;  // local
    vec2 txCoords;
    vec3 overlayColor{ 1.0f };
    i32  face_direction;
};

// each vertex has an overlay color
// global uniform float blend_BlockOverlayColor
//
