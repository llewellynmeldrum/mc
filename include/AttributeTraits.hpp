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
struct attribute_traits<glm::vec2> {
    using value_type = f32;
    static constexpr i32  count = 2;
    static constexpr auto vertex_attribute_type = VertexAttributeType::f32;
    static constexpr bool is_integer = false;
    static constexpr bool normalized = false;
};
template <>
struct attribute_traits<glm::vec3> {
    using value_type = f32;
    static constexpr i32  count = 3;
    static constexpr auto vertex_attribute_type = VertexAttributeType::f32;
    static constexpr bool is_integer = false;
    static constexpr bool normalized = false;
};
template <>
struct attribute_traits<glm::ivec3> {
    using value_type = i32;
    static constexpr i32  count = 3;
    static constexpr auto vertex_attribute_type = VertexAttributeType::i32;
    static constexpr bool is_integer = true;
    static constexpr bool normalized = false;
};
template <>
struct attribute_traits<glm::vec4> {
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

struct VertexAttribute {
    u32         location;
    i32         count;
    const void* offset_ptr;
    bool        normalized;
    bool        is_integer;

    VertexAttributeType vat;
    u32 divisor{0}; // for instancing
};

template <typename T>
constexpr VertexAttribute make_attr(u32 _location, std::intptr_t offset, u32 _divisor=0) {
    return VertexAttribute{
        .location = _location,
        .count = attribute_traits<T>::count,
        .offset_ptr = reinterpret_cast<void*>(offset),
        .normalized = attribute_traits<T>::normalized,
        .is_integer = attribute_traits<T>::is_integer,
        .vat = attribute_traits<T>::vertex_attribute_type,
        .divisor = _divisor,
    };
}

template <std::size_t SZ>
struct VertexLayout {
    i32                                   stride;
    const std::array<VertexAttribute, SZ> attrs;
};
