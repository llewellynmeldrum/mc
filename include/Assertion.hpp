#pragma once 

#include "refl.hpp"
#include "libassert/assert.hpp"
// HACK: to remove the cassert/ assert.h macro. This will of course break if either is included after this header
#undef assert 

#define assert(...) ASSERT(__VA_ARGS__)
#define debug_assert(...) DEBUG_ASSERT(__VA_ARGS__)
#define assume(...) ASSUME(__VA_ARGS__)

#define assert_val(...) ASSERT_VAL(__VA_ARGS__)
#define debug_assert_val(...) DEBUG_ASSERT_VAL(__VA_ARGS__)
#define assume_val(...) ASSUME_VAL(__VA_ARGS__)

#define panic(...) PANIC(__VA_ARGS__)
#define unreachable(...) UNREACHABLE(__VA_ARGS__)

#define assert_eq(a,b)  ASSERT(a==b)
#define assert_neq(a,b) ASSERT(a!=b)
#define assert_lt(a,b)  ASSERT(a< b)
#define assert_gt(a,b)  ASSERT(a> b)
#define assert_leq(a,b) ASSERT(a<=b)
#define assert_geq(a,b) ASSERT(a>=b)


[[noreturn]] void report_out_of_range(i64 cont_sz, std::string_view err_msg, refl::variable cont_var, refl::variable key_var,  refl::source_location loc);

