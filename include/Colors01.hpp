#pragma once 

#include "glmVecTypes.hpp"
using StorageType = glm::vec4;
namespace Color01{

constexpr inline StorageType RED        {1.0f,0.0f,0.0f,1.0f};
constexpr inline StorageType WHITE      {1.0f,1.0f,1.0f,1.0f};
constexpr inline StorageType ORANGE     {1.0f,0.5f,0.0f,1.0f};
constexpr inline StorageType BROWN      {0.4f,.15f,0.0f,1.0f};
constexpr inline StorageType YELLOW     {1.0f,1.0f,0.0f,1.0f};
constexpr inline StorageType GREEN      {0.0f,1.0f,0.0f,1.0f};
constexpr inline StorageType BLUE       {0.0f,0.0f,1.0f,1.0f};
constexpr inline StorageType PURPLE     {1.0f,0.0f,1.0f,1.0f};

}; // Namespace Color01
