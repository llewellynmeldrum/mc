#pragma once 

#include "glm/gtc/type_ptr.hpp"
using namespace glm;
template <class T>
concept is_glm_type = requires(T obj){
true;
};
template<typename T>
concept is_glm_vec2 = requires(T v) {
    { v.x } -> std::convertible_to<float>;
    { v.y } -> std::convertible_to<float>;
    typename T::value_type;
    { T(1.0f) };            // Constructor requirement
};
template<typename T>
concept is_glm_vec3 = requires(T v) {
    { v.x } -> std::convertible_to<float>;
    { v.y } -> std::convertible_to<float>;
    { v.z } -> std::convertible_to<float>;
    typename T::value_type;
    { T(1.0f) };            // Constructor requirement
};
template<typename T>
concept is_glm_vec4 = requires(T v) {
    { v.x } -> std::convertible_to<float>;
    { v.y } -> std::convertible_to<float>;
    { v.z } -> std::convertible_to<float>;
    { v.w } -> std::convertible_to<float>;
    typename T::value_type;
};


template<typename T>
concept is_glm_mat4 = requires(T m, glm::vec4 v) {
    { m[0] }; // Column access
    { m * v } -> std::same_as<glm::vec4>; // Matrix-vector multiplication
};
// returns a constant address to a glm object
template <class T>
requires is_glm_type<T>
const auto data(T obj){
    return glm::value_ptr(obj);
}

