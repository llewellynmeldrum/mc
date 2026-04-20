#pragma once 
#include "Types.h"
#include "Vectors.hpp"
#include <Array>
#include <cassert>
#include <initializer_list>
#include <mdspan>

struct mat4{
    mat4() = default;
    static const mat4 identity;
    static constexpr u64 ROWS{4}, COLS {4};

    auto view() {
    }
    mat4(f32 val){
        auto v = view();
        for (u64 i = 0; i<ROWS; i++){
            v[i,i] = val;
        }
    }
private: 
};

// multiplying a vec4 by the identiy matrix leaves it untouched.
const mat4 mat4::identity = mat4(1.0);


// scaling matrix:
// Sx    0    0    0 | 
//  0   Sy    0    0 |
//  0    0   Sx    0 |
//  0    0    0   Sz | 

