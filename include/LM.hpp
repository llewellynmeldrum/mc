#pragma once

#include "Bitwise.hpp"
#include "NumericConcepts.hpp"
#include "Types.h"
#include "glm/vec3.hpp"
#include <type_traits>
namespace LM {

struct variable{
    std::string_view type_str;
    std::string_view  val_str;
    std::string_view  name_str;
    const auto& val(){return val_str;}
    const auto& type(){return type_str;}
    const auto& name(){return name_str;}
};
struct source_location{
    const char* _file;
    const char* _function;
    const char* _pretty_fn;
    int _line;
    int _col{0};
    const auto& file_name(){return _file;}
    const auto& function_name(){return _function;}
    const auto& pretty_fn(){return _pretty_fn;}
    const auto& line(){return _line;}
    const auto& column(){return _col;}
};
#define SRC_LOC_CURRENT()\
    LM::source_location{\
        ._file=__FILE_NAME__,\
        ._function=__FUNCTION__,\
        ._pretty_fn=__PRETTY_FUNCTION__,\
        ._line=__LINE__,\
    }
#define VAR_DBG(val)\
    LM::variable{\
        .type_str = pretty_type_name<decltype(val)>(),\
        .val_str = std::format("{}",val),\
        .name_str= #val,\
    }
constexpr inline i32 ieuclid_mod(i32 a, i32 b) noexcept{
    i32 r = a % b;
    return r<0 ? r+b : r;
}

template<typename T>
requires Numeric<T>
constexpr inline T min(T a, T b) noexcept{
    return a<b ? a : b;
}
template<typename T>
requires Numeric<T>
constexpr inline T max(T a, T b) noexcept{
    return a>b ? a : b;
}

template<typename T>
requires Numeric<T>
constexpr inline T avg(T a, T b) noexcept{
    return (a+b/static_cast<T>(2));
}

constexpr inline i32 ifloor_div(i32 a, i32 b) noexcept{
    i32 quotient = a/b;
    bool hasRemainder =a%b!=0;
    if (hasRemainder && SignsDiffer(a,b)){
        quotient-=1; // round down
    }
    return quotient;
}
template<typename A, typename B>
requires FloatingPoint<A> && Numeric<B>
constexpr inline A ffloor_div(A a, B b) noexcept{
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
    if (glm::abs(a - b) < 1e-9) {
        return Float{0};
    }
    return (t - a) / (b - a);
}

template<typename A, typename B, typename C>
requires IVec3<A> && IVec3<B> && IVec3<C>
constexpr bool isVecInBounds(A v, B lo, C hi) noexcept {
    return lo.x <= v.x && v.x < hi.x &&  //
           lo.y <= v.y && v.y < hi.y &&  //
           lo.z <= v.z && v.z < hi.z;    //
}



template<typename A, typename B>
requires IVec3<A> && IVec3<B>
constexpr inline std::remove_cvref_t<A> euclid_mod(A a, B b) noexcept {
    return {
        ieuclid_mod(a.x,b.x),
        ieuclid_mod(a.y,b.y),
        ieuclid_mod(a.z,b.z),
    };
}
template<typename A>
requires FVec3<A>
constexpr inline std::remove_cvref_t<A> floor(A a) noexcept {
    return {
        std::floor(a.x),
        std::floor(a.y),
        std::floor(a.z),
    };
}

template<typename A, typename B>
requires IVec3<A> && IVec3<B>
constexpr inline std::remove_cvref_t<A> floor_div(A a, B b) {
    return {
        ifloor_div(a.x,b.x),
        ifloor_div(a.y,b.y),
        ifloor_div(a.z,b.z),
    };
}
template<typename A, typename B>
requires FVec3<A> && AnyVec3<B>
constexpr inline std::remove_cvref_t<A> floor_div(A a, B b) {
    return {
        ffloor_div(a.x,b.x),
        ffloor_div(a.y,b.y),
        ffloor_div(a.z,b.z),
    };
}

template<typename A, typename B>
requires IVec3<A> && IVec3<B>
constexpr inline std::remove_cvref_t<A> min(A a, B b) noexcept {
    return {
        min(a.x,b.x),
        min(a.y,b.y),
        min(a.z,b.z),
    };
}
template<typename A, typename B>
requires IVec3<A> && IVec3<B>
constexpr inline std::remove_cvref_t<A> max(A a, B b) noexcept {
    return {
        max(a.x,b.x),
        max(a.y,b.y),
        max(a.z,b.z),
    };
}


}  // namespace LM
