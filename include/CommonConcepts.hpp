#pragma once
// Requires that T is implemented by std::format
#include <format>
template <typename T>
concept Formattable = std::formattable<std::remove_cvref_t<T>, char>;

// Requires that a type implements a '<<' overload which returns an ostream& 
template <typename T>
concept OStreamable = requires(T x, std::ostream& os){
    {os << x} -> std::same_as<std::ostream&>;
};
