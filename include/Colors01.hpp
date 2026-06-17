#pragma once 

#include "Types.h"
#include "glmVecTypes.hpp"
#include <initializer_list>
using StorageType = glm::vec4;
namespace Color01{


#define rgb(r,g,b) r/255.0,g/255.0,b/255.0,
#define decl_color(NAME, c)\
constexpr inline StorageType NAME {c 1.0f};\
constexpr inline StorageType CONCAT(NAME,_a) (u8 alpha) {return{c alpha/255.0};};

decl_color(RED    ,rgb(255,000,000))
decl_color(WHITE  ,rgb(255,255,255))
decl_color(ORANGE ,rgb(255,127,000))
decl_color(BROWN  ,rgb(102, 38,000))
decl_color(YELLOW ,rgb(255,255,000))
decl_color(GREEN  ,rgb(000,255,000))
decl_color(BLUE   ,rgb(000,000,255))
decl_color(PURPLE ,rgb(255,000,255))
decl_color(GREY_50,rgb(127,127,127))
}; // Namespace Color01
#undef color
