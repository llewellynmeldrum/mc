#pragma once 
#define ASSERT_TRUE(a) assert(a)

#include "refl.hpp"
#define assert_binary_valuewise(a, COMPARATOR, b)do{\
    [[unlikely]] \
    if  ((a COMPARATOR b) == false){\
        assert_failure(#COMPARATOR,  MAKE_VAR(a),MAKE_VAR(b),SRC_LOC_CURRENT());\
    }\
}while(0)

#define assert_unary(invariant, ...)do{\
    [[unlikely]]\
    if  (!(invariant)){\
        assert_failure(static_cast<std::string>(#invariant),  SRC_LOC_CURRENT(), "" __VA_OPT__( ) __VA_ARGS__);\
    }\
}while(0)

// HACK: to remove the cassert/ assert.h macro. This will of course break if either is included after this header
#undef assert 


#define assert(invariant,...) assert_unary(invariant, "" __VA_OPT__( ) __VA_ARGS__)

// TODO: add bitwise comparators? could be useful
#define assert_eq(a,b)  assert_binary_valuewise(a,==,b)
#define assert_neq(a,b) assert_binary_valuewise(a,!=,b)
#define assert_lt(a,b)  assert_binary_valuewise(a,<,b)
#define assert_gt(a,b)  assert_binary_valuewise(a,>,b)
#define assert_leq(a,b) assert_binary_valuewise(a,<=,b)
#define assert_geq(a,b) assert_binary_valuewise(a,>=,b)


[[gnu::cold, gnu::noinline, noreturn]] 
void assert_failure(std::string_view comparator, refl::variable a, refl::variable b, refl::source_location loc);

[[gnu::cold, gnu::noinline, noreturn]] 
void assert_failure(std::string_view name, refl::source_location loc,std::string_view  desc);

[[noreturn]] 
void report_OOR(i64 cont_sz, std::string_view err_msg, refl::variable cont_var, refl::variable key_var,  refl::source_location loc);

