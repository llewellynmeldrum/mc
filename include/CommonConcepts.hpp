#pragma once
#include <format>
#include <concepts>
#include <map>
#include <map>
#include <ranges>
#include <memory>
#include <type_traits>
#include <string>
#include <vector>
#include <unordered_map>
#include "Macros.hpp"
// =========================
// STD template wrappers
// ========================

template<typename Fn, typename ...Args>
concept callable_with = std::invocable<Fn, Args...>;

template <typename T>
using no_cvref = std::remove_cvref_t<T>;

template<typename T>
concept hashable = requires {std::hash<T>{};};


template<typename T, typename ...Args>
concept paren_constructible = std::constructible_from<T, Args...>;

template<typename T, typename ...Args>
concept brace_constructible = requires(T t, Args... vargs){
    T{vargs...}->T;
};

template<typename T, typename V>
concept implicit_convertible_to = std::convertible_to<T,V>;

template<typename T, typename V>
concept explicit_convertible_to = requires(T t, V v){
    {static_cast<V>(t)};
};

template<typename T>
concept has_default_ctor = std::is_default_constructible_v<T>;


template<typename T1, typename T2>
concept same_type = std::same_as<T1,T2>;

template<typename T1, typename T2>
concept same_type_no_cvref = std::same_as<no_cvref<T1>,no_cvref<T2>>;

template<typename T, typename U1, typename U2>
concept same_as_either = std::same_as<T, U1> || std::same_as<T,U2>;

template <typename T>
concept pointer_like = requires(T ptr) {
    *ptr;
    { static_cast<bool>(ptr) } -> std::convertible_to<bool>; 
};

template <typename T>
concept is_smart_ptr = pointer_like<T> && !std::is_pointer_v<T>;

template <typename T, typename Deleter= std::default_delete<T> >
struct is_a_unique_ptr : std::false_type {};

template<typename T>
struct is_a_unique_ptr<std::unique_ptr<T>>:std::true_type{};

template<typename T>
inline constexpr bool is_a_unique_ptr_v = is_a_unique_ptr<T>::value;

template<typename T>
concept is_unique_ptr = is_a_unique_ptr_v<T>;


template <typename T>
concept sequential_container = std::ranges::range<T>;

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
concept same_as_nocvref = std::same_as<no_cvref<T1>, no_cvref<T2>>;

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


enum struct IterationSignal{
    CONTINUE,
    BREAK,
};
template <typename T>
concept Formattable = std::formattable<no_cvref<T>, char>;

// Requires that a type implements a '<<' overload which returns an ostream& 
template <typename T>
concept OStreamable = requires(T x, std::ostream& os){
    {os << x} -> std::same_as<std::ostream&>;
};


//static_assert(!ArrayLike<std::map<int,int>>);

template <typename C>
concept map_like = requires(C c){
    typename no_cvref<C>::mapped_type;
    typename no_cvref<C>::key_type;
    

// BUG: the following line needs some sort of conditonal metaprogramming i think using like std::conditional shit.
// i have no idea how to do that so this concept just assumes ::key_type and ::mapped_type are enough to differentiate


    //{c.at(std::declval<typename no_cvref<C>::key_type>())}->std::same_as<typename no_cvref<C>::mapped_type&>;
    {c.size()} -> std::same_as<std::size_t>;
    {c.contains(std::declval<typename no_cvref<C>::key_type>())} -> same_as_nocvref<bool>;
};

template <typename C>
concept array_like = !map_like<C> && std::ranges::contiguous_range<C> && requires(C c){
    {c.size()} -> std::same_as<std::size_t>;
};

// ================= 
//       TESTS
// =================
//
static_assert(hashable<int>);
static_assert(hashable<std::string>);
static_assert(!hashable<std::pair<std::string,std::string>>);

static_assert(!std::convertible_to<void*,int*>);
static_assert(explicit_convertible_to<void*,int*>);

static_assert(array_like<std::array<int,10>>);
static_assert(array_like<std::vector<int>>);

static_assert(map_like<std::unordered_map<int,std::string>>);
static_assert(map_like<std::unordered_map<int,std::string>&>);
static_assert(map_like<std::map<int,std::string>>);

static_assert(!(map_like<std::array<int,10>>));
static_assert(!(map_like<std::vector<int>>));
