#pragma once

#include "cpptrace/cpptrace.hpp"

#include <iostream>

#if defined(__clang__)
    #define TRAP() __builtin_debugtrap()
#elif defined(__GNUC__) || defined(__GNUG__)
    #define TRAP() __builtin_trap()
#else
    #error "Unsupported compiler. Use gcc or clang noob"
#endif

constexpr inline void PRINT_TRACE(bool color = true){
    cpptrace::generate_trace().print(std::cerr, color); 
}
constexpr inline void PRINT_TRACE_SNIPPETS(bool color = true){
    cpptrace::generate_trace().print_with_snippets(std::cerr, color); 
}

[[noreturn]]
constexpr inline void BREAKPOINT_QUIET(){
    TRAP();
    std::abort();
}
[[noreturn]]
constexpr inline void BREAKPOINT(std::size_t skip_frames=0){
    cpptrace::generate_trace(skip_frames).print(std::cerr, false); 
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
