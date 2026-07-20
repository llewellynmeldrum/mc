
//#include "FormatSpecs.hpp"

#include "FmtStyle.hpp"
#include "Assertion.hpp"
#include "Breakpoints.hpp"
#include "UnixHelpers.hpp"

#include <cstdio>
#include <string>
#include <format>
#include <print>
#include <exception>

inline auto fn_fmt = fmt::fg_rgb(141, 189, 251);
inline auto file_fmt = fmt::fg_rgb(0, 203, 0);
inline auto num_fmt = fmt::fg_rgb(180, 200, 200);
inline auto misc_fmt = fmt::fg_rgb(122, 218, 229);
inline auto intense_red_fmt = fmt::style(fmt::ansi::bold, fmt::ansi::fg_rgb(252, 148, 159));
inline auto red_fmt = fmt::style(fmt::ansi::fg_rgb(252, 148, 159));
inline auto type_fmt = fmt::style(fmt::ansi::fg_rgb(243, 211, 152));
inline auto iden_fmt = fmt::style(fmt::ansi::fg_rgb(252, 148, 159));
inline auto val_fmt = fmt::style(fmt::ansi::fg_rgb(255, 165, 119));
inline auto bg_code = fmt::style(fmt::ansi::bg_code_blocks);


using namespace std;
std::string report_location(refl::source_location loc,std::string_view msg=""){
    return std::format( "{} {} {} {} {} {}\n",
           fmt::styled(intense_red_fmt,    msg),
           fmt::styled(fn_fmt,             loc.pretty_fn()),
           fmt::styled(misc_fmt,           "->"),
           fmt::styled(file_fmt,           loc.file_name()),
           fmt::styled(misc_fmt,           ":"),
           fmt::styled(num_fmt,            loc.line())
    );
}

auto banner_str(auto ch,  size_t n = unix::term_cols()){
    std::string s{"\n"};
    for (size_t i=0;i<n;i++){
        s+= ch;
    }
    return s+"\n";
};
void report_OOR(i64 cont_size, std::string_view err_msg, 
                       refl::variable cont_var, refl::variable key_var,  refl::source_location loc){
    using namespace std;
    std::cout.flush();
    std::cerr.flush();
    auto line1 = report_location(loc,err_msg);
    std::string err_specific = {};
    std::string key_str = 
        std::format("{1} : {0} ({2})", 
                    fmt::styled(type_fmt, key_var.type()), 
                    fmt::styled(iden_fmt, key_var.name()),
                    fmt::styled(val_fmt, key_var.val())
                    );

    std::string cont_str = 
        std::format("{1} : {0}", fmt::styled(type_fmt, cont_var.type()), fmt::styled(iden_fmt, cont_var.name()));
    std::string range_str{};
    if (cont_size != -1){
        range_str = std::format("Range: (0,{}]",cont_size);
    }
    err_specific = std::format("Index {} is out of bounds for {}. {}", key_str, cont_str,range_str); 
    auto top_banner = fmt::styled(intense_red_fmt , banner_str("▔"));
    auto bot_banner = fmt::styled(intense_red_fmt , banner_str("▁"));
    println(stderr, "{}{}\t{}{}", 
            top_banner,
            line1,
            err_specific,
            bot_banner);
    BREAKPOINT(3);
    std::abort();
}
void assert_failure(std::string_view name, refl::source_location loc,std::string_view note){
    using namespace std;
    std::cout.flush();
    std::cerr.flush();

    string line1 = format(
        "{} {} {} {} {} {}\n",
            fmt::styled(intense_red_fmt,    "ASSERT FAIL in:"),
            fmt::styled(fn_fmt,             loc.pretty_fn()),
            fmt::styled(misc_fmt,           "->"),
            fmt::styled(file_fmt,           loc.file_name()),
            fmt::styled(misc_fmt,           ":"),
            fmt::styled(num_fmt,            loc.line())
    );
    auto line2 = format(
            "\t{} {} {} {} \n",
            fmt::styled_fg(fmt::bg_code()+ type_fmt,"condition:"),
            fmt::styled(iden_fmt, name),
            fmt::styled_fg(         misc_fmt, "<--"),
            fmt::styled(fmt::underline() + intense_red_fmt, "FALSE")
    );
    auto line3 = format("\tnote:'{}'",fmt::styled(red_fmt,note));
    println(stderr, "\n{}\n{}{}{}\n{}\n",
            fmt::styled(intense_red_fmt, banner_str("▔")),
            line1,line2,line3,
            fmt::styled(intense_red_fmt,banner_str("▁"))
    );
    BREAKPOINT();
    std::abort();
}

void assert_failure(std::string_view comparator, refl::variable a, refl::variable b, refl::source_location loc){
    std::cout.flush();
    std::cerr.flush();
    using namespace std;

    string line1 = format(
        "{} {} {} {} {} {}\n",
            fmt::styled(intense_red_fmt, "ASSERT FAIL in:"),
                     fmt::styled(fn_fmt,   loc.pretty_fn()),
                   fmt::styled(misc_fmt,              "->"),
                   fmt::styled(file_fmt,   loc.file_name()),
                   fmt::styled(misc_fmt,               ":"),
                    fmt::styled(num_fmt,        loc.line())
    );
    auto line2 = format( "\t{}{} {} {} {} {}{} {} {}\n",
                          fmt::styled_fg(fmt::reset_fg(),         ""),
                         fmt::styled_fg(fmt::bg_code()+type_fmt,        "T"),
                                 fmt::styled_fg(iden_fmt,   a.name()),
                   fmt::styled_fg(fmt::strike()+misc_fmt, comparator),
                                 fmt::styled_fg(type_fmt,        "U"),
                                 fmt::styled_bg(iden_fmt,   b.name()),
                          fmt::styled_fg(fmt::reset_fg(),         ""),
                                 fmt::styled_fg(misc_fmt,      "<--"),
            fmt::styled(fmt::underline()+intense_red_fmt,    "FALSE")
    );
    auto line3=format(
            "\t{}[with {}{}{}{} {}{}{}]{}\n",

                      fmt::italic(),
            fmt::styled_fg(type_fmt,      "T"),
            fmt::styled_fg(misc_fmt,      "="),
            fmt::styled_bg(type_fmt, a.type()),
             fmt::styled_fg(num_fmt,      ","),

            fmt::styled_fg(type_fmt,      "U"),
            fmt::styled_fg(misc_fmt,      "="),
               fmt::styled(type_fmt, b.type()),
                   fmt::no_italic()
    );
    auto line4 = format(
            "\t{}[and  {}{}{}{} {}{}{}]{}",

            fmt::italic(),
            fmt::styled_fg(iden_fmt, a.name()),
            fmt::styled_fg(        misc_fmt,"="),
            fmt::styled_bg(fmt::reset(),a.val()),
            fmt::styled_fg(        num_fmt,","),

            fmt::styled_fg(iden_fmt,b.name()),
            fmt::styled_fg(misc_fmt,"="),
            fmt::styled_fg(fmt::reset(), b.val()),
            fmt::no_italic()
    );

    auto stripe_n_str = [](auto ch,  size_t n){
        std::string s{};
        for (size_t i=0;i<n;i++){
            s+= ch;
        }
        return s;
    };
    println(stderr, "\n{}\n{}{}{}{}\n{}\n",
            fmt::styled(intense_red_fmt,    banner_str("▔")),
            line1,line2,line3,line4,
            fmt::styled(intense_red_fmt,banner_str("▁"))
    );
    BREAKPOINT();
    std::terminate();
}
