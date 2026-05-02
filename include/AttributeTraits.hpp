#pragma once
#include "Types.h"

#include "glmWrapper.hpp"

#include "VertexAttributeType.hpp"
template <typename T>
struct attribute_traits;

template <>
struct attribute_traits<f64> {
    using value_type = f64;
    static constexpr i32  count = 1;
    static constexpr auto vertex_attribute_type = VertexAttributeType::f64;
    static constexpr bool is_integer = false;
    static constexpr bool normalized = false;
};

template <>
struct attribute_traits<f32> {
    using value_type = f32;
    static constexpr i32  count = 1;
    static constexpr auto vertex_attribute_type = VertexAttributeType::f32;
    static constexpr bool is_integer = false;
    static constexpr bool normalized = false;
};

template <>
struct attribute_traits<vec2> {
    using value_type = f32;
    static constexpr i32  count = 2;
    static constexpr auto vertex_attribute_type = VertexAttributeType::f32;
    static constexpr bool is_integer = false;
    static constexpr bool normalized = false;
};
template <>
struct attribute_traits<vec3> {
    using value_type = f32;
    static constexpr i32  count = 3;
    static constexpr auto vertex_attribute_type = VertexAttributeType::f32;
    static constexpr bool is_integer = false;
    static constexpr bool normalized = false;
};
template <>
struct attribute_traits<vec4> {
    using value_type = f32;
    static constexpr i32  count = 4;
    static constexpr auto vertex_attribute_type = VertexAttributeType::f32;
    static constexpr bool is_integer = false;
    static constexpr bool normalized = false;
};

template <>
struct attribute_traits<i32> {
    using value_type = i32;
    static constexpr i32  count = 1;
    static constexpr auto vertex_attribute_type = VertexAttributeType::i32;
    static constexpr bool is_integer = true;
    static constexpr bool normalized = false;
};
