#include <cstdint>

#include "Profiling.hpp"
#include "Types.h"
#include <fstream>
#include <string>
#include <sstream>
#include <cstdint>

std::uint64_t current_rss_bytes_linux() {
    std::ifstream file("/proc/self/status");
    std::string key;
    u64 value_kb;
    std::string unit;

    while (file >> key >> value_kb >> unit) {
        if (key == "VmRSS:") {
            return value_kb * 1024;
        }
    }

    return 0; 
}

#include <mach/mach.h>
#include <cstdint>

std::uint64_t current_rss_bytes_macos() {
    mach_task_basic_info info;
    mach_msg_type_number_t count = MACH_TASK_BASIC_INFO_COUNT;

    kern_return_t result = task_info(
        mach_task_self(),
        MACH_TASK_BASIC_INFO,
        reinterpret_cast<task_info_t>(&info),
        &count
    );

    if (result != KERN_SUCCESS) {
        return 0;
    }

    return static_cast<std::uint64_t>(info.resident_size);
}

u64 current_rss_bytes() {
#if defined(__linux__)
    return current_rss_bytes_linux();
#elif defined(__APPLE__)
    return current_rss_bytes_macos();
#else
    return 0;
#endif
}
