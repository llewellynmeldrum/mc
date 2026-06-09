#include "AnsiCodes.hpp"
#include "Assertion.hpp"
#include "DEBUG.hpp"
void assert_failure(std::string_view name, LM::source_location loc,std::string_view note){
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
            intense_red_fmt,    "[ERROR!]:Assertion failed in", reset,
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
    auto stripe_n_str = [](auto ch,  std::size_t n){
        std::string s{};
        for (std::size_t i=0;i<n;i++){
            s+= ch;
        }
        return s;
    };
    println(stderr, "\n{}{}{}\n{}{}{}\n{}{}{}\n",intense_red_fmt,    stripe_n_str("▔",line1.length()/2), reset,line1,line2,line3,intense_red_fmt,stripe_n_str("▁",line1.length()/2), reset);
    DEBUG_BREAKPOINT_QUIET();
}
void assert_failure(std::string_view comparator, LM::variable a, LM::variable b, LM::source_location loc){
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
            intense_red_fmt,    "[ERROR!]:Assertion failed in", reset,
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
    println(stderr, "\n{}{}{}\n{}{}{}{}\n{}{}{}\n",intense_red_fmt,    stripe_n_str("▔",line1.length()/2), reset,line1,line2,line3,line4,intense_red_fmt,stripe_n_str("▁",line1.length()/2), reset);
    DEBUG_BREAKPOINT_QUIET();
}
