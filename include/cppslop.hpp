#pragma once

// i refuse to pollute 5 different classes with this bullshit
#define DECL_MOVE_ONLY(CLASS_NAME)                                                                 \
    CLASS_NAME(const CLASS_NAME&) = delete;                                                        \
    CLASS_NAME& operator=(const CLASS_NAME&) = delete;                                             \
    CLASS_NAME(CLASS_NAME&&) = default;                                                            \
    CLASS_NAME& operator=(CLASS_NAME&&) = default;

#define FORWARD_DECL_ENUM_STRUCT(NAMESPACE, NAME, UNDERLYING)                                      \
    namespace NAMESPACE {                                                                          \
    enum struct NAME : UNDERLYING;                                                                 \
    }

#define FORWARD_DECL_ENUM_CLASS(NAMESPACE, NAME, UNDERLYING)                                       \
    namespace NAMESPACE {                                                                          \
    enum class NAME : UNDERLYING;                                                                  \
    }

#define FORWARD_DECL_STRUCT(NAMESPACE, NAME)                                                       \
    namespace NAMESPACE {                                                                          \
    struct NAME;                                                                                   \
    }

#define FORWARD_DECL_CLASS(NAMESPACE, NAME)                                                        \
    namespace NAMESPACE {                                                                          \
    class NAME;                                                                                    \
    }
