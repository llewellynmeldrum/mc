#pragma once 

#include "glm/gtc/type_ptr.hpp"
using namespace glm;
template <class T>
concept is_glm_type = requires(T obj){
    true;
    // TODO: idk how to do this properly
};

// returns a constant address to a glm object
template <class T>
requires is_glm_type<T>
const auto data(T obj){
    return glm::value_ptr(obj);
}

