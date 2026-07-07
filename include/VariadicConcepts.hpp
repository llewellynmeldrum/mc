#include "CommonConcepts.hpp"
#include <concepts>

// Ensures all types in Args are std::same_as<T>
template<typename T, typename ...Args>
concept variadic_all_same = (std::same_as<T, Args> && ...);

// Ensures all types in Args are std::same_as<no_cvref<T>>
template<typename T, typename ...Args>
concept variadic_all_same_nocvref = (std::same_as<no_cvref<T>, no_cvref<Args>> && ...);
