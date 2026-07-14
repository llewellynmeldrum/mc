#pragma once 
#include "Types.h"
#include <charconv>
#include <optional>
#include <string>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/sysctl.h>


// unix utilities
namespace unix {
    u64 inline get_file_size(const char* filename){
        struct stat st;
        lstat(filename, &st);
        return static_cast<u64>(st.st_size);
    }

    bool inline file_exists(const char* filename){
        return !access(filename, R_OK);
    }
    std::string inline exec(std::string sv) {
        char buffer[128]; // NOLINT
        std::string result = "";
        // Open pipe to read command output
        FILE* pipe = popen(sv.c_str(), "r");
        if (!pipe) return "ERROR";
        while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            result += buffer;
        }
        pclose(pipe);
        return result;
    }

    template<typename T>
    inline std::optional<T> get_env(std::string name) {
        const char* str = std::getenv(name.c_str());
        if (!str) return std::nullopt;

        if constexpr(std::same_as<T,std::string>){
            return std::make_optional(std::string(str));
        }else{
            T value{};
        
            std::string_view sv{str};

            auto [ptr, ec] = std::from_chars(sv.data(), sv.data() + sv.size(), value);

            if (ec != std::errc{} || ptr != sv.data() + sv.size() || value <= 0) {
                return std::nullopt;
            }

            return value;
        }
    }

    // WARNING:!!! 
    // This function is EXCEPTIONALLY slow. Like really really really slow. It literally has to 
    inline std::size_t term_cols() {
        auto cols = get_env<int>("COLUMNS");
        return cols ? *cols : 0;
    }
    inline std::size_t term_rows() {
        auto rows = get_env<int>("LINES");
        return rows ? *rows : 0;
    }
    // fixed in c++26 lol
    // NOTE: Didnt verify that this works with gdb, works with lldb tho
    inline bool is_debugger_present(){
        #if defined(__APPLE__)
            std::array<int,4> mib {CTL_KERN, KERN_PROC, KERN_PROC_PID, getpid()};
            struct kinfo_proc info{};
            size_t size = sizeof(info);
            sysctl(mib.data(), 4, &info, &size, nullptr, 0);
            return (info.kp_proc.p_flag & P_TRACED) != 0;

        #elif defined(__linux__)
            std::ifstream status_file("/proc/self/status");
            std::string line;
            while (std::getline(status_file, line)) {
                if (line.find("TracerPid:") == 0) {
                    int pid = std::stoi(line.substr(10));
                    return pid != 0;
                }
            }
            return false;

        #endif 
    }
    // find the resident set size (approximate memory usage of a process). 
    // aka the 'working set' on windows. Maybe one day I will support that disgusting API and OS
    inline std::size_t rss_bytes(){
        struct rusage usage;
        if (getrusage(RUSAGE_SELF, &usage) == 0) {
        #if defined(__APPLE__)
            // on macos, ru_maxrss is in BYTES (1B)
            std::size_t rb_bytes = usage.ru_maxrss;
        #elif defined(__linux__)
            // on linux, ru_maxrss is in KILOBYTES (1000B)
            std::size_t rb_bytes = usage.ru_maxrss / 1000.0;
        #endif
            return rb_bytes;
        } else {
            return 0;
        }
    }
}

