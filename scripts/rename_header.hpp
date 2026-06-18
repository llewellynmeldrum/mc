#pragma once
// Pure, side-effect-free helpers used by the `rename-header` tool.
//
// These are split out from `rename-header.cpp` so they can be unit tested
// without dragging in `main`, the process-terminating `error()`, or the
// `<print>`-based logging (see test_rename_header.cpp).
#include <filesystem>
#include <fstream>
#include <string>

namespace fs = std::filesystem;

inline std::string read_file(const fs::path &p) {
    using namespace std;
    auto fstr = ifstream{p, ios::binary};
    fstr.exceptions(ios::badbit);
    auto buf = std::string(fs::file_size(p), '\0');
    fstr.read(buf.data(), buf.size());
    return buf;
}

inline decltype(auto) trim(std::string &&str) {
    const std::string whitespace = " \t\n\r\f\v";

    // Trim trailing space
    size_t end = str.find_last_not_of(whitespace);
    if (end != std::string::npos) {
        str.erase(end + 1);
    }

    // Trim leading space
    size_t start = str.find_first_not_of(whitespace);
    if (start != std::string::npos) {
        str.erase(0, start);
    } else {
        str.clear();  // String is entirely whitespace
    }
    return str;
}

inline void write_to_file(const fs::path &p, const std::string buf) {
    using namespace std;
    auto fstr = ofstream{p, ios::binary};
    fstr.exceptions(ios::badbit);
    fstr.write(buf.data(), buf.size());
}

inline std::string line_of(std::string_view str, std::size_t index) {
    using namespace std;
    auto prev_newl_idx = str.rfind("\n", index);
    auto next_newl_idx = str.find("\n", index);

    auto prev_newl = prev_newl_idx == string::npos
                         ? str.begin()  // assume start of file
                         : str.begin() + prev_newl_idx;
    auto next_newl = next_newl_idx == string::npos
                         ? str.end()  // assume end of file
                         : str.begin() + next_newl_idx;

    return trim(std::string(prev_newl, next_newl));
}
