#pragma once 
// clang-format off
#include "Types.h"
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
    Direction ::RIGHT,//
    Direction ::DOWN, //
    Direction ::UP,
};

const static inline std::unordered_map<Direction, Direction> inverseDirection = {
    {Direction::FORWARD,  Direction::BACKWARD},
    {Direction::BACKWARD,  Direction::FORWARD},
    {Direction::LEFT,        Direction::RIGHT},
    {Direction::RIGHT,        Direction::LEFT},
    {Direction::DOWN,           Direction::UP},
    {Direction::UP,           Direction::DOWN},
};
const static inline std::unordered_map<i8,i8> inverseDirection_n = {
	{0,1},
	{1,0},
	{2,3},
	{3,2},
	{4,5},
	{5,4},
};

constexpr inline std::array<glm::ivec3, 6> Direction_offset{
    // clang-format off
    glm::ivec3{  0,  0, -1 },
    glm::ivec3{  0,  0, +1 },
    glm::ivec3{ -1,  0,  0 },
    glm::ivec3{ +1,  0,  0 },
    glm::ivec3{  0, -1,  0 },
    glm::ivec3{  0, +1,  0 },
    // clang-format on
};

// clang-format off
// To be used like: for (const auto& [dir, dirOffset]: eachDirOffset)
constexpr inline std::initializer_list<std::pair<Direction, const glm::ivec3&>> eachDirOffset = {
    { Direction ::FORWARD,   Direction_offset[static_cast<i8>(Direction ::FORWARD)] },
    { Direction ::BACKWARD, Direction_offset[static_cast<i8>(Direction ::BACKWARD)] },
    { Direction ::LEFT,         Direction_offset[static_cast<i8>(Direction ::LEFT)] },
    { Direction ::RIGHT,       Direction_offset[static_cast<i8>(Direction ::RIGHT)] },
    { Direction ::DOWN,         Direction_offset[static_cast<i8>(Direction ::DOWN)] },
    { Direction ::UP,             Direction_offset[static_cast<i8>(Direction ::UP)] }, //
};

constexpr inline std::initializer_list<std::pair<Direction, i32>> eachDirIndex = {
    { Direction ::FORWARD,  static_cast<i32>(Direction ::FORWARD ) },
    { Direction ::BACKWARD, static_cast<i32>(Direction ::BACKWARD) },
    { Direction ::LEFT,     static_cast<i32>(Direction ::LEFT    ) },
    { Direction ::RIGHT,    static_cast<i32>(Direction ::RIGHT   ) },
    { Direction ::DOWN,     static_cast<i32>(Direction ::DOWN    ) },
    { Direction ::UP,       static_cast<i32>(Direction ::UP      ) },
};
