#pragma once
#include "Types.h"
#include <cstdlib>
#include <functional>
#include <string_view>
#include "glmWrapper.hpp"

enum struct Direction : i8 {
    FORWARD,   // -z
    BACKWARD,  // +z
    LEFT,      //-x
    RIGHT,     // +x
    DOWN,      // -y
    UP,        // +y
};
auto Direction_FORWARD_IDX = 0;
auto Direction_BACKWARD_IDX = 1;
auto Direction_LEFT_IDX = 2;
auto Direction_RIGHT_IDX = 3;
auto Direction_DOWN_IDX = 4;
auto Direction_UP_IDX = 5;
template <typename C>
concept ContiguousContainer = std::ranges::contiguous_range<C>;

template <typename T>
concept isSequenceContainer = requires(T t) {
    std::begin(t);
    std::end(t);
};
// Generic wrapper for T which accepts a callable that returns T, taking any params, and updates it.
// Caller must use .invalidate() when the cached value could be invalid, and any subsequent calls to
// .get() will call the update() function supplied in the ctor. By default, T cached is unitialized,
// and the first call to get() will set its default value.
template <typename T, typename... Args>
struct CachedValue {
    // pass in an optional initial value, and a callable Generator
    bool                      isStale = true;
    std::function<T(Args...)> update;
    T                         cached{};

    CachedValue() = default;

    template <typename Callable>
    CachedValue(Callable&& f) : update(std::forward<Callable>(f)) {
        isStale = true;
    }

    const T&
    get(Args... vargs) & {
        if (isStale) {
            // LOG_DEBUG("isStale, updating.");
            cached = update(vargs...);
            isStale = false;
        } else {
            // LOG_DEBUG("cached! no update.");
        }
        return cached;
    }
    void
    invalidate() {
        isStale = true;
    }
};
static inline f32
randf(f32 min, f32 max) {
    return min + (random() / (f32)RAND_MAX) * (max - min);
}
static inline double
nstoms(const i64 ns) {
    return ns / 1000000.0;
}
static inline double
stons(const i64 ns) {
    return ns * 1000000000ULL;
}

template <typename T>
static inline constexpr std::string_view
pretty_type_name() {
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
