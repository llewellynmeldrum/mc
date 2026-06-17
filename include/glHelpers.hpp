#pragma once 

#include <unordered_map>
#include <string>

#include "Types.h"
#include "glbinding/gl/enum.h"




std::string lookup_gl_error_description( const std::string& function_name, const std::string& error);

