#pragma once
#include <print>
#if defined(__clang__)
    #define DEBUG_BREAKPOINT_QUIET() __builtin_debugtrap()
#elif defined(__GNUC__) || defined(__GNUG__)
    #define DEBUG_BREAKPOINT_QUIET() __builtin_trap()
#else
    #error "Unsupported compiler. Use gcc or clang noob"
#endif

#define DEBUG_BREAKPOINT()                                                                         \
    do {                                                                                           \
        std::println(                                                                              \
            stderr, "\e[31;1;4mTriggered BREAKPOINT in {}:{} !\n\e[0m\e[31;1mFunction -> {}\e[0m", \
            __FILE_NAME__, __LINE__, __PRETTY_FUNCTION__);                                         \
        DEBUG_BREAKPOINT_QUIET();                                                                  \
    } while (0)

