#pragma once
#include "Types.h"
#include <glbinding/gl/enum.h>

// SECTION: 
// Gl enum trait
template <typename T>
struct gl_enum_trait;

template <>
struct gl_enum_trait<f32> {
    static constexpr gl::GLenum value = gl::GL_FLOAT;
};

template <>
struct gl_enum_trait<f64> {
    static constexpr gl::GLenum value = gl::GL_DOUBLE;
};
template <>
struct gl_enum_trait<i32> {
    static constexpr gl::GLenum value = gl::GL_INT;
};
template <>
struct gl_enum_trait<u32> {
    static constexpr gl::GLenum value = gl::GL_UNSIGNED_INT;
};
template <>
struct gl_enum_trait<u8> {
    static constexpr gl::GLenum value = gl::GL_UNSIGNED_BYTE;
};
template <>
struct gl_enum_trait<i8> {
    static constexpr gl::GLenum value = gl::GL_BYTE;
};
template<typename T>
using gl_enum_trait_t = gl_enum_trait<T>::value;

