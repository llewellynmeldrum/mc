#pragma once
#include "Types.h"
enum struct VertexAttributeType {
    u32,
    i32,
    f32,
    f64,
};
template <VertexAttributeType T>
struct vertex_attribute_type_real_type;

template <>
struct vertex_attribute_type_real_type<VertexAttributeType::f32> {
    using type = f32;
};
template <>
struct vertex_attribute_type_real_type<VertexAttributeType::f64> {
    using type = f64;
};

template <VertexAttributeType T>
using vertex_attr_t = typename vertex_attribute_type_real_type<T>::type;
