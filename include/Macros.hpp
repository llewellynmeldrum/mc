#pragma once 

// NOTE: we use GCC's always_inline, because it issues a compilation error if inlining fails. 
// I prefer this failure mode to clang's always_inline. 
// Also both GCC and clang support gnu::always inline, but only clang supports clang::always_inline, GCC silently ignores it.
#define ALWAYS_INLINE [[gnu::always_inline]]
