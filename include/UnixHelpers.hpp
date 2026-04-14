#pragma once 
#include "Types.h"
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
}
