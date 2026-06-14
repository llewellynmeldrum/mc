#pragma once 
#include <ostream>
namespace fmt{
namespace ansi{
    enum {
    reset=0,
    bold=1,
    dim=2,
    italic=3,
    underline=4,
    slow_blink=5,
    fast_blink=6,
    rev=7,
    hide=8,
    strike=9,
    dbl_underline=21,
    no_intensity=22,
    no_italic=23,
    no_under=24,
    no_blink=25,
    no_rev=27,
    no_hide=28,
    no_strike=29,

    fg_black=30,
    fg_red=31,
    fg_green=32,
    fg_yellow=33,
    fg_blue=34,
    fg_magenta=35,
    fg_cyan=36,
    fg_white=37,

    //fg_rgb(...)=38,

    fg_default=39,
    bg_black=40,
    bg_red=41,
    bg_green=42,
    bg_yellow=43,
    bg_blue=44,
    bg_magenta=45,
    bg_cyan=46,
    bg_white=47,

    //bg_rgb(...)=48,

    bg_default=49,

    framed=51,
    circle=52,
    overline=53,
    no_frame=54,
    no_over=55,

    //un_rgb(...)=58,

    un_default=59,
    sup=73,
    sub=74,
    no_su=75,

    bg_grey=100,
    bg_br_red=101,
    bg_br_green=102,
    bg_br_yellow=103,
    bg_br_blue=104,
    bg_br_magenta=105,
    bg_br_cyan=106,

    fg_grey=90,
    fg_br_red=91,
    fg_br_green=92,
    fg_br_yellow=93,
    fg_br_blue=94,
    fg_br_magenta=95,
    fg_br_cyan=96,
    };
inline std::string fg_rgb(int r, int g, int b){
    using namespace std;
    // \e[38;r;g;bm
    return std::format("38;2;{};{};{}", to_string(r), to_string(g), to_string(b));
}
inline std::string fg_rgb(int w){ return fg_rgb(w,w,w); }

inline std::string bg_rgb(int r, int g, int b){
    using namespace std;
    // \e[38;r;g;bm
    return std::format("48;2;{};{};{}", to_string(r), to_string(g), to_string(b));
}
inline std::string bg_rgb(int w){ return bg_rgb(w,w,w); }

inline std::string underline_rgb(int r, int g, int b){
    using namespace std;
    // \e[38;r;g;bm
    return std::format("58;{};{};{}", to_string(r), to_string(g), to_string(b));
}

inline std::string underline_rgb(int w){ return underline_rgb(w,w,w);}


inline std::string bg_code_blocks = ansi::bg_rgb(50);
} //NOTE: namespace: ansi

template<typename ...Args>
inline std::string style(Args ...codes){
    if constexpr(sizeof...(codes) ==0) return "\e[m";
    using namespace std;
    string out{"\e["};
    auto append =[&](auto code){
        if constexpr (std::is_same_v<std::string, decltype(code)>){
            out += code + ";";
        }else{
            out += to_string(code) + ";";
        }
    };
    (append(codes), ...);
    out.back()='m';
    return out;
}
template<typename ...Args>
inline std::string bg_rgb(Args ...vargs){
    if constexpr(sizeof...(vargs) ==0) return "\e[m";
    using namespace std;
    string out{"\e["};
    auto append =[&](auto code){
        out+=to_string(code) + ";";
    };
    (append(vargs), ...);
    out.back()='m';
    return out;
}
inline std::string reset = style(ansi::reset);
inline std::string bold = style(ansi::bold);
inline std::string dim = style(ansi::dim);
inline std::string italic = style(ansi::italic);
inline std::string underline = style(ansi::underline);
inline std::string slow_blink = style(ansi::slow_blink);
inline std::string fast_blink = style(ansi::fast_blink);
inline std::string rev = style(ansi::rev);
inline std::string hide = style(ansi::hide);
inline std::string strike = style(ansi::strike);
inline std::string dbl_underline = style(ansi::dbl_underline);
inline std::string no_intensity = style(ansi::no_intensity);
inline std::string no_italic = style(ansi::no_italic);
inline std::string no_under = style(ansi::no_under);
inline std::string no_blink = style(ansi::no_blink);
inline std::string no_rev = style(ansi::no_rev);
inline std::string no_hide = style(ansi::no_hide);
inline std::string no_strike = style(ansi::no_strike);

inline std::string fg_rgb(int r, int g, int b){return style(ansi::fg_rgb(r,g,b));}
inline std::string fg_rgb(int w){return style(ansi::fg_rgb(w));}

inline std::string fg_black = style(ansi::fg_black);
inline std::string fg_red = style(ansi::fg_red); inline std::string bold_red = style(ansi::bold, ansi::fg_red);
inline std::string fg_green = style(ansi::fg_green);
inline std::string fg_yellow = style(ansi::fg_yellow);
inline std::string fg_blue = style(ansi::fg_blue);
inline std::string fg_magenta = style(ansi::fg_magenta);
inline std::string fg_cyan = style(ansi::fg_cyan);
inline std::string fg_white = style(ansi::fg_white);

inline std::string fg_pink = fg_rgb(255,192,203);


inline std::string fg_default = style(ansi::fg_default);
inline std::string reset_fg = style(ansi::fg_default);
inline std::string bg_black = style(ansi::bg_black);
inline std::string bg_red = style(ansi::bg_red);
inline std::string bg_green = style(ansi::bg_green);
inline std::string bg_yellow = style(ansi::bg_yellow);
inline std::string bg_blue = style(ansi::bg_blue);
inline std::string bg_magenta = style(ansi::bg_magenta);
inline std::string bg_cyan = style(ansi::bg_cyan);
inline std::string bg_white = style(ansi::bg_white);

inline std::string bg_rgb(int r, int g, int b){return style(ansi::bg_rgb(r,g,b));}
inline std::string bg_rgb(int w){return style(ansi::bg_rgb(w));}

inline std::string bg_default = style(ansi::bg_default);
inline std::string reset_bg= style(ansi::bg_default);

inline std::string framed = style(ansi::framed);
inline std::string circle = style(ansi::circle);
inline std::string overline = style(ansi::overline);
inline std::string no_frame = style(ansi::no_frame);
inline std::string no_over = style(ansi::no_over);



inline std::string un_default = style(ansi::un_default);
inline std::string sup = style(ansi::sup);
inline std::string sub = style(ansi::sub);
inline std::string no_su = style(ansi::no_su);

inline std::string bg_grey = style(ansi::bg_grey);
inline std::string bg_gray = style(ansi::bg_grey);

inline std::string fg_grey = style(ansi::fg_grey);
inline std::string fg_gray = style(ansi::fg_grey);

inline std::string bg_br_red = style(ansi::bg_br_red);
inline std::string bg_br_green = style(ansi::bg_br_green);
inline std::string bg_br_yellow = style(ansi::bg_br_yellow);
inline std::string bg_br_blue = style(ansi::bg_br_blue);
inline std::string bg_br_magenta = style(ansi::bg_br_magenta);
inline std::string bg_br_cyan = style(ansi::bg_br_cyan);

inline std::string fg_br_red = style(ansi::fg_br_red);
inline std::string fg_br_green = style(ansi::fg_br_green);
inline std::string fg_br_yellow = style(ansi::fg_br_yellow);
inline std::string fg_br_blue = style(ansi::fg_br_blue);
inline std::string fg_br_magenta = style(ansi::fg_br_magenta);
inline std::string fg_br_cyan = style(ansi::fg_br_cyan);


template<typename T>
std::string styled(auto style, const T& val){
    return std::format("{0}{1}{2}",style,val,fmt::reset);
}
}
