#pragma once 
#include <utility>

#include "Block.hpp"
#include "Breakpoints.hpp"
#include "Types.h"
// Biomes should describe ECOLOGY / climate, and feature palettes.
#define LIST_OF_BIOMES \
X(Ocean)\
X(Plains)\
X(Tundra)\
X(Desert)\
X(Beach)\

enum struct BiomeID{
#define X(val, ...) val,
LIST_OF_BIOMES
#undef X
    COUNT
};

struct BiomeDef{
    std::string_view name; // set by macro
    BiomeID biome_type; // set by macro
};

inline constexpr std::array<BiomeDef, std::to_underlying(BiomeID::COUNT)> biome_info{
#define X(var, ...) BiomeDef {.name=#var, .biome_type=BiomeID:: var},
    LIST_OF_BIOMES
#undef X
};

// FORMAT SPECIALISATION
template<> 
struct std::formatter<BiomeID>{
	constexpr auto parse(std::format_parse_context& ctx){return ctx.begin();}
	auto format(const BiomeID& val, std::format_context& ctx)const {
        const auto& info = biome_info[std::to_underlying(val)];
		return std::format_to(ctx.out(), "{}", info.name);
    }
};
