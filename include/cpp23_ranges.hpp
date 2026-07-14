#pragma once 

// NOTE: in order to use c++23 ranges, we switch based on the compiler
// gcc16 has all of these implemented, but clang lags behind, thus we can use the original ranges library instead
#ifdef __clang__
    #include <range/v3/view/iota.hpp>
    #include <range/v3/view/cartesian_product.hpp>
    #include <range/v3/view/chunk.hpp>
    #include <range/v3/view/enumerate.hpp>
    #include <range/v3/view/map.hpp>
    #include <range/v3/view/transform.hpp>
    #include <range/v3/view/filter.hpp>
    #include <range/v3/view/reverse.hpp>

    #include <range/v3/range/conversion.hpp>

    #include <range/v3/algorithm/sort.hpp>
    #include <range/v3/algorithm/nth_element.hpp>
    #include <range/v3/algorithm/copy.hpp>
//    #include <range/v3/.hpp>
#elif defined(__GNU__)
    #include <ranges>
    namespace ranges = std::ranges;
#endif 

namespace views = ranges::views;
