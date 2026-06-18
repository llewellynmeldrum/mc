#include "rename_header.hpp"

#include <cassert>
#include <filesystem>
#include <print>
#include <string>

namespace fs = std::filesystem;

#define help_fmt                                                               \
    "Error! {} Usage: \n"                                                      \
    "       rename-header <from> <to>"

template <typename... _Args>
void error(std::format_string<_Args...> __fmt, _Args &&...__args) {
    using namespace std;
    println(stderr, "\e[31m{}", format(__fmt, std::forward<_Args>(__args)...));
    const fs::path cwd = fs::current_path();
    println(stderr, "\tcwd: '{}'", cwd.string());
    println("\e[0m");
    std::exit(1);
}

bool rename_file(fs::path path_from, fs::path path_to) {
    if (fs::is_directory(path_from)) {
        error("{} is a directory! cannot rename.",
              path_from.filename().string());
        return false;
    }
    if (!fs::exists(path_from)) {
        error("src file 'include/{}' does not exist! cannot rename.",
              path_from.filename().string());
        return false;
    }
    if (fs::exists(path_to)) {
        error("dst file 'include/{}' already exists! cannot rename.",
              path_from.filename().string());
        return false;
    }
    println("Renaming {} to {}.", path_from.filename().string(),
            path_to.filename().string());
    fs::rename(path_from, path_to);

    return true;
};

int main(int argc, char **argv) {
    using namespace std;
    fs::path src{};
    fs::path dst{};
    const fs::path cwd = filesystem::current_path();
    if (argc == 1) {
        error(help_fmt, "No NAME param provided.");
    } else if (argc == 3) {
        src = cwd / fs::path{"include"} / fs::path{argv[1]};
        dst = cwd / fs::path{"include"} / fs::path{argv[2]};
    } else {
        error(help_fmt, "Incorrect number of arguments.");
    }
    const std::string src_str = src.filename().string();
    const std::string dst_str = dst.filename().string();
    rename_file(src, dst);
    assert(!dst_str.contains(src_str));
    // avoid recursive loop i.e
    // from=xyz to=xyzabc
    // xyz -> xyzabc -> xyzabcxyzabc ...

    println("cwd: {}", cwd.string());
    for (const auto &dir : fs::directory_iterator(cwd)) {
        if (!fs::is_directory(dir)) {
            continue;
        }
        if (dir.path().filename().string() == "include" ||
            dir.path().filename().string() == "src") {
            println("path: {}", dir.path().string());
            for (const auto &file : fs::directory_iterator(dir)) {
                // open the file

                if (file.is_directory()) {
                    error("Project contains directory within the ./{} "
                          "directory. This should be a file.",
                          file.path().filename().string());
                }
                auto file_contents = read_file(file.path());
                if (file_contents.contains(src_str)) {
                    std::println("In file '{}':",
                                 file.path().filename().string());
                }
                while (file_contents.contains(src_str)) {
                    auto index = file_contents.find(src_str);
                    std::print("Changed line:\n'{}'",
                               trim(line_of(file_contents, index)));
                    auto start = file_contents.begin() + index;
                    auto end = start + src_str.length();
                    file_contents.replace(start, end, dst_str);
                    std::println("\t-->\t'{}'", line_of(file_contents, index));
                }
                write_to_file(file.path(), file_contents);
            }
        }
    }
}
