#pragma once 

template <typename From, typename To>
using copy_const_t = std::conditional_t<
    std::is_const_v<From>, 
    std::add_const_t<To>, 
    To
>;
