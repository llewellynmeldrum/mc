#pragma once
#define BRK_ASSERT(invariant) if (!invariant){ DEBUG_BREAKPOINT("FAILED ASSERTION: [" #invariant "]");}


#if defined(__clang__)
    #define TRAP() __builtin_debugtrap()
#elif defined(__GNUC__) || defined(__GNUG__)
    #define TRAP() __builtin_trap()
#else
    #error "Unsupported compiler. Use gcc or clang noob"
#endif

#if defined(IGNORE_BREAKPOINTS)
    #define DEBUG_BREAKPOINT_QUIET()                \
    fprintf(stderr,                                 \
            "WARNING! IGNORING BREAKPOINT_QUIET @ %s:%d"  \
            "TO RE-ENABLE, #undef `IGNORE_BREAKPOINTS`!",__FILE_NAME__, __LINE__);

    #define DEBUG_BREAKPOINT(msg)                   \
    fprintf(stderr,                                 \
            "WARNING! IGNORING BREAKPOINT @ %s:%d"  \
            "TO RE-ENABLE, #undef `IGNORE_BREAKPOINTS`!",__FILE_NAME__, __LINE__);
#else 
    #define DEBUG_BREAKPOINT_QUIET() TRAP()
#define DEBUG_BREAKPOINT(msg)                                                                         \
    do {                                                                                           \
        std::println(                                                                              \
            stderr, "\e[31;1;4m" msg " -> Triggered BREAKPOINT in {}:{} !\n\e[0m\e[31;1mFunction -> {}\e[0m", \
                     __FILE_NAME__, __LINE__, __PRETTY_FUNCTION__);                                         \
        DEBUG_BREAKPOINT_QUIET();                                                                  \
    } while (0)

#endif 

#if !defined(DEBUG_BREAKPOINT) || !defined(DEBUG_BREAKPOINT_QUIET) || !defined(TRAP)
    #error "hi"
#endif

