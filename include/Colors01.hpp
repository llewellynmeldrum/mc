#pragma once 

#include "Types.h"
#include "glmVecTypes.hpp"
using StorageType = glm::vec4;
namespace Color01{

#define color(NAME, r,g,b)\
constexpr inline StorageType NAME {r,g,b,1.0f};\
constexpr inline StorageType CONCAT(NAME,_a) (f32 alpha) {return{r,g,b,alpha};};

color(RED,       1.0,0.0,0.0)
color(WHITE,     1.0,1.0,1.0)
color(ORANGE ,   1.0,0.5,0.0)
color(BROWN  ,   0.4,.15,0.0)
color(YELLOW ,   1.0,1.0,0.0)
color(GREEN  ,   0.0,1.0,0.0)
color(BLUE   ,   0.0,0.0,1.0)
color(PURPLE ,   1.0,0.0,1.0)
color(GREY_50,   0.5,0.5,0.5)

}; // Namespace Color01
#undef color
