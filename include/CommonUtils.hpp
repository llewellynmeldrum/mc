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
    const T m_default;
    const T m_min;
    const T m_max;
    T m_cur{m_default};
    Bounded& clamp()& noexcept{
        m_cur = std::clamp(m_cur,m_min,m_max);
        return *this;
    }

    Bounded& reset()& noexcept{
        m_cur = m_default;
        return *this;
    }
    constexpr T get() const noexcept{ return m_cur; }
    constexpr T get_default() const noexcept{ return m_default; }
    constexpr T get_min() const noexcept{ return m_min; }
    constexpr T get_max() const noexcept{ return m_max; }
    constexpr T get_range() const noexcept{ return get_max()-get_min(); }
    constexpr operator T() const noexcept{ return m_cur; }

    template<typename V>
        requires implicit_convertible_to<V,T>
    Bounded& operator=(V rhs)& noexcept{
        m_cur=rhs;
        clamp();
        return *this;
    }
    constexpr Bounded& operator+=(T rhs)& noexcept{
        m_cur+=rhs;
        clamp();
        return *this;
    }
    constexpr Bounded& operator-=(T rhs)& noexcept{
        m_cur-=rhs;
        clamp();
        return *this;
    }
    constexpr Bounded& operator*=(T rhs)& noexcept{
        m_cur*=rhs;
        clamp();
        return *this;
    }
    constexpr Bounded& operator/=(T rhs)& noexcept{
        m_cur/=rhs;
        clamp();
        return *this;
    }

    // postincrement
    constexpr T operator++() & noexcept{
        ++m_cur;
        clamp();
        return *this;
    }

    // preincrement
    constexpr T operator++(int _)& noexcept{
        T before = m_cur;
        ++(*this);
        clamp();
        return before;
    }
    constexpr T operator--() & noexcept{
        --m_cur;
        clamp();
        return *this;
    }

    // preincrement
    constexpr T operator--(int _)& noexcept{
        T before = m_cur;
        --(*this);
        clamp();
        return before;
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


