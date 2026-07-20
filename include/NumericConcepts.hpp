#pragma once 
#include "CommonConcepts.hpp"
#define NUMERIC_CONCEPTS_TESTS
template <typename T>
concept FloatingPoint = std::floating_point<std::remove_cvref_t<T>>;

template <typename T>
concept Integral = std::integral<std::remove_cvref_t<T>>;


template <typename T>
concept Numeric = FloatingPoint<T> || Integral<T>;



template <typename T>
struct vec_traits { static constexpr bool is_glm = false; };

template <glm::length_t L, typename E, glm::qualifier Q>
struct vec_traits<glm::vec<L, E, Q>> {
    static constexpr bool is_glm = true;
    static constexpr glm::length_t length = L;
    using element = E;
};

template <typename Tag, typename ScalarType> struct Coord2;
template <typename Tag, typename E>
struct vec_traits<Coord2<Tag, E>> {
    static constexpr bool is_glm = false;
    static constexpr glm::length_t length = 2;
    using element = E;
};

template <typename Tag, typename ScalarType> struct Coord3;
template <typename Tag, typename E>
struct vec_traits<Coord3<Tag, E>> {
    static constexpr bool is_glm = false;
    static constexpr glm::length_t length = 3;
    using element = E;
};


template <typename T>
concept is_fvec = 
requires{ typename vec_traits<no_cvref<T>>::element;} &&
    FloatingPoint<typename vec_traits<no_cvref<T>>::element>;

template <typename T>
concept is_ivec =
    requires { typename vec_traits<no_cvref<T>>::element; } &&
    Integral<typename vec_traits<no_cvref<T>>::element>;

template <typename T, size_t N>
concept is_vec_N = vec_traits<no_cvref<T>>::length == N;

template <typename T, size_t N>
concept is_ivec_N = is_ivec<T> && is_vec_N<T,N>;

template <typename T, size_t N>
concept is_fvec_N = is_fvec<T> && is_vec_N<T,N>; 

template <typename T>
concept is_vec2 = is_vec_N<T,2>;
template <typename T>
concept is_vec3 = is_vec_N<T,3>;
template <typename T>
concept is_vec4 = is_vec_N<T,4>;

template <typename T>
concept is_fvec2 = is_fvec_N<T,2>;
template <typename T>
concept is_fvec3 = is_fvec_N<T,3>;
template <typename T>
concept is_fvec4 = is_fvec_N<T,4>;

template <typename T>
concept is_ivec2 = is_ivec_N<T,2>;
template <typename T>
concept is_ivec3 = is_ivec_N<T,3>;
template <typename T>
concept is_ivec4 = is_ivec_N<T,4>;

template <typename T>
inline constexpr bool is_glm_vec = vec_traits<T>::is_glm;

#ifdef NUMERIC_CONCEPTS_TESTS
static_assert( is_fvec2<glm::vec2>);
static_assert(!is_fvec3<glm::vec2>);
static_assert(!is_fvec4<glm::vec2>);
static_assert(!is_ivec2<glm::vec2>);
static_assert(!is_ivec3<glm::vec2>);
static_assert(!is_ivec4<glm::vec2>);

static_assert(!is_fvec2<glm::vec3>);
static_assert( is_fvec3<glm::vec3>);
static_assert(!is_fvec4<glm::vec3>);
static_assert(!is_ivec2<glm::vec3>);
static_assert(!is_ivec3<glm::vec3>);
static_assert(!is_ivec4<glm::vec3>);

static_assert(!is_fvec2<glm::vec4>);
static_assert(!is_fvec3<glm::vec4>);
static_assert( is_fvec4<glm::vec4>);
static_assert(!is_ivec2<glm::vec4>);
static_assert(!is_ivec3<glm::vec4>);
static_assert(!is_ivec4<glm::vec4>);

static_assert(!is_fvec2<glm::ivec2>);
static_assert(!is_fvec3<glm::ivec2>);
static_assert(!is_fvec4<glm::ivec2>);
static_assert( is_ivec2<glm::ivec2>);
static_assert(!is_ivec3<glm::ivec2>);
static_assert(!is_ivec4<glm::ivec2>);

static_assert(!is_fvec2<glm::ivec3>);
static_assert(!is_fvec3<glm::ivec3>);
static_assert(!is_fvec4<glm::ivec3>);
static_assert(!is_ivec2<glm::ivec3>);
static_assert( is_ivec3<glm::ivec3>);
static_assert(!is_ivec4<glm::ivec3>);

static_assert(!is_fvec2<glm::ivec4>);
static_assert(!is_fvec3<glm::ivec4>);
static_assert(!is_fvec4<glm::ivec4>);
static_assert(!is_ivec2<glm::ivec4>);
static_assert(!is_ivec3<glm::ivec4>);
static_assert( is_ivec4<glm::ivec4>);


#endif 
// tests for them all 

