#pragma once

#include "Types.h"

#include <string_view>
#include <unordered_map>
#include <chrono>
using us_Duration = std::chrono::duration<double, std::micro>;
using val = std::pair<double, std::string_view>;

extern std::unordered_map<std::string_view, val>    clock_map;
extern std::unordered_map<std::string_view, size_t> count_map;

void timer_register(std::string_view name, std::string_view unit_name);
void timer_report(std::string_view name, double duration_us);
void timer_log_avg_us(std::string_view name, size_t count);
void timer_log_ms(std::string_view name);
void timer_log_ms_avg_us(std::string_view name, size_t count);

struct ScopeTimer {
    using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;

    TimePoint        start;
    std::string_view name;

    TimePoint now();
    ScopeTimer(std::string_view name, std::string_view unit_name = "unit");
    ~ScopeTimer();
    static std::string summary();
};
