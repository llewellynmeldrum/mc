#pragma once
#include <stdint.h>
typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef int64_t i64;
typedef int32_t i32;
typedef int16_t i16;
typedef int8_t  i8;

typedef float  f32;
typedef double f64;

typedef char Byte;

#define arrlen(x) (sizeof(x) / sizeof(x[0]))

#define NOP ((void)0)
#define CONCAT(a, b) a##b

#define LBRACE (
#define RBRACE )

// clang-format off
#define MACRO_BEGIN do{
#define MACRO_END }while (0)
// clang-format on
