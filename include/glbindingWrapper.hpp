#pragma once
#include "Types.h"
#define GL_SILENCE_DEPRECATION
#include <glbinding/gl/gl.h>  // All OpenGL symbols (functions, enums, etc.)
// gl/gl.h includes:
/*
    #include <glbinding/gl/bitfield.h>  // bitfield constants (e.g., GL_COLOR_BUFFER_BIT)
    #include <glbinding/gl/boolean.h>   // boolean constants (GL_TRUE and GL_FALSE)
    #include <glbinding/gl/enum.h>      // symbol constants (e.g., GL_VERTEX_SHADER)
    #include <glbinding/gl/functions.h> // functions
    #include <glbinding/gl/types.h>     // type declarations of the OpenGL API (including bitfields,
   boolean, enum, and extensions) #include <glbinding/gl/values.h>    // special values (e.g.,
   GL_INVALID_INDEX)
*/
#include <glbinding/glbinding.h>  // glbinding initialization and management

constexpr static inline gl::GLenum to_glenum(u32 a) {
    return static_cast<gl::GLenum>(a);
}
constexpr static inline u32 to_u32(gl::GLenum a) {
    return static_cast<u32>(a);
}
constexpr static inline i32 to_i32(gl::GLenum a) {
    return static_cast<i32>(a);
}

#include "AttributeTraits.hpp"

template <typename T>
constexpr gl::GLenum gl_type() {
}
template <typename T>
constexpr gl::GLenum gl_type_from(T val);

#include "glTypeTraits.hpp"

template <>
constexpr gl::GLenum gl_type_from<VertexAttributeType>(VertexAttributeType vat) {

    int i = 5124;
    switch (vat) {
    case VertexAttributeType::u32:
        return gl_type_traits<u32>::value;
        break;
    case VertexAttributeType::i32:
        return gl_type_traits<i32>::value;
        break;
    case VertexAttributeType::f32:
        return gl_type_traits<f32>::value;
        break;
    case VertexAttributeType::f64:
        return gl_type_traits<f64>::value;
        break;
    };
}
#define _GLFW_INCLUDE_WARNING
