#pragma once 

#include "glmWrapper.hpp"
#include "CommonUtils.hpp"
#include "NumericConcepts.hpp"
#include <print>
#include <type_traits>
#include <functional>
#include <unordered_set>
#include "Breakpoints.hpp"
#include "Types.h"
#include "CommonConcepts.hpp"
#include "Assertion.hpp"


template<typename IntVec2, typename Fn>
    requires is_ivec2<IntVec2> && callable_with<Fn,IntVec2>
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

// EXCLUSIVE!!!!!!!!!!!! EX!!!! YOU LITERALLY NAMED THE FUCKING FUNCTION EXCLUSIVE!!!!!!!!!!! THE SECOND VARIABLE IS EX-CLU-SIVE. HOW THE FUCK DO YOU FORGET THAT. HOW MANY FUCKING TIMES. YOU PIECE OF SHIT 
template<typename IntVec2, typename Fn>
    requires is_ivec2<IntVec2> && callable_with<Fn,val_t<IntVec2>,val_t<IntVec2>>
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
    requires is_ivec3<IntVec3> && callable_with<Fn,IntVec3>
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
    requires is_ivec3<IntVec3> && has_3_scalar_params<Fn,IntVec3>
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

template<typename IVEC2, typename Fn>
    requires is_ivec2<IVEC2>
size_t for_each_spiral(size_t MAX_COUNT, IVEC2 center, i32 radius, Fn&& task){
    using namespace std;
    using namespace glm;
    size_t count = {0};

    if (MAX_COUNT == 0){
        return 0;
    }
    const IVEC2& C = center;


    auto should_continue = [&](i32 x,i32 z) {
        if (count >= MAX_COUNT){
            return false; // shouldnt continue
        }
        auto should_add = invoke(task, x,z);
        count += should_add;
        return count < MAX_COUNT;
    };

    i32 x{C.x}, z{C.z};
    if(!should_continue(x,z)) return count;
    for (i32 r = 1; r <= radius; r++){
        const i32 extent = 2*r;
        // break out of the prior spiral  (off to the 'left')
        if(!should_continue(--x,z)) return count; 
        for (int j = 0; j<extent - 1;j++)    {
            if(!should_continue(x,++z)) return count;
        } // traverse the remaining (-X) edge 'upwards'
        for (int j = 0; j<extent ; j++)     {
            if(!should_continue(++x,z)) return count;
        }  // traverse the whole     (Z+) edge 'right'
        for (int j = 0; j<extent ; j++)     {
            if(!should_continue(x,--z)) return count;
        }  // traverse the whole     (+X) edge 'down'
        for (int j = 0; j<extent ; j++)     {
            if(!should_continue(--x,z)) return count;
        }  // traverse the whole     (+X) edge 'left'
    }
    return count;
}
