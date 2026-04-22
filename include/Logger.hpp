#pragma once

#include <cxxabi.h>
#include <memory>
#include <string>
#include <sstream>
#include <concepts>
#include <format>
#include <print>
#include <string>
#include <string_view>
#include <type_traits>

#include "Types.h"
#include "CommonUtils.hpp"
#include "CommonConcepts.hpp"
#include "AnsiCodes.hpp"
#include "DebugFormat.hpp"
extern u64 program_epoch_ns;  // TODO: Must be defined in main file! do get_current_ns() as soon as
                              // main begins.


static inline double get_current_ns() {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return (u64)stons(ts.tv_sec) + ts.tv_nsec;
}

static inline u64 ms_since_start() {
    u64    current_ns = get_current_ns();
    u64    ns_elapsed = current_ns - program_epoch_ns;
    double ms = nstoms(ns_elapsed);
    return ms;
}


struct LogLevel{
    const char* prefix;
    const char* color;
    i32 precedence;
};

inline LogLevel DEBUG{"[DEBUG]",       fmt::cyan,  -1};
inline LogLevel INFO{"[INFO]",         fmt::yellow,0};
inline LogLevel NOTICE{"[NOTICE]",     fmt::pink,  1};
inline LogLevel WARN{"[WARN]",         fmt::lired, 2};
inline LogLevel ERROR{"[ERROR]",       fmt::red,   3};
inline LogLevel FATAL{"[FATAL]",       fmt::green, 4};
inline LogLevel COUNT{"[COUNT]",       fmt::red,   5};



#define LOG_LVL(lvl,file, ln, fmt_str, ...)do{                          \
    std::print("{:03.3f} ", ms_since_start()/1000.0);                \
    std::print("{}{:<8}{} ", lvl.color, lvl.prefix, fmt::clear);       \
    std::print("{}{}:{:<3}{} ", fmt::bold, file, ln, fmt::clear);      \
    std::println(fmt_str, ##__VA_ARGS__);                               \
}while(0)

#define LOG_DEBUG(fmt, ...) LOG_LVL(DEBUG, __FILE_NAME__,__LINE__ ,fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) LOG_LVL(INFO, __FILE_NAME__,__LINE__ ,fmt, ##__VA_ARGS__)
#define LOG_NOTICE(fmt, ...) LOG_LVL(NOTICE, __FILE_NAME__,__LINE__ ,fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...) LOG_LVL(WARN, __FILE_NAME__,__LINE__ ,fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) LOG_LVL(ERROR, __FILE_NAME__,__LINE__ ,fmt, ##__VA_ARGS__)
#define LOG_FATAL(fmt, ...) LOG_LVL(FATAL, __FILE_NAME__,__LINE__ ,fmt, ##__VA_ARGS__)

#define LOG_EXIT(code) do{\
    LOG_LVL(FATAL, __FILE_NAME__,__LINE__ ,"Exiting with code {}.",code);\
    exit(code);\
}while(0)


template <typename T>
constexpr std::string_view pretty_type_name() {
#if defined(__clang__)
    std::string_view p = __PRETTY_FUNCTION__;
    auto start = p.find("T = ");
    start += 4;
    auto end = p.rfind(']');
    return p.substr(start, end - start);

#elif defined(__GNUC__)
    std::string_view p = __PRETTY_FUNCTION__;
    auto start = p.find("with T = ");
    start += 9;
    auto end = p.find(';', start);
    return p.substr(start, end - start);

#else
    return "unsupported compiler";
#endif
}

template <typename T>
constexpr std::string fmt_expr(const char* identifier, T&& expr) {
    using Arg = std::remove_reference_t<T>;
    std::string expr_str{};

    if constexpr(isSequenceContainer<T>){
        expr_str.append("\n{\n");
        for (const auto& e: expr){
            expr_str.append("\t");
            expr_str.append(dbg_fmt(e));
            if(&e != &expr.back()){
                expr_str.append(",\n");
            }
        }
        expr_str.append("\n}");
    }else{
        expr_str = dbg_fmt(expr);
    }
    return std::format("{}{:>12}{} "
                       "{}{:<12}{} "
                       " = {:<12};",
                       fmt::cyan, pretty_type_name<Arg>(),fmt::clear,
                       fmt::red, identifier, fmt::clear,
                       expr_str);
}

// @brief returns a string containing the underlying type of the object expr represents. No value categories.
template <typename T>
std::string fmt_obj(const char* identifier, T&& expr) {
    using Arg = std::remove_reference_t<T>;

    if constexpr (Formattable<T>) {
        return std::format("{} {} = {}", pretty_type_name<Arg>(), identifier, expr);
    } else {
        return std::format("{} {} = <unformattable tomato>", pretty_type_name<Arg>(), identifier);
    }
}

#define LOG_EXPR(expr) std::println("{}:{} -> {}", __FILE_NAME__, __LINE__, fmt_expr(#expr, (expr)))



