#include "Profiler.hpp"
#include <format>
#include <unordered_map>

std::unordered_map<std::string_view, val>    clock_map;
std::unordered_map<std::string_view, size_t> count_map;

ScopeTimer::TimePoint ScopeTimer::now() {
    return std::chrono::steady_clock::now(); 
}

ScopeTimer::ScopeTimer(std::string_view name, std::string_view unit_name) {
    timer_register(name, unit_name);
    start = now();
    this->name = name;
}

ScopeTimer::~ScopeTimer() {
    auto end = now();
    auto diff = end - start;
    timer_report(name, us_Duration(diff).count());
}
std::string ScopeTimer::summary() {
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

void timer_register(std::string_view name, std::string_view unit_name) {
    clock_map.insert({ name, { {}, unit_name } });
}

void timer_report(std::string_view name, double duration) {
    auto& [duration_us, unit_name] = clock_map.at(name);
    duration_us = duration;
}

void timer_log_ms_avg_us(std::string_view name, size_t count) {
    count_map.emplace(name, count);
    auto [duration_us, unit_name] = clock_map[name];
    auto duration_ms = duration_us / 1000.0;
    auto avg_us = duration_us / count;
    std::println("[TIMER] {} took {:0.1f}ms, {:0.2f}us per {}", name, duration_ms, avg_us, unit_name);
}
void timer_log_avg_us(std::string_view name, size_t count) {
    count_map.emplace(name, count);
    auto [duration_us, unit_name] = clock_map[name];
    auto avg_us = duration_us / count;
    std::println("[TIMER] {} took {:0.2f}us per {}", name, avg_us, unit_name);
}
void timer_log_ms(std::string_view name) {
    auto [duration_us, _] = clock_map[name];
    auto duration_ms = duration_us / 1000.0;
    std::println("[TIMER] {} took {:0.1f}ms", name, duration_ms);
}
