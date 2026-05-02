#pragma once
#include "Types.h"
#include <glbinding/gl/enum.h>
template <typename T>
struct gl_type_traits;

template <>
struct gl_type_traits<f32> {
    static constexpr gl::GLenum value = gl::GL_FLOAT;
};

template <>
struct gl_type_traits<f64> {
    static constexpr gl::GLenum value = gl::GL_DOUBLE;
};
template <>
struct gl_type_traits<i32> {
    static constexpr gl::GLenum value = gl::GL_INT;
};
template <>
struct gl_type_traits<u32> {
    static constexpr gl::GLenum value = gl::GL_UNSIGNED_INT;
};
template <>
struct gl_type_traits<u8> {
    static constexpr gl::GLenum value = gl::GL_UNSIGNED_BYTE;
};
template <>
struct gl_type_traits<i8> {
    static constexpr gl::GLenum value = gl::GL_BYTE;
};
