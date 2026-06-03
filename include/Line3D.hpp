#pragma once 
#include "CommonConcepts.hpp"
#include "Types.h"

struct Line3D{
    Line3D(glm::vec3 _p0, glm::vec3 _p1) : p0(_p0), p1(_p1){
    }
    glm::vec3 p0;
    glm::vec3 p1;
    glm::vec4 color={1,1,1,1};
};
extern f32 Line3D_thickness;


