#pragma once

#include "Types.h"

#include <format>
#include <chrono>
#include <format>
#include <string_view>
#include <unordered_map>
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

    inline TimePoint now() { return std::chrono::steady_clock::now(); }
    inline ScopeTimer(std::string_view name, std::string_view unit_name = "unit") {
        timer_register(name, unit_name);
        start = now();
        this->name = name;
    }
    inline ~ScopeTimer() {
        auto end = now();
        auto diff = end - start;
        timer_report(name, us_Duration(diff).count());
    }
    static inline std::string summary() {
        using sys_clock = std::chrono::system_clock;

        auto               now = sys_clock::now();
        time_t             t = sys_clock::to_time_t(now);
        std::ostringstream res{};
        res << "|||||||||||||||||||||||||||" << std::endl;
        res << "|        SUMMARY          |" << std::endl;
        res << "|||||||||||||||||||||||||||" << std::endl;
        res << std::format("{}", std::ctime(&t));
        for (const auto& [key, val] : clock_map) {
            const auto& us_elapsed = val.first;
            const auto  ms_elapsed = val.first / 1000.0;
            const auto& unit_name = val.second;
            res << std::format(
                "{:20} took {:<20} ({})\n", key, std::format("{:0.2f}ms, ", ms_elapsed),
                std::format("{:0.2f}us per {}", us_elapsed / count_map[key], unit_name));
        }
        return res.str();
    }
};
