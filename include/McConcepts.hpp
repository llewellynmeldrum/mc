#pragma once 
#include <concepts>
#include "CommonConcepts.hpp"
#include "NumericConcepts.hpp"
template <typename T>
concept CoordLike = requires(T v){
    {v.raw()}->std::convertible_to<glm::vec3>;
};
template <typename T>
concept AnyCoord = AnyVec3Like<T> && !AnyVec4Like<T> && CoordLike<T>;
