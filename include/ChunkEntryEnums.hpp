#pragma once
#include "Block.hpp"
enum struct JobType{
    Gen,
    Mesh,
    Light,
};
enum struct DataType{
    Block,
    PackedLight
};
template<typename T>
concept chunk_store_compatible =(
       std::same_as<T,Block> 
    || std::same_as<T,PackedLightValue>
);
