#pragma once 
#include "CommonConcepts.hpp"
#include "NumericConcepts.hpp"


template<typename Int>
requires Integral<Int>
[[gnu::always_inline]]
inline constexpr Int SetBit(Int& x, std::size_t i){
    return x ^= (1 << i);
};

template<typename Int>
requires Integral<Int>
[[gnu::always_inline]]
inline constexpr Int UnsetBit(Int& x, std::size_t i){
    return x &= ~(1 << i);
};

template<typename Int>
requires Integral<Int>
[[gnu::always_inline]]
inline constexpr bool GetBit(Int x, std::size_t i){
    return 1 & (x >> i);
};

template<typename IntA, typename IntB>
requires Integral<IntA> && Integral<IntB>
[[gnu::always_inline]]
inline constexpr bool SignsDiffer(IntA a, IntB b){
    return (a^b)<0; 

    // if the signs differ, the sign bit will be set to 1, thus making it negative
    
    // i.e 100110 ->
    //     101101 -> 
    //     -> Both negative, thus their XOR will have sign bit=0, therefore positive
    // i.e 000110 ->
    //     001101 -> 
    //     -> Both positive, thus their XOR will have sign bit=0, therefore positive
    // i.e 100110 ->
    //     001101 -> 
    //     -> Signs differ, thus their XOR will have sign bit=1, therefore negative. 
};


template<typename Cont>
requires std::ranges::contiguous_range<Cont> && Integral<val_t<Cont>>
inline constexpr std::size_t pop_count(Cont& cont){
    for (const auto& val: cont){
        std::popcount(val);
    }
    return 1;
}
