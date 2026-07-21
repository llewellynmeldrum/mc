#pragma once

#include "DebugOptions.hpp"

#ifdef ENABLE_CPPTRACE
    #include "cpptrace/cpptrace.hpp"
#endif

#include <iostream>

#if defined(__clang__)
    #define TRAP() __builtin_debugtrap()
#elif defined(__GNUC__) || defined(__GNUG__)
    #define TRAP() __builtin_trap()
#else
    #error "Unsupported compiler. Use gcc or clang noob"
#endif

constexpr inline void PRINT_TRACE(bool color = true){
#ifdef ENABLE_CPPTRACE
        cpptrace::generate_trace().print(std::cerr, color); 
#endif
}
constexpr inline void PRINT_TRACE_SNIPPETS(bool color = true){
#ifdef ENABLE_CPPTRACE
    cpptrace::generate_trace().print_with_snippets(std::cerr, color); 
#endif
}

[[noreturn]]
constexpr inline void BREAKPOINT_QUIET(){
    TRAP();
    std::abort();
}
[[noreturn]]
constexpr inline void BREAKPOINT(size_t skip_frames=0){
#ifdef ENABLE_CPPTRACE
    cpptrace::generate_trace(skip_frames).print(std::cout, false); 
#else
    std::println(stderr,"No backtrace, cpptrace is disabled. Define ENABLE_CPPTRACE if u want them");
    std::println("No backtrace, cpptrace is disabled. Define ENABLE_CPPTRACE if u want them");
#endif
    BREAKPOINT_QUIET();
}
/* #define BREAKPOINT_QUIET() TRAP()
 #define BREAKPOINT(msg)                                                                         \
     do {                                                                                           \
         std::println(                                                                              \
             stderr, "\e[31;1;4m" msg " -> Triggered BREAKPOINT in {}:{} !\n\e[0m\e[31;1mFunction -> {}\e[0m", \
                      __FILE_NAME__, __LINE__, __PRETTY_FUNCTION__);                                         \
         BREAKPOINT_QUIET();                                                                  \
     } while (0)
 
 
 #if !defined(BREAKPOINT) || !defined(BREAKPOINT_QUIET) || !defined(TRAP)
     #error "hi"
 #endif
*/ 
