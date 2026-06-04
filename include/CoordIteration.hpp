#pragma once 

#include "glmWrapper.hpp"
#include "CommonUtils.hpp"
#include "NumericConcepts.hpp"
#include <print>
#include <type_traits>
#include <functional>
#include <unordered_set>
#include "DEBUG.hpp"
#include "Types.h"
#include "CommonConcepts.hpp"


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

// Overload for the function below it, with 
// -> Each spiral begins at `center`, and continues outwards in a clockwise pattern, which approximates nearest to farthest ordering. 
//    NOTE: (clockwise when viewed from above, assuming y is the vertical axis)
// -> The iteration begins at the top slice (center.y+radius), and continues downwards by 1 after each spiral.
// -> It is guaranteed to reach every coordinate (if `MAX_COUNT` permits), in the range:
//   [C.x-R, C.x+R]
//   [C.y-R, C.y+R]
//   [C.z-R, C.z+R]
template<typename IntVec3, typename Integer, typename Fn>
    requires IVec3Like<IntVec3>
void SpiralIterateRange(IntVec3 center, Integer vertRadius, Integer hozRadius, Fn&& task){
    SpiralIterateRange(numeric_max<std::size_t>(), center, vertRadius,hozRadius, std::forward<Fn>(task));
}
// runs task(x,y,z) in a particular order: a series of horizontal spirals.
// -> Each spiral begins at `center`, and continues outwards in a clockwise pattern, which approximates nearest to farthest ordering. 
//    NOTE: (clockwise when viewed from above, assuming y is the vertical axis)
// -> The iteration begins at the top slice (center.y+radius), and continues downwards by 1 after each spiral.
// -> It is guaranteed to reach every coordinate (if `MAX_COUNT` permits), in the range:
//   [C.x-R, C.x+R]
//   [C.y-R, C.y+R]
//   [C.z-R, C.z+R]
template<typename IntVec3, typename Integer, typename Fn>
    requires IVec3Like<IntVec3> && Integral<Integer>
void SpiralIterateRange(std::size_t MAX_COUNT, IntVec3 center, Integer vertRadius, Integer hozRadius, Fn&& task){
    using namespace std;
    using namespace glm;
    size_t count = {0};

    if (MAX_COUNT == 0){
        return;
    }
    const Integer& R = hozRadius;
    const IntVec3& C = center;
    i32 minY = C.y-static_cast<i32>(vertRadius);
    i32 maxY = C.y+static_cast<i32>(vertRadius);
    std::unordered_set<ivec3> expected;
    std::unordered_set<ivec3> unique;


    for (i32 y = maxY; y>=minY; y--){
        for (i32 x = C.x-hozRadius; x<=C.x+hozRadius; x++){
            for (i32 z = C.x-hozRadius; z<=C.x+hozRadius; z++){
                expected.emplace(x,y,z);
            }
        }
    }

    auto should_continue = [&count, &task, MAX_COUNT, &unique](i32 x, i32 y, i32 z) {
        if (count >= MAX_COUNT){
            return false; // shouldnt continue
        }
        auto shouldAdd = invoke(task, x,y,z);
        if (shouldAdd){
            count++;
            unique.emplace(x,y,z);
        }
        assert(unique.size()==count);
        return count < MAX_COUNT;
    };

    for (i32 y = maxY; y>=minY; y--){
        Integer x{C.x}, z{C.z};
		if(!should_continue(x,y,z)) return;
        for (i32 r = 1; r <= R; r++){
            const i32 extent = 2*r;
            // break out of the prior spiral  (off to the 'left')
			if(!should_continue(--x,y,z)) return; 
            for (int j = 0; j<extent - 1;j++)    {
				if(!should_continue(x,y,++z)) return;
			} // traverse the remaining (-X) edge 'upwards'
            for (int j = 0; j<extent ; j++)     {
				if(!should_continue(++x,y,z)) return;
			}  // traverse the whole     (Z+) edge 'right'
            for (int j = 0; j<extent ; j++)     {
				if(!should_continue(x,y,--z)) return;
			}  // traverse the whole     (+X) edge 'down'
            for (int j = 0; j<extent ; j++)     {
				if(!should_continue(--x,y,z)) return;
			}  // traverse the whole     (+X) edge 'left'
        }
    }
}
