#pragma once
#include <cstdint>
#include <numeric>
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

const inline f32 F32_MAX = std::numeric_limits<f32>::max();
const inline f32 F32_MIN = std::numeric_limits<f32>::lowest();

template<size_t bitwidth, bool is_signed>
    requires (bitwidth < 8)
class ubits{
    using setter_type = u32;
    using storage_type = std::conditional_t<is_signed,int8_t,uint8_t>;
public:
    constexpr ubits(setter_type val) {
        *this = val; 
    }
    
    constexpr ubits& operator=(setter_type rhs) noexcept{
        data = rhs & bitmask();
        return *this;
    }
    
    constexpr setter_type bitmask(){
        return (~(0u)) >> bitwidth;
    }

    // Implicit conversion
    operator setter_type() const noexcept{
        return data; 
    } 

private:
    storage_type data : bitwidth; 
};

using u7 = ubits<7, false>;
using u6 = ubits<6, false>;
using u5 = ubits<5, false>;
using u4 = ubits<4, false>;
using u3 = ubits<2, false>;
using u2 = ubits<2, false>;
using u1 = ubits<1, false>;

using i7 = ubits<7, true>;
using i6 = ubits<6, true>;
using i5 = ubits<5, true>;
using i4 = ubits<4, true>;
using i3 = ubits<2, true>;
using i2 = ubits<2, true>;
using i1 = ubits<1, true>;

#define arrlen(x) (sizeof(x) / sizeof(x[0]))

#define NOP ((void)0)
#define CONCAT(a, b) a##b

#define LBRACE (
#define RBRACE )

// clang-format off
#define MACRO_BEGIN do{
#define MACRO_END }while (0)
// clang-format on
