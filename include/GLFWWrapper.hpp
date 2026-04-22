#pragma once 
#ifdef _GLFW_INCLUDE_WARNING
#error "You have included glbinding before glfw! Order must be glfw->glbinding"
#endif

#ifdef __APPLE__
    #define GL_SILENCE_DEPRECATION
#endif // __APPLE__
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
