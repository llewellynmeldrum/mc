#pragma once 
#include "Logger.hpp"
#include <functional>

enum struct Direction{
    RIGHT,
    LEFT,
    UP,
    DOWN,
    BACKWARD,
    FORWARD,
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
