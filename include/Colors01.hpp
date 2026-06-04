#pragma once 

#include "Types.h"
#include "glmVecTypes.hpp"
using StorageType = glm::vec4;
namespace Color01{

constexpr inline StorageType TRANSPARENT {0.0f,0.0f,0.0f,0.0f};
constexpr inline StorageType RED        {1.0f,0.0f,0.0f,1.0f};
constexpr inline StorageType WHITE      {1.0f,1.0f,1.0f,1.0f};
constexpr inline StorageType ORANGE     {1.0f,0.5f,0.0f,1.0f};
constexpr inline StorageType BROWN      {0.4f,.15f,0.0f,1.0f};
constexpr inline StorageType YELLOW     {1.0f,1.0f,0.0f,1.0f};
constexpr inline StorageType GREEN      {0.0f,1.0f,0.0f,1.0f};
constexpr inline StorageType BLUE       {0.0f,0.0f,1.0f,1.0f};
constexpr inline StorageType PURPLE     {1.0f,0.0f,1.0f,1.0f};

constexpr inline StorageType RED_a    (f32 alpha)    {return{1.0f,0.0f,0.0f,alpha};}
constexpr inline StorageType WHITE_a (f32 alpha)    {return{1.0f,1.0f,1.0f,alpha};}
constexpr inline StorageType ORANGE_a (f32 alpha)    {return{1.0f,0.5f,0.0f,alpha};}
constexpr inline StorageType BROWN_a (f32 alpha)    {return{0.4f,.15f,0.0f,alpha};}
constexpr inline StorageType YELLOW_a (f32 alpha)    {return{1.0f,1.0f,0.0f,alpha};}
constexpr inline StorageType GREEN_a (f32 alpha)    {return{0.0f,1.0f,0.0f,alpha};}
constexpr inline StorageType BLUE_a (f32 alpha)    {return{0.0f,0.0f,1.0f,alpha};}
constexpr inline StorageType PURPLE_a (f32 alpha)    {return{1.0f,0.0f,1.0f,alpha};}
}; // Namespace Color01
