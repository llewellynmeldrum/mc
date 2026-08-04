#include "Breakpoints.hpp"
#define LIBASSERT_USE_FMT
#define LIBASSERT_USE_MAGIC_ENUM
#include "libassert/assert.hpp"
namespace TraceSettings{
    void init(){
        using namespace cpptrace;
        auto lock = std::lock_guard(mut);
        fmt
            .header("Stack trace (most recent call first):")
            .colors(formatter::color_mode::always)
            .paths(formatter::path_mode::basename)
            .addresses(formatter::address_mode::none)
            .snippets(false)
            .snippet_context(0)
            .columns(false)
            .symbols(formatter::symbol_mode::pretty)
            .filtered_frame_placeholders(false)
            .hide_exception_machinery(true);
            ;
        libassert::set_diff_highlighting(true);
//    libassert::assertion_info::action();
    }
};
