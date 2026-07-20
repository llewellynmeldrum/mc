#pragma once 

#include "WorldGen_BiomeFeatures.hpp"
//#include "WorldGen_SharedBiomeFeatures.hpp"

// NOTE:
// Feature traits describe static requirements and characteristics of features.
template<typename T>
struct feature_traits{
    constexpr static bool allow_underwater = false;
    constexpr static bool allow_outside_water = true;
};


