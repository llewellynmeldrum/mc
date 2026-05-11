#pragma once
#include "Types.h"
#include <cstdlib>
#include <functional>
#include <string_view>
#include <type_traits>
#include "glmWrapper.hpp"

// clang-format off
#define DECL_SMART_ENUM(NAME, UNDERLYING_T, ...) \
enum struct NAME : UNDERLYING_T { __VA_ARGS__, COUNT };\
\
constexpr inline NAME NAME##_from(UNDERLYING_T e) noexcept{\
    return static_cast<NAME>(e);\
};\
constexpr inline UNDERLYING_T to_##NAME(NAME e) noexcept{\
    return static_cast<UNDERLYING_T>(e);\
};\
constexpr inline std::size_t NAME##Count = static_cast<std::size_t>(NAME::COUNT);

// clang-format off
DECL_SMART_ENUM(Direction, i8, FORWARD, BACKWARD, LEFT, RIGHT, DOWN, UP)

// To be used like: for (const auto& dir: eachDirection)
constexpr inline std::initializer_list<Direction> eachDirection{
    Direction ::FORWARD,   //
    Direction ::BACKWARD,  //
    Direction ::LEFT,      //
    Direction ::RIGHT,     //
    Direction ::DOWN,      //
    Direction ::UP,
};

const static inline std::unordered_map<Direction, Direction> inverseDirection = {
	{Direction::FORWARD,  Direction::BACKWARD},
	{Direction::BACKWARD, Direction::FORWARD},
	{Direction::LEFT,     Direction::RIGHT},
	{Direction::RIGHT,    Direction::LEFT},
	{Direction::DOWN,     Direction::UP},
	{Direction::UP,       Direction::DOWN},
};
const static inline std::unordered_map<i8,i8> inverseDirection_n = {
	{0,1},
	{1,0},
	{2,3},
	{3,2},
	{4,5},
	{5,4},
};

constexpr inline std::array<ivec3, 6> Direction_offset{
    // clang-format off
    ivec3{  0,  0, -1 },
    ivec3{  0,  0, +1 },
    ivec3{ -1,  0,  0 },
    ivec3{ +1,  0,  0 },
    ivec3{  0, -1,  0 },
    ivec3{  0, +1,  0 },
    // clang-format on
};

// clang-format off
// To be used like: for (const auto& [dir, dirOffset]: eachDirOffset)
constexpr inline std::initializer_list<std::pair<Direction, const ivec3&>> eachDirOffset = {
    { Direction ::FORWARD,  Direction_offset[static_cast<i8>(Direction ::FORWARD)] },
    { Direction ::BACKWARD, Direction_offset[static_cast<i8>(Direction ::BACKWARD)] },
    { Direction ::LEFT,     Direction_offset[static_cast<i8>(Direction ::LEFT)] },
    { Direction ::RIGHT,    Direction_offset[static_cast<i8>(Direction ::RIGHT)] },
    { Direction ::DOWN,     Direction_offset[static_cast<i8>(Direction ::DOWN)] },
    { Direction ::UP,       Direction_offset[static_cast<i8>(Direction ::UP)] },  //
};

constexpr inline std::initializer_list<std::pair<Direction, i32>> eachDirIndex = {
    { Direction ::FORWARD,  static_cast<i32>(Direction ::FORWARD ) },
    { Direction ::BACKWARD, static_cast<i32>(Direction ::BACKWARD) },
    { Direction ::LEFT,     static_cast<i32>(Direction ::LEFT    ) },
    { Direction ::RIGHT,    static_cast<i32>(Direction ::RIGHT   ) },
    { Direction ::DOWN,     static_cast<i32>(Direction ::DOWN    ) },
    { Direction ::UP,       static_cast<i32>(Direction ::UP      ) },
};
// clang-format on

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
