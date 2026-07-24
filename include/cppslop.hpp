#pragma once

#include "NumericConcepts.hpp"
#include "Types.h"

using size_t = std::size_t;
#define NO_MOVE(T)          \
T(T&&) = delete;            \
T& operator=(T&&) = delete;

#define NO_COPY(T)          \
T(const T&) = delete;            \
T& operator=(const T&) = delete;

// NOTE:
// I dont like how similar `default` and `delete` look when glazing over classes.
// I think an uppercase delete might help to remedy this.
#define DELETE delete

// i refuse to pollute 5 different classes with this bullshit
#define DECL_MOVE_ONLY(CLASS_NAME)                                                                 \
    CLASS_NAME(const CLASS_NAME&) = delete;                                                        \
    CLASS_NAME& operator=(const CLASS_NAME&) = delete;                                             \
    CLASS_NAME(CLASS_NAME&&) = default;                                                            \
    CLASS_NAME& operator=(CLASS_NAME&&) = default;

#define DECL_NO_COPY(CLASS_NAME)                                                                   \
    CLASS_NAME(const CLASS_NAME&) = delete;                                                        \
    CLASS_NAME& operator=(const CLASS_NAME&) = delete;


#define FORWARD_DECL_ENUM_STRUCT(NAME, UNDERLYING)                                                \
    enum class NAME : UNDERLYING;

#define FORWARD_DECL_ENUM_CLASS(NAMESPACE, NAME, UNDERLYING)                                       \
    enum class NAME : UNDERLYING;

#define FORWARD_DECL_TEMPLATE_STRUCT(NAME, ...)\
    template< __VA_ARGS__ >                 \
    struct NAME;

#define FORWARD_DECL_TEMPLATE_CLASS(NAME, ...)\
    template< __VA_ARGS__ >                 \
    class NAME;

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

// NOTE! This requires an equality operator to be defined as well.
#define STD_HASH_SPECIALIZATION(T, tName, fn_body)     \
namespace std{                                                      \
    template<>                                                      \
    struct hash<T>{                                                 \
        size_t operator()(const T& tName) const {              \
            fn_body\
        }                                                           \
    };                                                              \
}
