#pragma once 
#define ASSERT_TRUE(a) assert(a)

#include "LM.hpp"
#define assert_binary(a, COMPARATOR, b)do{\
    if (!(a COMPARATOR b)){\
        assert_failure(#COMPARATOR,  VAR_DBG(a),VAR_DBG(b),SRC_LOC_CURRENT());\
    }\
}while(0)
#define assert_unary(invariant, ...)do{\
    if (!(invariant)){\
        assert_failure(static_cast<std::string>(#invariant),  SRC_LOC_CURRENT(), "" __VA_OPT__( ) __VA_ARGS__);\
    }\
}while(0)

#define assert_eq(a,b) assert_binary(a,==,b)
#define assert_neq(a,b) assert_binary(a,!=,b)
#define assert_lt(a,b) assert_binary(a,<,b)
#define assert_gt(a,b) assert_binary(a,>,b)
#define assert_leq(a,b) assert_binary(a,<=,b)
#define assert_geq(a,b) assert_binary(a,>=,b)

#undef assert
#define assert(invariant,...) assert_unary(invariant, "" __VA_OPT__( ) __VA_ARGS__)
void assert_failure(std::string_view comparator, LM::variable a, LM::variable b, LM::source_location loc);
void assert_failure(std::string_view name, LM::source_location loc,std::string_view  desc);
