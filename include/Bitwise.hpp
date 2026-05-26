#pragma once 
template<std::integral Integer>
[[gnu::always_inline]]
constexpr Integer SetBit(Integer& x, std::size_t i){
    return x ^= (1 << i);
};

template<std::integral Integer>
[[gnu::always_inline]]
constexpr Integer UnsetBit(Integer& x, std::size_t i){
    return x &= ~(1 << i);
};

template<std::integral Integer>
[[gnu::always_inline]]
constexpr bool GetBit(Integer x, std::size_t i){
    return 1 & (x >> i);
};
