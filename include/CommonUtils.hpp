#pragma once 
#include "Types.h"
#include <cstdlib>
#include <functional>
#include "glmWrapper.hpp"

enum struct Direction{
    UP, 
    DOWN,
    LEFT,
    RIGHT,
    BACKWARD,
    FORWARD,
};
template <typename C>
concept ContiguousContainer = std::ranges::contiguous_range<C>;


template<typename T>
concept isSequenceContainer = requires(T t) {
    std::begin(t);
    std::end(t);
};
// Generic wrapper for T which accepts a callable that returns T, taking any params, and updates it. Caller must use .invalidate() when the cached value could be invalid, and any subsequent calls to .get() will call the update() function supplied in the ctor.
// By default, T cached is unitialized, and the first call to get() will set its default value.
template <typename T, typename... Args>
struct CachedValue{
    // pass in an optional initial value, and a callable Generator
    bool isStale = true;
    std::function<T(Args...)> update;
    T cached{};

    CachedValue()= default;

    template<typename Callable>
    CachedValue(Callable&& f) : update(std::forward<Callable>(f)){
        isStale = true;
    }

    const T& get(Args... vargs) &{
        if (isStale){
            //LOG_DEBUG("isStale, updating.");
            cached = update(vargs...);
            isStale=false;
        }else{
            //LOG_DEBUG("cached! no update.");
        }
        return cached;
    }
    void invalidate(){
        isStale = true;
    }

};
f32 randf(f32 min, f32 max){
    return min+(random()/(f32)RAND_MAX)*(max-min);
}
vec3 randvec3(f32 min, f32 max){
    return vec3{
        randf(min,max),
        randf(min,max),
        randf(min,max),
    };
}
