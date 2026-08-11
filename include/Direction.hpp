#pragma once 
// clang-format off
#include <flat_map>
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
constexpr inline size_t NAME##Count = static_cast<size_t>(NAME::COUNT);

// clang-format off

enum struct Direction : i8 { 
    NORTH,  // -z NORTH
    SOUTH, // +z SOUTH
    WEST,     // -x WEST
    EAST,    // +x EAST
    DOWN,     // -y
    UP,       // +y
    COUNT 
};

constexpr inline size_t Direction_Count = static_cast<size_t>(Direction ::COUNT);

// To be used like: for (const auto& dir: eachDirection)
constexpr inline std::array<Direction,6> each_direction{
    Direction ::NORTH,   //
    Direction ::SOUTH,  //
    Direction ::WEST,      //
    Direction ::EAST,//
    Direction ::DOWN, //
    Direction ::UP,             //
};
constexpr inline std::array<Direction,4> each_cardinal_direction{
    Direction ::NORTH,   //
    Direction ::SOUTH,  //
    Direction ::WEST,      //
    Direction ::EAST,//
};
constexpr inline std::array<Direction,2> each_vertical_direction{
    Direction ::DOWN, //
    Direction ::UP,
};

const static inline std::unordered_map<Direction, Direction> inverseDirection = {
    {Direction::NORTH,  Direction::SOUTH},
    {Direction::SOUTH,  Direction::NORTH},
    {Direction::WEST,        Direction::EAST},
    {Direction::EAST,        Direction::WEST},
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
constexpr inline std::array<glm::ivec2, 4> Direction_offset2D{
    // clang-format off
    glm::ivec2{  0, -1 },
    glm::ivec2{  0, +1 },
    glm::ivec2{ -1,  0 },
    glm::ivec2{ +1,  0 },
    // clang-format on
};

// clang-format off
// To be used like: for (const auto& [dir, dirOffset]: eachDirOffset)
constexpr inline std::array<std::pair<Direction, glm::ivec3>,6> eachDirOffset = {
    std::pair{ Direction ::NORTH,   Direction_offset[static_cast<i8>(Direction ::NORTH)] },
    std::pair{ Direction ::SOUTH, Direction_offset[static_cast<i8>(Direction ::SOUTH)] },
    std::pair{ Direction ::WEST,         Direction_offset[static_cast<i8>(Direction ::WEST)] },
    std::pair{ Direction ::EAST,       Direction_offset[static_cast<i8>(Direction ::EAST)] },
    std::pair{ Direction ::DOWN,         Direction_offset[static_cast<i8>(Direction ::DOWN)] },
    std::pair{ Direction ::UP,             Direction_offset[static_cast<i8>(Direction ::UP)] }, //
};
constexpr inline std::array<std::pair<Direction, glm::ivec2>,4> eachDirOffset2D = {
    std::pair{ Direction ::NORTH,      Direction_offset2D[static_cast<i8>(Direction ::NORTH)] },
    std::pair{ Direction ::SOUTH,     Direction_offset2D[static_cast<i8>(Direction ::SOUTH)] },
    std::pair{ Direction ::WEST,         Direction_offset2D[static_cast<i8>(Direction ::WEST)] },
    std::pair{ Direction ::EAST,        Direction_offset2D[static_cast<i8>(Direction ::EAST)] },
};

constexpr inline std::array<std::pair<Direction, i32>,6> eachDirIndex = {
    std::pair{ Direction ::NORTH,  static_cast<i32>(Direction ::NORTH ) },
    std::pair{ Direction ::SOUTH, static_cast<i32>(Direction ::SOUTH) },
    std::pair{ Direction ::WEST,     static_cast<i32>(Direction ::WEST    ) },
    std::pair{ Direction ::EAST,    static_cast<i32>(Direction ::EAST   ) },
    std::pair{ Direction ::DOWN,     static_cast<i32>(Direction ::DOWN    ) },
    std::pair{ Direction ::UP,       static_cast<i32>(Direction ::UP      ) },
};
constexpr inline std::array<std::pair<Direction, i32>,4> eachDirIndex2D = {
    std::pair{ Direction ::NORTH,  static_cast<i32>(Direction ::NORTH ) },
    std::pair{ Direction ::SOUTH, static_cast<i32>(Direction ::SOUTH) },
    std::pair{ Direction ::WEST,     static_cast<i32>(Direction ::WEST    ) },
    std::pair{ Direction ::EAST,    static_cast<i32>(Direction ::EAST   ) },
};
