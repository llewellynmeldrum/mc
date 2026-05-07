#if defined(__clang__)
#pragma clang diagnostic ignored "-Weverything"
#elif defined(__GNUC__) || defined(__GNUG__)
#pragma GCC diagnostic ignored "-Wall"
#endif
// c++ doesnt like some of the switch statements in stb_image
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.hpp"
