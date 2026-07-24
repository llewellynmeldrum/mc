#pragma once

#include "BitwiseOps.hpp"
#include "CommonConcepts.hpp"
#include "NumericConcepts.hpp"
#include "Types.h"
#include "glm/vec3.hpp"
#include <random>
#include <range/v3/range/concepts.hpp>
#include <range/v3/algorithm/contains.hpp>
#include <string_view>
#include <type_traits>




namespace LM {

// @BRIEF: 
// Classes which contain a ctor which accepts a deferred_init_t accept the contract that they will perform:
// -> NO intialization,
// -> NO resource acquisition (mutex, memory arenas, etc)
// -> NO memory allocation
// ->or anything of the sort, 
//  WITHIN this constructor.
    // --
    // A corresponding .init()/.setup() method needs to be provided if this ctor is used.
// Classes designed like this should probably resemble:

/* 
struct Foo{
    Foo() {init();}
    Foo(deferred_init_t){}

    void init();            // <- performs the init/acquisition/allocations
}
*/ 
struct deferred_init_t{};
inline constexpr deferred_init_t deferred_init{};

template<typename Cont, typename T>
constexpr inline bool linear_contains(const Cont& cont, const T& e){
    if constexpr( requires(Cont c, T e) {
        {c.contains(e)} -> std::same_as<bool>;
    }){
        return cont.contains(e);
    }else{
        // linear search
        return ranges::contains(cont,e);
    }
}



template<typename T>
    requires Numeric<T>
constexpr inline T random(T min = numeric_min<T>(), T max = numeric_max<T>()){
    auto get_seed = [](){
        std::random_device rd;
        auto seed = rd();
        return ((seed == 0) ? 1uz : seed);
    };
    thread_local std::minstd_rand0 gen(get_seed());
    std::uniform_int_distribution dist{min,max};
    return dist(gen);
}




constexpr inline i32 ieuclid_mod(i32 a, i32 b) noexcept{
    i32 r = a % b;
    return r<0 ? r+b : r;
}


constexpr inline f32 euclid_mod(f32 a, f32 b) noexcept{
    return fmod(a,b);
}


template<typename T>
    requires Numeric<T>
constexpr inline T avg(T a, T b) noexcept{
    return ((a+b)/static_cast<T>(2));
}

constexpr inline i32 floor_div(i32 a, i32 b) noexcept{
    i32 quotient = a/b;
    bool hasRemainder =a%b!=0;
    if (hasRemainder && SignsDiffer(a,b)){
        quotient-=1; // round down
    }
    return quotient;
}

constexpr inline f32 floor_div(f32 a, i32 b) noexcept{
    return std::floor(a/b);
}
template<typename T>
requires Numeric<T>
constexpr inline T constrain(T lo, T hi, T val) noexcept{
    return std::min(std::max(val,lo),hi);
}

template <typename Float>
requires FloatingPoint<Float>
constexpr Float unlerp(Float a, Float b, Float t) noexcept {
    if (t<=a) return a;
    if (t>=b) return b;
    if (glm::abs(a - b) < 1e-9) {
        return Float{0};
    }
    return (t - a) / (b - a);
}


template<typename A, typename B, typename C>
requires is_ivec3<A> && is_ivec3<B> && is_ivec3<C>
inline constexpr bool isVecInBounds(A v, B lo, C hi) noexcept {
    return lo[0] <= v[0] && v[0] < hi[0] &&  //
           lo[1] <= v[1] && v[1] < hi[1] &&  //
           lo[2] <= v[2] && v[2] < hi[2];    //
}

template<typename A, typename B, typename C>
requires is_ivec2<A> && is_ivec2<B> && is_ivec2<C>
inline constexpr bool isVecInBounds(A v, B lo, C hi) noexcept {
    return lo[0] <= v[0] && v[0] < hi[0] &&  //
           lo[1] <= v[1] && v[1] < hi[1];
}

template <typename Number>
requires Numeric<Number>
constexpr Number square(Number a) noexcept {
    return a*a;
}
template<typename Vec3>
requires is_vec3<Vec3> 
inline constexpr auto piecewise_mult(Vec3 p0, Vec3 p1) noexcept {
    return no_cvref<Vec3>{
        p1[0] * p0[0],
        p1[1] * p0[1],
        p1[2] * p0[2],
    };
}

// Euclidean distance squared between two anyvec3's
template<typename Vec3>
requires is_vec3<Vec3> 
inline constexpr f32 sq_dist(Vec3 p0, Vec3 p1) noexcept {
    return  square(p1[0] - p0[0]) + 
            square(p1[1] - p0[1]) + 
            square(p1[2] - p0[2]);
}

// Euclidean distance squared between two anyvec3's
template<typename Vec2>
    requires is_vec2<Vec2> 
inline constexpr f32 sq_dist(Vec2 p0, Vec2 p1) noexcept {
    return  square(p1[0] - p0[0]) + 
            square(p1[1] - p0[1]);
}

template<typename Vec3>
requires is_vec3<Vec3> 
inline constexpr f32 dist(Vec3 p0, Vec3 p1) noexcept {
    return std::sqrt(sq_dist(p0,p1));
}
template<typename Vec2>
requires is_vec2<Vec2> 
inline constexpr f32 dist(Vec2 p0, Vec2 p1) noexcept {
    return std::sqrt(sq_dist(p0,p1));
}

// TODO: test this function?
template<typename A, typename B>
requires is_ivec3<A> && is_ivec3<B>
constexpr inline std::remove_cvref_t<A> euclid_mod(A a, B b) noexcept {
    return {
        ieuclid_mod(a[0],b[0]),
        ieuclid_mod(a[1],b[1]),
        ieuclid_mod(a[2],b[2]),
    };
}

template<typename A>
    requires is_fvec3<A>
constexpr inline std::remove_cvref_t<A> floor(A a) noexcept {
    return {
        std::floor(a[0]),
        std::floor(a[1]),
        std::floor(a[2]),
    };
}

template<typename A, typename B>
requires is_ivec3<A> && is_ivec3<B>
constexpr inline std::remove_cvref_t<A> floor_div(A a, B b) {
    return {
        floor_div(a[0],b[0]),
        floor_div(a[1],b[1]),
        floor_div(a[2],b[2]),

    };
}
template<typename A, typename B>
requires is_fvec3<A> && is_vec3<B>
constexpr inline std::remove_cvref_t<A> floor_div(A a, B b) {
    return {
        floor_div(a[0],b[0]),
        floor_div(a[1],b[1]),
        floor_div(a[2],b[2]),
    };
}

template<typename IntVector3>
requires is_ivec3<IntVector3> 
constexpr inline std::remove_cvref_t<IntVector3> min(IntVector3 a, IntVector3 b) noexcept {
    return {
        std::min(a[0],b[0]),
        std::min(a[1],b[1]),
        std::min(a[2],b[2]),
    };
}
template<typename A, typename B>
requires is_ivec3<A> && is_ivec3<B>
constexpr inline std::remove_cvref_t<A> max(A a, B b) noexcept {
    return {
        std::max(a[0],b[0]),
        std::max(a[1],b[1]),
        std::max(a[2],b[2]),
    };
}


}  // namespace LM
