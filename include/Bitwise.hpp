#pragma once 
#include "Types.h"
struct BitMasker{
    consteval BitMasker(u32 _offset, u32 _mask)
        : offset(_offset)
        , mask(_mask)
    {
        if (offset >= 31){
            throw "what are we doing here bromato";
        }
    }
    u32 offset; // location of the first bit 
    u32 mask;
    constexpr u32 get_from(u32 storage) const noexcept{
        return (storage & mask) >> offset;
    }
    constexpr void set_in(u32& storage, u32 value) const noexcept{
        storage |= (value << offset);
    }
};
