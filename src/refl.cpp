#include "refl.hpp"

#include <string_view>
using namespace std::string_view_literals;
namespace refl{
bool variable::is_integral() const noexcept{
    return (
            type_str == "unsigned int"sv
        ||  type_str == "int"sv
    );
    
}
} 
