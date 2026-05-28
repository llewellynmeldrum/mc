#pragma once 
#include "NumericConcepts.hpp"
#include <type_traits>
#include <concepts>


// IVec3 Overload
template<typename A, typename B, typename Fn>
    requires IVec3<A> && IVec3<B> 
                && std::invocable<Fn, std::remove_cvref_t<A>>
bool ForEachCoordInRangeEx(A min, B max, Fn&& task){
    static_assert(std::same_as<A,B>, 
        "Bounds types must match exactly!");

    using TaskReturnType = std::invoke_result_t<Fn,std::remove_cvref_t<A>>;
    static_assert(std::same_as<A,TaskReturnType>, 
        "Task must return the exact type of bounds params!");

    using VecType = std::invoke_result_t<Fn,std::remove_cvref_t<A>>;
    using ScalarType = decltype(min)::value_type;

    bool iteration_took_place = false;
    for (ScalarType x = min.x; x<max.x; x++){
        iteration_took_place=true;
        for (ScalarType y = min.y; y<max.y; y++){
            for (ScalarType z = min.z; z<max.z; z++){
                task(VecType{x,y,z});
            }
        }
    }

    assert(iteration_took_place); // probably an error if this triggers
    return iteration_took_place;
}

// IVec2 Overload
template<typename A, typename B, typename Fn>
    requires IVec2<A> && IVec2<B> 
                && std::invocable<Fn, std::remove_cvref_t<A>>
bool ForEachCoordInRangeEx(A min, B max, Fn&& task){
    static_assert(std::same_as<A,B>, 
        "Bounds types must match exactly!");

    using TaskReturnType = std::invoke_result_t<Fn,std::remove_cvref_t<A>>;
    static_assert(std::same_as<A,TaskReturnType>, 
        "Task must return the exact type of bounds params!");

    using VecType = std::invoke_result_t<Fn,std::remove_cvref_t<A>>;
    using ScalarType = decltype(min)::value_type;

    bool iteration_took_place = false;
    for (ScalarType x = min.x; x<max.x; x++){
        iteration_took_place=true;
        for (ScalarType y = min.y; y<max.y; y++){
            task(VecType{x,y});
        }
    }

    assert(iteration_took_place); // probably an error if this triggers
    return iteration_took_place;
}
