#pragma once
#include "NumericConcepts.hpp"
#include "CommonConcepts.hpp"
#include "Types.h"
#include <algorithm>
#include <cstdlib>
#include <string_view>

// @Brief:
// Wrapper for the following recurring case:
// ```c++
//     float health;
//     const float health_base = 100;
//     const float health_max = 100;
//     const float health_min = 0;
// ```
// All arithmetic operators should work by default, except bitwise. (could add a specialization for int types in the future)
// The value can essentially be treated as its underlying T, *with the important exception* of 
// participation in overload resolution (i think), and probably some deduction cases.
template<typename T>
    requires Numeric<T>
struct Bounded{
    const T base;
    const T min;
    const T max;
    T val{base};
    constexpr inline operator T() const noexcept{
        return val;
    }
    inline Bounded& operator=(T rhs)& noexcept{
        val=rhs;
        clamp();
        return *this;
    }
    constexpr inline Bounded& operator+=(T rhs)& noexcept{
        val+=rhs;
        return *this;
    }
    constexpr inline Bounded& operator-=(T rhs)& noexcept{
        val-=rhs;
        return *this;
    }
    constexpr inline Bounded& operator*=(T rhs)& noexcept{
        val*=rhs;
        return *this;
    }
    constexpr inline Bounded& operator/=(T rhs)& noexcept{
        val/=rhs;
        return *this;
    }

    // postincrement
    constexpr inline T operator++() & noexcept{
        ++val;
        clamp();
        return *this;
    }

    // preincrement
    constexpr inline T operator++(int _)& noexcept{
        T before = val;
        ++(*this);
        return before;
    }
    constexpr inline T operator--() & noexcept{
        --val;
        clamp();
        return *this;
    }

    // preincrement
    constexpr inline T operator--(int _)& noexcept{
        T before = val;
        --(*this);
        return before;
    }

    inline Bounded& clamp()& noexcept{
        val = std::clamp(val,min,max);
        return *this;
    }

    inline Bounded& reset()& noexcept{
        val = base;
        return *this;
    }


};

// clang-format on

//@BRIEF:
// Generic wrapper for T which accepts a callable that returns T, taking any params, and updates it.
// Caller must use .invalidate() when the cached value could be invalid, and any subsequent calls to
// .get() will call the update() function supplied in the ctor. By default, T cached is unitialized,
// and the first call to get() will set its default value.
template <typename T>
struct CachedValue {
    void invalidate() noexcept{
        isStale = true; 
    }

    mutable T cached{};
    mutable bool        isStale{};
    CachedValue()=default;
    ~CachedValue()=default;



    template <typename UpdateFn>
    const T& get(UpdateFn&& update) const {
        if (isStale) {
            const auto& update_cached = std::forward<UpdateFn>(update);
            cached = update_cached();
            isStale = false;
        }
        return cached;
    }
};


static inline f32 randf(f32 min, f32 max) {
    return min + (random() / (f32)RAND_MAX) * (max - min);
}
static inline double nstoms(const i64 ns) {
    return ns / 1000000.0;
}
static inline double stons(const i64 ns) {
    return ns * 1000000000ULL;
}


template <typename T>
static inline constexpr std::string_view pretty_type_name() {
#if !defined(__cplusplus)
#error "non c++ context, tf compiler are you using"
#else
    #if __cplusplus == 202603L
        #warning "pretty_type_name does not work as intended on gcc16 with c++26 mode enabled."
        // Actually a very convinient time for this to break, as this is the update with reflection
        // So the entire need for this fuckery is erased by that update. Nice timing.
    #endif
#endif

#if defined(__clang__)
    std::string_view p = __PRETTY_FUNCTION__;
    auto             start = p.find("T = ");
    start += 4;
    auto end = p.rfind(']');
    return p.substr(start, end - start);

#elif defined(__GNUC__)
    std::string_view p = __PRETTY_FUNCTION__;
    auto             start = p.find("with T = ");
    start += 9;
    auto end = p.find(';', start);
    return p.substr(start, end - start);

#else
    return "unsupported compiler";
#endif
}
