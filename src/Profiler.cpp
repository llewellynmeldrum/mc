#include "Profiler.hpp"
#include <print>
#include <unordered_map>

std::unordered_map<std::string_view, val> clock_map;
using std::println;

void
timer_register(std::string_view name, std::string_view unit_name) {
    clock_map.insert({ name, { {}, unit_name } });
}

void
timer_report(std::string_view name, double duration) {
    auto& [duration_us, unit_name] = clock_map.at(name);
    duration_us = duration;
}

void
timer_log_avg_us(std::string_view name, size_t count) {
    auto [duration_us, unit_name] = clock_map[name];
    auto avg_us = duration_us / count;
    println("[TIMER] {} took {:0.1f}us, {:0.2f}us per {}", name, duration_us, avg_us, unit_name);
}
void
timer_log_ms(std::string_view name) {
    auto [duration_us, _] = clock_map[name];
    auto duration_ms = duration_us / 1000.0;
    println("[TIMER] {} took {:0.1f}ms", name, duration_ms);
}
