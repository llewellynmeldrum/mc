// Unit tests for the pure helpers of the `rename-header` tool.
//
// These cover trim(), line_of(), read_file() and write_to_file(). The tool's
// remaining surface (error(), rename_file(), main()) is omitted on purpose:
// error() / the failure paths of rename_file() call std::exit(1) and write to
// the terminal, so they are integration-level concerns rather than units.
//
// Build & run (no <print>, so a stock C++23 libstdc++ is enough):
//     c++ -std=c++23 scripts/test_rename_header.cpp -o /tmp/test_rename_header
//     /tmp/test_rename_header
#include "rename_header.hpp"

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <string>

// --- tiny test harness ------------------------------------------------------
static int g_checks = 0;
static int g_failures = 0;

#define CHECK(cond)                                                            \
    do {                                                                       \
        ++g_checks;                                                            \
        if (!(cond)) {                                                         \
            ++g_failures;                                                      \
            std::cerr << "FAIL " << __FILE__ << ":" << __LINE__ << "  " #cond  \
                      << "\n";                                                 \
        }                                                                      \
    } while (0)

#define CHECK_EQ(actual, expected)                                             \
    do {                                                                       \
        ++g_checks;                                                            \
        const auto _a = (actual);                                              \
        const auto _e = (expected);                                            \
        if (!(_a == _e)) {                                                     \
            ++g_failures;                                                      \
            std::cerr << "FAIL " << __FILE__ << ":" << __LINE__ << "  " #actual \
                      << " == " #expected << "\n   actual   = [" << _a         \
                      << "]\n   expected = [" << _e << "]\n";                  \
        }                                                                      \
    } while (0)

// trim takes std::string&& and hands back a reference into it, so capture the
// result into an owning std::string before the temporary argument dies.
static std::string do_trim(std::string s) { return trim(std::move(s)); }

// --- trim() -----------------------------------------------------------------
static void test_trim() {
    CHECK_EQ(do_trim("hello"), std::string{"hello"});         // no-op
    CHECK_EQ(do_trim("  hello"), std::string{"hello"});       // leading
    CHECK_EQ(do_trim("hello   "), std::string{"hello"});      // trailing
    CHECK_EQ(do_trim("   hello   "), std::string{"hello"});   // both
    CHECK_EQ(do_trim(""), std::string{""});                   // empty
    CHECK_EQ(do_trim("    "), std::string{""});               // all spaces
    CHECK_EQ(do_trim("\t\n\r\f\v "), std::string{""});        // all ws kinds
    CHECK_EQ(do_trim("\t \nhi\r\n "), std::string{"hi"});     // mixed ws
    // internal whitespace must be preserved
    CHECK_EQ(do_trim("  a b\tc  "), std::string{"a b\tc"});
    // a single non-whitespace char surrounded by whitespace
    CHECK_EQ(do_trim(" x "), std::string{"x"});
}

// --- line_of() --------------------------------------------------------------
static void test_line_of() {
    // single line, no newlines: whole (trimmed) string regardless of index
    CHECK_EQ(line_of("hello world", 0), std::string{"hello world"});
    CHECK_EQ(line_of("hello world", 5), std::string{"hello world"});

    const std::string doc = "first\nsecond\nthird";
    //                       0123456 789...
    CHECK_EQ(line_of(doc, 0), std::string{"first"});   // in first line
    CHECK_EQ(line_of(doc, 2), std::string{"first"});   // still first line
    CHECK_EQ(line_of(doc, 7), std::string{"second"});  // middle line
    CHECK_EQ(line_of(doc, 14), std::string{"third"});  // last line

    // returned line is trimmed of surrounding whitespace
    CHECK_EQ(line_of("a\n   spaced   \nb", 6), std::string{"spaced"});

    // a #include-style line, the tool's actual use case
    const std::string src = "#pragma once\n#include \"Old.hpp\"\nint x;";
    const auto idx = src.find("Old.hpp");
    CHECK(idx != std::string::npos);
    CHECK_EQ(line_of(src, idx), std::string{"#include \"Old.hpp\""});
}

// --- read_file() / write_to_file() round trip -------------------------------
static fs::path make_temp_path() {
    static int counter = 0;
    const auto stamp =
        std::chrono::steady_clock::now().time_since_epoch().count();
    return fs::temp_directory_path() /
           ("rename_header_test_" + std::to_string(stamp) + "_" +
            std::to_string(counter++) + ".tmp");
}

static void test_file_round_trip() {
    {  // ordinary multi-line text
        const auto p = make_temp_path();
        const std::string content = "line one\nline two\n#include \"X.hpp\"\n";
        write_to_file(p, content);
        CHECK(fs::exists(p));
        CHECK_EQ(fs::file_size(p), content.size());
        CHECK_EQ(read_file(p), content);
        fs::remove(p);
    }
    {  // empty file
        const auto p = make_temp_path();
        write_to_file(p, "");
        CHECK(fs::exists(p));
        CHECK_EQ(fs::file_size(p), std::uintmax_t{0});
        CHECK_EQ(read_file(p), std::string{""});
        fs::remove(p);
    }
    {  // binary-ish content with embedded NUL must survive (binary mode)
        const auto p = make_temp_path();
        const std::string content("a\0b\0c", 5);
        write_to_file(p, content);
        CHECK_EQ(fs::file_size(p), std::uintmax_t{5});
        const auto back = read_file(p);
        CHECK_EQ(back.size(), std::size_t{5});
        CHECK(back == content);
        fs::remove(p);
    }
    {  // overwrite replaces, not appends
        const auto p = make_temp_path();
        write_to_file(p, "the longer original contents");
        write_to_file(p, "short");
        CHECK_EQ(read_file(p), std::string{"short"});
        fs::remove(p);
    }
}

int main() {
    test_trim();
    test_line_of();
    test_file_round_trip();

    if (g_failures == 0) {
        std::cout << "All " << g_checks << " checks passed.\n";
        return EXIT_SUCCESS;
    }
    std::cerr << g_failures << " of " << g_checks << " checks FAILED.\n";
    return EXIT_FAILURE;
}
