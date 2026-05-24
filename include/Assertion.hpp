#pragma once 
#include "DebugFormat.hpp"

#include "AnsiCodes.hpp"
#include "DEBUG.hpp"
#include <print>
#define ASSERT_LT(a,b) assert_lt(a,b,#a,#b,__FILE_NAME__, __FUNCTION__, __PRETTY_FUNCTION__, __LINE__)
#define ASSERT_GT(a,b) assert_gt(a,b,#a,#b,__FILE_NAME__, __FUNCTION__, __PRETTY_FUNCTION__, __LINE__)
#define ASSERT_EQ(a,b) assert_eq(a,b,#a,#b,__FILE_NAME__, __FUNCTION__, __PRETTY_FUNCTION__, __LINE__)
#define ASSERT_NEQ(a,b) assert_neq(a,b,#a,#b,__FILE_NAME__, __FUNCTION__, __PRETTY_FUNCTION__, __LINE__)
#define ASSERT_TRUE(a) assert_true(a,#a,__FILE_NAME__, __FUNCTION__, __PRETTY_FUNCTION__, __LINE__)
#define ASSERT_LEQ(a,b) assert_leq(a,b,#a,#b,__FILE_NAME__, __FUNCTION__, __PRETTY_FUNCTION__, __LINE__)
#define ASSERT_GEQ(a,b) assert_geq(a,b,#a,#b,__FILE_NAME__, __FUNCTION__, __PRETTY_FUNCTION__, __LINE__)
inline void assert_generic_failure(std::string_view comparator, auto a, auto b,
                                std::string_view str_a, std::string_view str_b,
                                std::string_view file_name, std::string_view fn, std::string_view pretty_fn, int line){
    auto red = fmt::red;
    auto id_fmt     = fmt::blue;
    auto clr        = fmt::clear;
    auto val_fmt    = fmt::orange;
    std::println(stderr, "\e[31;1;4m"
        "Assertion FAILED in {"/*FILE*/"}:{"/*LINE*/"} !\n"     "\e[0m\e[31;1m"
         "Expected:{}\n\t {}{}{} {} {}{}{},"                 "\n"
          "{}Where{}\n\t {}{}{}={}{}{}, {}{}{}={}{}{},"
            " ({}i.e expected{} {}{}{} {} {}{}{})"                         "\n"
           "{}in function:\n\t{}{} {}\n\n",
        file_name, line,
         clr, id_fmt, str_a, clr, comparator,  id_fmt,  str_b, clr,
          fmt::red, clr, id_fmt, str_a, clr, val_fmt, dbg_fmt(a), clr,
                id_fmt, str_b, clr, val_fmt, dbg_fmt(b), clr,
          fmt::red,clr, val_fmt, dbg_fmt(a), clr,comparator, val_fmt, dbg_fmt(b), clr,
           fmt::red, id_fmt, pretty_fn, clr);                                         

    DEBUG_BREAKPOINT_QUIET();
}


inline bool assert_true(bool a,
                                std::string_view str_a,
                                std::string_view file_name, std::string_view fn, std::string_view pretty_fn, int line){
    if (!(a)){
        assert_generic_failure("==", a,true, str_a, "True",file_name,fn,pretty_fn,line);
        return false;
    } else{
        return true;
    }
}
inline bool assert_lt(auto a, auto b,
                                std::string_view str_a, std::string_view str_b,
                                std::string_view file_name, std::string_view fn, std::string_view pretty_fn, int line){
    if (!(a<b)){
        assert_generic_failure("<", a,b,str_a,str_b,file_name,fn,pretty_fn,line);
        return false;
    } else{
        return true;
    }
}
inline bool assert_gt(auto a, auto b,
                                std::string_view str_a, std::string_view str_b,
                                std::string_view file_name, std::string_view fn, std::string_view pretty_fn, int line){
    if (!(a>b)){
        assert_generic_failure(">", a,b,str_a,str_b,file_name,fn,pretty_fn,line);
        return false;
    } else{
        return true;
    }
}
inline bool assert_geq(auto a, auto b,
                                std::string_view str_a, std::string_view str_b,
                                std::string_view file_name, std::string_view fn, std::string_view pretty_fn, int line){
    if (!(a>=b)){
        assert_generic_failure(">=", a,b,str_a,str_b,file_name,fn,pretty_fn,line);
        return false;
    } else{
        return true;
    }
}
inline bool assert_leq(auto a, auto b,
                                std::string_view str_a, std::string_view str_b,
                                std::string_view file_name, std::string_view fn, std::string_view pretty_fn, int line){
    if (!(a<=b)){
        assert_generic_failure("<=", a,b,str_a,str_b,file_name,fn,pretty_fn,line);
        return false;
    } else{
        return true;
    }
}

inline bool assert_eq(auto a, auto b,
                                std::string_view str_a, std::string_view str_b,
                                std::string_view file_name, std::string_view fn, std::string_view pretty_fn, int line){
    if (!(a==b)){
        assert_generic_failure("==", a,b,str_a,str_b,file_name,fn,pretty_fn,line);
        return false;
    } else{
        return true;
    }
}
inline bool assert_neq(auto a, auto b,
                                std::string_view str_a, std::string_view str_b,
                                std::string_view file_name, std::string_view fn, std::string_view pretty_fn, int line){
    if (!(a!=b)){
        assert_generic_failure("!=", a,b,str_a,str_b,file_name,fn,pretty_fn,line);
        return false;
    } else{
        return true;
    }
}
