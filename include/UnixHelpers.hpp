#pragma once 
#include "Types.h"
#include <string>
#include <unistd.h>
#include <sys/stat.h>

// unix utilities
namespace unix {
    u64 get_file_size(const char* filename){
        struct stat st;
        lstat(filename, &st);
        return static_cast<u64>(st.st_size);
    }

    bool file_exists(const char* filename){
        return !access(filename, R_OK);
    }
    std::string exec(std::string sv) {
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
}
