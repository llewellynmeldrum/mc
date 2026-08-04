#pragma once

#include "CommonConcepts.hpp"
#include "DebugOptions.hpp"

#include <mutex>
#include <thread>

#define LIBASSERT_USE_MAGIC_ENUM
#include "libassert/assert.hpp"
#include "cpptrace/cpptrace.hpp"
#include "cpptrace/formatting.hpp"

#include <iostream>
namespace TraceSettings{
    inline std::mutex mut;
    inline bool enable_color {true};
    inline cpptrace::formatter fmt{};
    template<typename Fn>
        requires callable_with<Fn, cpptrace::formatter&>
    inline void modify_fmt(Fn&& fn){
        auto lock = std::lock_guard(mut);
        std::invoke(fn,fmt);
    }
    void init();
};

#if defined(__clang__)
    #define TRAP() __builtin_debugtrap()
#elif defined(__GNUC__) || defined(__GNUG__)
    #define TRAP() __builtin_trap()
#else
    #error "Unsupported compiler. Use gcc or clang noob"
#endif

constexpr inline void PRINT_TRACE(size_t skip_frames){
    auto lock = std::lock_guard(TraceSettings::mut);
    auto trace = cpptrace::generate_trace(skip_frames);
    TraceSettings::fmt.print(trace);
    std::cerr.flush();
}

[[noreturn]] constexpr inline void BREAKPOINT_QUIET(){
    TRAP();
    std::abort();
}
[[noreturn]] constexpr inline void BREAKPOINT(){
#ifdef ENABLE_CPPTRACE
    PRINT_TRACE(2);
#else
    std::println(stderr,"No backtrace, cpptrace is disabled. Define ENABLE_CPPTRACE if u want them");
    std::println("No backtrace, cpptrace is disabled. Define ENABLE_CPPTRACE if u want them");
#endif
    BREAKPOINT_QUIET();
}
