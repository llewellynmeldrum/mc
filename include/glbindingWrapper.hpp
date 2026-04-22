#pragma once 
#define GL_SILENCE_DEPRECATION
#include <glbinding/gl/gl.h>       // All OpenGL symbols (functions, enums, etc.)
// gl/gl.h includes:
/*
    #include <glbinding/gl/bitfield.h>  // bitfield constants (e.g., GL_COLOR_BUFFER_BIT)
    #include <glbinding/gl/boolean.h>   // boolean constants (GL_TRUE and GL_FALSE)
    #include <glbinding/gl/enum.h>      // symbol constants (e.g., GL_VERTEX_SHADER)
    #include <glbinding/gl/functions.h> // functions
    #include <glbinding/gl/types.h>     // type declarations of the OpenGL API (including bitfields, boolean, enum, and extensions)
    #include <glbinding/gl/values.h>    // special values (e.g., GL_INVALID_INDEX)
*/
#include <glbinding/glbinding.h>   // glbinding initialization and management


#include "Types.h"
constexpr static inline gl::GLenum to_glenum(u32 a){
    return static_cast<gl::GLenum>(a);
}
constexpr static inline u32 to_u32(gl::GLenum a){
    return static_cast<u32>(a);
}
template<typename T>
constexpr gl::GLenum gl_type(){
    gl::GLenum res{};
    if constexpr (std::same_as<T, f32>){
        res = gl::GL_FLOAT;
    } else if constexpr(std::same_as<T,f64>){
        res = gl::GL_DOUBLE;
    } else if constexpr(std::same_as<T,i32>){
        res = gl::GL_INT;
    }else if constexpr (std::same_as<T,u32>){
        res = gl::GL_UNSIGNED_INT;
    }else if constexpr (std::same_as<T,u8>){
        res = gl::GL_UNSIGNED_BYTE;
    }
    return res;
}

#define _GLFW_INCLUDE_WARNING 
