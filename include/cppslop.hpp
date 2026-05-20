#pragma once

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

constexpr inline const auto EachInRange(
    i64 x0, i64 x1,
    i64 y0, i64 y1,
    i64 z0, i64 z1) {
    using std::views::iota;
    using std::views::cartesian_product;

    return cartesian_product(iota(x0,x1), iota(y0,y1), iota(z0,z1));
}

constexpr inline const auto EachInRange(
    i64 x0, i64 x1,
    i64 z0, i64 z1) {
    using std::views::iota;
    using std::views::cartesian_product;

    return cartesian_product(iota(x0,x1), iota(z0,z1));
}

constexpr inline const auto EachInRange(
    i64 x0, i64 x1){
    using std::views::iota;

    return iota(x0,x1);
}
