#pragma once

#include "NumericConcepts.hpp"
#include "Types.h"

// i refuse to pollute 5 different classes with this bullshit
#define DECL_MOVE_ONLY(CLASS_NAME)                                                                 \
    CLASS_NAME(const CLASS_NAME&) = delete;                                                        \
    CLASS_NAME& operator=(const CLASS_NAME&) = delete;                                             \
    CLASS_NAME(CLASS_NAME&&) = default;                                                            \
    CLASS_NAME& operator=(CLASS_NAME&&) = default;

#define DECL_NO_COPY(CLASS_NAME)                                                                   \
    CLASS_NAME(const CLASS_NAME&) = delete;                                                        \
    CLASS_NAME& operator=(const CLASS_NAME&) = delete;


#define FORWARD_DECL_ENUM_STRUCT( NAME, UNDERLYING)                                                \
    enum class NAME : UNDERLYING;

#define FORWARD_DECL_ENUM_CLASS(NAMESPACE, NAME, UNDERLYING)                                       \
    enum class NAME : UNDERLYING;

#define FORWARD_DECL_STRUCT(NAME)                                                                  \
    struct NAME;

#define FORWARD_DECL_CLASS(NAME)                                                                   \
    class NAME;

#define FORWARD_DECL_ENUM_STRUCT_NS(NAMESPACE, NAME, UNDERLYING)                                   \
    namespace NAMESPACE {                                                                          \
        FORWARD_DECL_ENUM_STRUCT(NAME,UNDERLYING)                                                  \
    }

#define FORWARD_DECL_ENUM_CLASS_NS(NAMESPACE, NAME, UNDERLYING)                                    \
    namespace NAMESPACE {                                                                          \
        FORWARD_DECL_ENUM_CLASS(NAME,UNDERLYING)                                                   \
    }

#define FORWARD_DECL_STRUCT_NS(NAMESPACE, NAME, UNDERLYING)                                        \
    namespace NAMESPACE {                                                                          \
        FORWARD_DECL_STRUCT(NAME,UNDERLYING)                                                       \
    }

#define FORWARD_DECL_CLASS_NS(NAMESPACE, NAME, UNDERLYING)                                        \
    namespace NAMESPACE {                                                                          \
        FORWARD_DECL_CLASS(NAME,UNDERLYING)                                                       \
    }

#define    ALL_CTORS_DEFAULT(T)\
    T(const T&)=default;\
    T(T&&)=default;\
    T& operator=(T&&)=default;\
    T& operator=(const T&)=default;

// NOTE! This requires an equality operator to be defined as well.
#define STD_HASH_SPECIALIZATION(T, tName, HashFunctionContents)     \
namespace std{                                                      \
    template<>                                                      \
    struct hash<T>{                                                 \
        std::size_t operator()(const T& tName) const {              \
            HashFunctionContents                                    \
        }                                                           \
    };                                                              \
}













// @SECTION
// std::views iota and cartesian product wrappers (1/2/3D range based for loop helpers)
constexpr inline const auto EachInRange(
    i64 x0, i64 x1,
    i64 y0, i64 y1,
    i64 z0, i64 z1) {
    using std::views::iota;
    using std::views::cartesian_product;

    return cartesian_product(iota(x0,x1), iota(y0,y1), iota(z0,z1));
}

// Inclusive lo, exclusive hi
constexpr inline const auto EachInRange(
    i64 x0, i64 x1,
    i64 z0, i64 z1) {
    using std::views::iota;
    using std::views::cartesian_product;

    return cartesian_product(iota(x0,x1), iota(z0,z1));
}
// both lo and hi are exclusive
constexpr inline const auto EachInRangeEx(
    i64 x0, i64 x1,
    i64 z0, i64 z1) {
    using std::views::iota;
    using std::views::cartesian_product;

    return cartesian_product(iota(x0+1,x1), iota(z0+1,z1));
}

constexpr inline const auto EachInRange(
    i64 x0, i64 x1){
    using std::views::iota;

    return iota(x0,x1);
}
template<typename IntVec3>
requires IVec3<IntVec3>
constexpr inline const auto EachInRange(IntVec3 min, IntVec3 max){
    using std::views::iota;
    using std::views::cartesian_product;

    return cartesian_product(iota(min.x,max.x), iota(min.y,max.y), iota(min.z,max.z));
}
template<typename IntVec2>
requires IVec2<IntVec2>
constexpr inline const auto EachInRange(IntVec2 min, IntVec2 max){
    using std::views::iota;
    using std::views::cartesian_product;

    return cartesian_product(iota(min.x,max.x), iota(min.y,max.y));
}

STD_HASH_SPECIALIZATION(glm::ivec3, obj,
    return (std::hash<i32>{}(obj.x) << 1) ^  // NOLINT
           (std::hash<i32>{}(obj.y) << 2) ^  // NOLINT
           (std::hash<i32>{}(obj.z));
)
STD_HASH_SPECIALIZATION(glm::vec3, obj,
    return (std::hash<f32>{}(obj.x) << 1) ^  // NOLINT
           (std::hash<f32>{}(obj.y) << 2) ^  // NOLINT
           (std::hash<f32>{}(obj.z));
)

