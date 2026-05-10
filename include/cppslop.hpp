#pragma once

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
