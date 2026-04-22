#include <format>
#include <string>
#include <sstream>

#include "CommonConcepts.hpp"

// @brief returns a string containing the type and value category, i.e the type that is passed with perfect forwarding
template <typename T>
constexpr std::string dbg_fmt(const T& val) {
    std::string expr_str{};
    using Arg = std::remove_cvref_t<T>;

    if constexpr(std::is_pointer_v<T>){
        // maybe change in future to deref automatically, but show its a pointer
        expr_str = std::format("{}", (void*)val); 
    } else if constexpr (Formattable<T>) {
        expr_str = std::format("{}", val);
    } else if constexpr (OStreamable<T>){
        std::ostringstream oss;
        oss << val;
        std::string expr_str = oss.str();
    }else {
        expr_str =  "<unformattable tomato>";
    }
    return expr_str;
}
