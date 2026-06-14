#pragma once 

#include <meta>
constexpr auto name = std::meta::identifier_of(^^int); // should be "int"


