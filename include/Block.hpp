#pragma once

#include "Types.h"
#include <array>
enum class BlockType : i64 {
    null = -1,
    empty = -1,
    AIR = 0,
    DIRT_BLOCK,
    GRASS_BLOCK,
    STONE_BLOCK,
    COUNT,
};
template <typename Attr_t>
using BlockAttribArray = std::array<Attr_t, static_cast<size_t>(BlockType::COUNT)>;

inline BlockAttribArray<f32> blockOpacity = {
    0.0,
    1.0,
    1.0,
};
struct Block {
    BlockType id{ BlockType::AIR };
    constexpr inline u64
    idx() const noexcept {
        return static_cast<i64>(id);
    }
    constexpr inline i64
    texture_id() const noexcept {
        return idx() - 1;  // this is hacky
    }
    constexpr inline bool
    isOpaque() const noexcept {
        return blockOpacity[idx()] >= 1.0;
    }
    constexpr inline bool
    isAir() const noexcept {
        return id == BlockType::AIR;
    }

    constexpr auto operator<=>(const Block& other) const = default;

    static constexpr inline const auto
    Null() {
        return Block{ .id = BlockType::null };
    }
    static constexpr inline const auto
    Empty() {
        return Block{ .id = BlockType::empty };
    }
};

const inline auto AIR_BLOCK = Block{ .id = BlockType::AIR };
