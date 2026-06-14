#pragma once
#include <format>
#include <concepts>
#include <map>
#include <map>
#include <ranges>
#include <type_traits>
#include <string>
#include "Macros.hpp"
// SECTION:: STL wrappers that are more understandable semantically (for me).
// Some are more for convinience.

template <typename C>
concept ArrayLike = std::ranges::contiguous_range<C> && requires(C c){
    {c.size()} -> std::same_as<std::size_t>;
};
static_assert(ArrayLike<std::array<int,10>>);
static_assert(ArrayLike<std::vector<int>>);
//static_assert(!ArrayLike<std::map<int,int>>);

// TODO: Make this concept work, then make the map overload, then format
template <typename C>
concept MapLike = requires(C c){
    typename std::remove_cvref_t<C>::value_type;
    typename std::remove_cvref_t<C>::mapped_type;
    typename std::remove_cvref_t<C>::key_type;

// BUG: the following line needs some sort of conditonal metaprogramming i think using like std::conditional shit.
// i have no idea how to do that so this concept just assumes ::key_type and ::mapped_type are enough to differentiate


//    {c.at(std::declval<typename std::remove_cvref_t<C>::key_type>())}->std::same_as<typename std::remove_cvref_t<C>::mapped_type&>;
    {c.size()} -> std::same_as<std::size_t>;
};
static_assert(MapLike<std::unordered_map<int,std::string>>);
static_assert(MapLike<std::unordered_map<int,std::string>&>);
static_assert(MapLike<std::map<int,std::string>>);

static_assert(!(MapLike<std::array<int,10>>));
static_assert(!(MapLike<std::vector<int>>));

template <typename T>
concept SequenceContainer = std::ranges::range<T>;

//NOTE: We use numeric_limits<T>::lowest(), as floating points types ::min() returns the minimum possible POSITIVE value, which is not what most would expect.
template<typename T>
ALWAYS_INLINE constexpr auto numeric_min(){ return std::numeric_limits<T>::lowest();}

template<typename T>
ALWAYS_INLINE constexpr auto numeric_max(){ return std::numeric_limits<T>::max();}



template <typename T, typename Fn, typename ...Args>
concept return_type_is =  
    std::invocable<Fn, Args...> &&
    std::same_as<std::invoke_result_t<Fn, Args...>, T>;

template <typename Fn, typename ...Args>
using return_type = std::invoke_result_t<Fn, Args...>;

template<typename T1, typename T2>
concept same_as_nocvref = std::same_as<std::remove_cvref_t<T1>, std::remove_cvref_t<T2>>;

template<typename T1>
using val_t = T1::value_type;

template <typename T>
concept has_to_string_overload = requires(T e) {
    std::to_string(e); 
};

template <typename T>
concept has_name_member = requires(T e) {
    { e.name } -> same_as_nocvref<std::string>;
};

template <typename T>
concept is_string_pair = requires(T e) {
    { e.second } -> same_as_nocvref<std::string>;
};

template<typename Fn, typename Vec>
concept has_3_scalar_params = std::invocable<Fn,val_t<Vec>,val_t<Vec>,val_t<Vec>>;

template<typename Fn, typename Vec>
concept has_2_scalar_params = std::invocable<Fn,val_t<Vec>,val_t<Vec>>;

template<typename Fn, typename ...Args>
concept callable_by = std::invocable<Fn, Args...>;


enum struct IterationSignal{
    CONTINUE,
    BREAK,
};
template <typename T>
concept Formattable = std::formattable<std::remove_cvref_t<T>, char>;

// Requires that a type implements a '<<' overload which returns an ostream& 
template <typename T>
concept OStreamable = requires(T x, std::ostream& os){
    {os << x} -> std::same_as<std::ostream&>;
};
