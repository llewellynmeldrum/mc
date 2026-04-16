#pragma once 

#include <unordered_map>
#include <string>

#include "Types.h"
#include "glbinding/gl/enum.h"

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
    }
    return res;
}

