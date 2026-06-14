#pragma once 
#include "Types.h"
#include <string>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/ioctl.h>


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
    inline std::optional<T> get_env(const char* name) {
        const char* str = std::getenv(name);
        if (!str) return std::nullopt;

        T value{};
        std::string_view sv{str};

        auto [ptr, ec] = std::from_chars(sv.data(), sv.data() + sv.size(), value);

        if (ec != std::errc{} || ptr != sv.data() + sv.size() || value <= 0) {
            return std::nullopt;
        }

        return value;
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
}

