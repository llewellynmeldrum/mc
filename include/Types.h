#pragma once
#include <cstdint>
#include <numeric>
#include <string_view>
// NOTE: I know this is considered bad practice, and I probably wont do it for the ""s operators, but i think ""sv is unique enough that it shouldnt matter.
// I just really dislike c strings.
using namespace std::string_view_literals;
using u64 = uint64_t;
using u32 = uint32_t;
using u16 = uint16_t;
using u8 = uint8_t;

using i64 = int64_t;
using i32 = int32_t;
using i16 = int16_t;
using i8 = int8_t;

using f32 = float;
using f64 = double;

using Byte = char;
using Radians = f32;

const inline f32 F32_MAX = std::numeric_limits<f32>::max();
const inline f32 F32_MIN = std::numeric_limits<f32>::lowest();


constexpr static inline std::size_t N_CARDINAL_DIRECTIONS {4};
// Parity with glsl
using uint = u32;


#define arrlen(x) (sizeof(x) / sizeof(x[0]))

#define NOP ((void)0)
#define CONCAT(a, b) a##b

#define LBRACE (
#define RBRACE )

// clang-format off
#define MACRO_BEGIN do{
#define MACRO_END }while (0)
// clang-format on
