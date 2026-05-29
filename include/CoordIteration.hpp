#pragma once 
#include "NumericConcepts.hpp"
#include <type_traits>
#include <concepts>
#include "DEBUG.hpp"
#include "Types.h"

template <typename T, typename Fn, typename ...Args>
concept return_type_is =  
    std::invocable<Fn, Args...> &&
    std::same_as<std::invoke_result_t<Fn, Args...>, T>;

template<typename T1, typename T2>
concept same_as_nocvref = std::same_as<std::remove_cvref_t<T1>, std::remove_cvref_t<T2>>;

template<typename T1>
using val_t = T1::value_type;


template<typename Fn, typename Vec>
concept has_3_scalar_params = std::invocable<Fn,val_t<Vec>,val_t<Vec>,val_t<Vec>>;

template<typename Fn, typename Vec>
concept has_2_scalar_params = std::invocable<Fn,val_t<Vec>,val_t<Vec>>;

template<typename Fn, typename ...Args>
concept callable_by = std::invocable<Fn, Args...>;


enum struct IterationSignal{
    CONTINUE,
    BREAK,
};

template<typename IntVec2, typename Fn>
    requires IVec2<IntVec2> && callable_by<Fn,IntVec2>
void ForEachInRangeEx(IntVec2 min, IntVec2 max, Fn&& task){
    using ScalarType = decltype(min)::value_type;

    for (ScalarType x = min.x; x<max.x; x++){
        for (ScalarType y = min.y; y<max.y; y++){
            if constexpr(return_type_is<IterationSignal, Fn,IntVec2>){
                switch(std::invoke(std::forward<Fn>(task),{x,y})){
                    case IterationSignal::CONTINUE: continue; break;
                    case IterationSignal::BREAK:    goto _break;
                }
            } else {
                std::invoke(std::forward<Fn>(task),{x,y});
            }
        }
    }
_break:

}

template<typename IntVec2, typename Fn>
    requires IVec2<IntVec2> && callable_by<Fn,val_t<IntVec2>,val_t<IntVec2>>
void ForEachInRangeEx(IntVec2 min, IntVec2 max, Fn&& task){
    using ScalarType = decltype(min)::value_type;

    for (ScalarType x = min.x; x<max.x; x++){
        for (ScalarType y = min.y; y<max.y; y++){
            if constexpr(return_type_is<IterationSignal, Fn, val_t<IntVec2>, val_t<IntVec2>>){
                switch(std::invoke(std::forward<Fn>(task),{x,y})){
                    case IterationSignal::CONTINUE: continue; break;
                    case IterationSignal::BREAK:    goto _break;
                }
            } else {
                std::invoke(std::forward<Fn>(task),x,y);
            }
        }
    }
_break:

}

template<typename IntVec3, typename Fn>
    requires IVec3<IntVec3> && callable_by<Fn,IntVec3>
void ForEachInRangeEx(IntVec3 min, IntVec3 max, Fn&& task){
    using ScalarType = decltype(min)::value_type;
    for (ScalarType x = min.x; x<max.x; x++){
    for (ScalarType y = min.y; y<max.y; y++){
    for (ScalarType z = min.z; z<max.z; z++){
        if constexpr(return_type_is<IterationSignal, Fn, IntVec3>){
            switch(std::invoke(std::forward<Fn>(task),{x,y,z})){
                case IterationSignal::CONTINUE: continue; break;
                case IterationSignal::BREAK:    goto _break;
            }
        } else {
            std::invoke(std::forward<Fn>(task),{x,y,z});
        }
    }
    }
    }
_break:
}

template<typename IntVec3, typename Fn>
    requires IVec3<IntVec3> && has_3_scalar_params<Fn,IntVec3>
void ForEachInRangeEx(IntVec3 min, IntVec3 max, Fn&& task){
    using ScalarType = decltype(min)::value_type;
    for (ScalarType x = min.x; x<max.x; x++){
    for (ScalarType y = min.y; y<max.y; y++){
    for (ScalarType z = min.z; z<max.z; z++){
        if constexpr(return_type_is<IterationSignal, Fn,val_t<IntVec3>,val_t<IntVec3>,val_t<IntVec3>>){
            switch(std::invoke(std::forward<Fn>(task),x,y,z)){
                case IterationSignal::CONTINUE: continue; break;
                case IterationSignal::BREAK:    goto _break;
            }
        } else {
            std::invoke(std::forward<Fn>(task),x,y,z);
        }
    }
    }
    }
_break:
}

template<typename Integer, typename Fn>
    requires Integral<Integer>
void ForEachInRangeEx(Integer min, Integer max, Fn&& task){
    for (Integer x = min; x < max; x++){
        std::invoke(std::forward<Fn>(task),x);
    }
}
