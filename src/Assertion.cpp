#include "AnsiCodes.hpp"
#include "Assertion.hpp"
#include "DEBUG.hpp"
#include "UnixHelpers.hpp"
using namespace std;
using namespace fmt;

// TEMP:
#define IGNORE_BREAKPOINTS

auto fn_fmt = style(ansi::bg_code_blocks, ansi::fg_rgb(141, 189, 251));
auto file_fmt = fg_rgb(0, 203, 0);
auto num_fmt = fg_rgb(180, 200, 200);
auto type_fmt = style(ansi::fg_rgb(243, 211, 152));
auto misc_fmt = style(ansi::fg_rgb(122, 218, 229));
auto intense_red_fmt = style(ansi::bold, ansi::fg_rgb(252, 148, 159));
auto red_fmt = style(ansi::fg_rgb(252, 148, 159));
auto iden_fmt = style(ansi::fg_rgb(252, 148, 159));
auto bg_code = style(ansi::bg_code_blocks);
auto val_fmt = style(ansi::fg_rgb(255, 165, 119));


std::string report_location(refl::source_location loc,std::string_view msg=""){
    return std::format(
        "{}{}{} {}{}{} {}{}{} {}{}{} {}{}{} {}{}{}\n",
            intense_red_fmt,    msg, reset,
            fn_fmt,             loc.pretty_fn(),                reset, 
            misc_fmt,           "->",                            reset , 
            file_fmt,           loc.file_name(),                reset , 
            misc_fmt,           ":",                            reset , 
            num_fmt,            loc.line(),                     reset
    );
}

auto banner_str(auto ch,  std::size_t n = unix::term_cols()){
    std::string s{"\n"};
    for (std::size_t i=0;i<n;i++){
        s+= ch;
    }
    return s+"\n";
};
std::string report_OOR(i64 cont_size, std::string_view err_msg, 
                       refl::variable cont_var, refl::variable key_var,  refl::source_location loc){
    using namespace std;
    using namespace fmt;
    auto line1 = report_location(loc,err_msg);
    std::string err_specific = {};
    std::string key_str = 
        std::format("{1} : {0} ({2})", 
                    styled(type_fmt, key_var.type()), 
                    styled(iden_fmt, key_var.name()),
                    styled(val_fmt, key_var.val())
                    );

    std::string cont_str = 
        std::format("{1} : {0}", styled(type_fmt, cont_var.type()), styled(iden_fmt, cont_var.name()));
    std::string range_str{};
    if (cont_size != -1){
        range_str = std::format("Range: (0,{}]",cont_size);
    }
    err_specific = std::format("Index {} is out of bounds for {}. {}", key_str, cont_str,range_str); 
    auto top_banner = intense_red_fmt + banner_str("▔") + reset;
    auto bot_banner = intense_red_fmt + banner_str("▁") + reset;
    println(stderr, "{}{}\t{}{}", 
            top_banner,
            line1,
            err_specific,
            bot_banner);
    DEBUG_BREAKPOINT_QUIET();
    return {};
}
std::string assert_failure(std::string_view name, refl::source_location loc,std::string_view note){
    using namespace std;
    using namespace fmt;

    auto fn_fmt = style(ansi::bg_code_blocks, ansi::fg_rgb(141, 189, 251));
    auto file_fmt = fg_rgb(0, 203, 0);
    auto num_fmt = fg_rgb(180, 200, 200);
    auto type_fmt = style(ansi::fg_rgb(243, 211, 152));
    auto misc_fmt = style(ansi::fg_rgb(122, 218, 229));
    auto intense_red_fmt = style(ansi::bold, ansi::fg_rgb(252, 148, 159));
    auto red_fmt = style(ansi::fg_rgb(252, 148, 159));
    auto iden_fmt = style(ansi::fg_rgb(252, 148, 159));
    auto bg_code = style(ansi::bg_code_blocks);
    auto val_fmt = style(ansi::fg_rgb(255, 165, 119));
    string line1 = format(
        "{}{}{} {}{}{} {}{}{} {}{}{} {}{}{} {}{}{}\n",
            intense_red_fmt,    "ASSERT FAIL in:", reset,
            fn_fmt,             loc.pretty_fn(),                reset, 
            misc_fmt,           "->",                            reset , 
            file_fmt,           loc.file_name(),                reset , 
            misc_fmt,           ":",                            reset , 
            num_fmt,            loc.line(),                     reset);
    auto line2 = format(
            "\t{}{}{} {}{}{} {}{}{} {}{}{} \n",
            bg_code+ type_fmt,"condition:",        reset_fg,
            iden_fmt, name,        reset,
                     misc_fmt, "<--",           reset_fg,
            underline + intense_red_fmt, "FALSE",  reset
    );
    auto line3 = format( "\tnote:'{}{}{}'",red_fmt,note,reset);
    println(stderr, "\n{}{}{}\n{}{}{}\n{}{}{}\n",
            intense_red_fmt,    banner_str("▔"), reset,
            line1,line2,line3,
            intense_red_fmt,banner_str("▁"), reset);
    DEBUG_BREAKPOINT_QUIET();
    return std::string(note);
}

void assert_failure(std::string_view comparator, refl::variable a, refl::variable b, refl::source_location loc){
    using namespace std;
    using namespace fmt;

    auto fn_fmt = style(ansi::bg_code_blocks, ansi::fg_rgb(141, 189, 251));
    auto file_fmt = fg_rgb(0, 203, 0);
    auto num_fmt = fg_rgb(180, 200, 200);
    auto type_fmt = style(ansi::fg_rgb(243, 211, 152));
    auto misc_fmt = style(ansi::fg_rgb(122, 218, 229));
    auto intense_red_fmt = style(ansi::bold, ansi::fg_rgb(252, 148, 159));
    auto iden_fmt = style(ansi::fg_rgb(252, 148, 159));
    auto bg_code = style(ansi::bg_code_blocks);
    auto val_fmt = style(ansi::fg_rgb(255, 165, 119));
    string line1 = format(
        "{}{}{} {}{}{} {}{}{} {}{}{} {}{}{} {}{}{}\n",
            intense_red_fmt,    "ASSERT FAIL in:", reset,
            fn_fmt,             loc.pretty_fn(),                reset, 
            misc_fmt,           "->",                            reset , 
            file_fmt,           loc.file_name(),                reset , 
            misc_fmt,           ":",                            reset , 
            num_fmt,            loc.line(),                     reset);
    auto line2 = format(
            "\t{}{}{}{}{}{} {}{}{} {}{}{} {}{}{} {}{}{}{}{}{} {}{}{} {}{}{}\n",
            reset_fg, "",               reset_fg, 
            bg_code+ type_fmt, "T",             reset_fg,
                     iden_fmt, a.name(),        reset_fg,
            strike+ misc_fmt, comparator,      reset_fg+no_strike,
                     type_fmt, "U",             reset_fg,
                     iden_fmt, b.name(),        reset_bg,
                     reset_fg, "",              reset_fg,
                     misc_fmt, "<--",           reset_fg,
            underline + intense_red_fmt, "FALSE",  reset
    );
    auto line3=format(
            "\t{}[with {}{}{}{}{}{}{}{}{}{}{}{} {}{}{}{}{}{}{}{}{}]{}\n",

            italic,
            type_fmt,"T",reset_fg,
                    misc_fmt,"=",reset_fg,
            type_fmt,a.type(), reset_bg,
                    num_fmt,",",reset_fg,

             type_fmt,"U",reset_fg,
                    misc_fmt,"=",reset_fg,
            type_fmt,b.type(), reset+italic,
            reset
    );
    auto line4 = format(
            "\t{}[and  {}{}{}{}{}{}{}{}{}{}{}{} {}{}{}{}{}{}{}{}{}]{}",

            italic,
            iden_fmt, a.name(),reset_fg,
                    misc_fmt,"=",reset_fg,
            reset,a.val(), reset_bg,
                    num_fmt,",",reset_fg,

            iden_fmt,b.name(),reset_fg,
                    misc_fmt,"=",reset_fg,
            reset,b.val(), reset+italic,
            reset
    );

    auto stripe_n_str = [](auto ch,  std::size_t n){
        std::string s{};
        for (std::size_t i=0;i<n;i++){
            s+= ch;
        }
        return s;
    };
    println(stderr, "\n{}{}{}\n{}{}{}{}\n{}{}{}\n",
            intense_red_fmt,    banner_str("▔"), reset,
            line1,line2,line3,line4,
            intense_red_fmt,banner_str("▁"), reset);
    DEBUG_BREAKPOINT_QUIET();
}
