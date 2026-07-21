#pragma once

#include <mutex>
#define INCLUDE_LOGGER_LAST
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
#include "FmtStyle.hpp"
#include "DebugFormat.hpp"
extern i64 program_epoch_ns;  // TODO: Must be defined in main file! do get_current_ns() as soon as
                              // main begins.

static inline double get_current_ns() {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return (i64)stons(ts.tv_sec) + ts.tv_nsec;
}
static inline double get_current_us() {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return (f64)(stons(ts.tv_sec) + ts.tv_nsec) / 1000.0;
}

static inline i64 ms_since_start() {
    i64    current_ns = get_current_ns();
    i64    ns_elapsed = current_ns - program_epoch_ns;
    double ms = nstoms(ns_elapsed);
    return ms;
}

struct LogLevel {
    const char* prefix;
    std::string color;
    i32         precedence;
};

inline LogLevel DEBUG{ "[DEBUG]", fmt::fg_cyan(), -1 };
inline LogLevel INFO{ "[INFO]", fmt::fg_yellow(), 0 };
inline LogLevel NOTICE{ "[NOTICE]", fmt::fg_pink(), 1 };
inline LogLevel WARN{ "[WARN]", fmt::fg_br_red(), 2 };
inline LogLevel ERROR{ "[ERROR]", fmt::fg_red(), 3 };
inline LogLevel FATAL{ "[FATAL]", fmt::fg_green(), 4 };
inline LogLevel COUNT{ "[COUNT]", fmt::fg_red(), 5 };

extern std::mutex log_mut;
#define LOG_LVL(lvl, file, ln, fmt_str, ...)                                                       \
    do {                                                                                           \
        std::unique_lock<std::mutex> lock(log_mut);                                                \
        std::print("{:03.3f} ", ms_since_start() / 1000.0);                                        \
        std::print("{}{:<8}{} ", lvl.color, lvl.prefix, fmt::reset());                               \
        std::print("{}{}:{:<3}{} ", fmt::bold(), file, ln, fmt::reset());                              \
        std::println(fmt_str, ##__VA_ARGS__);                                                      \
    } while (0)

#define LOG_LVL_NOLOCK(lvl, file, ln, fmt_str, ...)                                                       \
    do {                                                                                           \
        std::print("{:03.3f} ", ms_since_start() / 1000.0);                                        \
        std::print("{}{:<8}{} ", lvl.color, lvl.prefix, fmt::reset());                               \
        std::print("{}{}:{:<3}{} ", fmt::bold(), file, ln, fmt::reset());                              \
        std::println(fmt_str, ##__VA_ARGS__);                                                      \
    } while (0)

#define LOG_DEBUG(fmt, ...) LOG_LVL(DEBUG, __FILE_NAME__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) LOG_LVL(INFO, __FILE_NAME__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_NOTICE(fmt, ...) LOG_LVL(NOTICE, __FILE_NAME__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...) LOG_LVL(WARN, __FILE_NAME__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) LOG_LVL(ERROR, __FILE_NAME__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_FATAL(fmt, ...) LOG_LVL(FATAL, __FILE_NAME__, __LINE__, fmt, ##__VA_ARGS__)

#define LOG_DEBUG_NOLOCK(fmt, ...) LOG_LVL(DEBUG, __FILE_NAME__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_INFO_NOLOCK(fmt, ...) LOG_LVL(INFO, __FILE_NAME__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_NOTICE_NOLOCK(fmt, ...) LOG_LVL(NOTICE, __FILE_NAME__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_WARN_NOLOCK(fmt, ...) LOG_LVL(WARN, __FILE_NAME__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_ERROR_NOLOCK(fmt, ...) LOG_LVL(ERROR, __FILE_NAME__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_FATAL_NOLOCK(fmt, ...) LOG_LVL(FATAL, __FILE_NAME__, __LINE__, fmt, ##__VA_ARGS__)

#define LOG_EXIT(code)                                                                             \
    do {                                                                                           \
        if (code == EXIT_SUCCESS)                                                                  \
            LOG_LVL(NOTICE, __FILE_NAME__, __LINE__, "Exiting with code {}.", code);               \
        else                                                                                       \
            LOG_LVL(FATAL, __FILE_NAME__, __LINE__, "Exiting with code {}.", code);                \
        exit(code);                                                                                \
    } while (0)

template <typename T>
constexpr std::string fmt_expr(const char* identifier, T&& expr) {
    using Arg = std::remove_reference_t<T>;
    std::string expr_str{};
    expr_str = std::format("{}",expr);
    return std::format("{}{:>12}{} "
                       "{}{:<12}{} "
                       " = {:<12};",
                       fmt::fg_cyan(), pretty_type_name<Arg>(), fmt::reset(), fmt::fg_red(), identifier,
                       fmt::reset(), expr_str);
}

// @brief returns a string containing the underlying type of the object expr represents. No value
// categories.
template <typename T>
std::string fmt_obj(const char* identifier, T&& expr) {
    using Arg = std::remove_reference_t<T>;

    if constexpr (Formattable<T>) {
        return std::format("{} {} = {}", pretty_type_name<Arg>(), identifier, expr);
    } else {
        return std::format("{} {} = <unformattable tomato>", pretty_type_name<Arg>(), identifier);
    }
}

#define LOG_EXPR(expr) std::println(stderr,"{}:{} -> {}{} {} = {}",\
        __FILE_NAME__, __LINE__, \
        fmt::bg_code(),\
        fmt::styled_fg(fmt::fg_type(), pretty_type_name<decltype(expr)>()),\
        fmt::styled_fg(fmt::fg_identifier(),#expr),\
        fmt::styled(fmt::reset(), expr)\
        )










