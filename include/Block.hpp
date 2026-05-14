#pragma once

#include "Types.h"
#include <array>
enum class BlockType : i64 {
    null = 0,
    empty = 0,
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
    1.0,
};
static_assert(blockOpacity.size() == static_cast<size_t>(BlockType::COUNT));
struct Block {
    BlockType            id{ BlockType::AIR };
    constexpr inline u64 idx() const noexcept { return static_cast<i64>(id); }
    constexpr inline i64 texture_id() const noexcept {
        return idx() - 1;  // this is hacky idk why i have to do this
    }
    constexpr inline bool isOpaque() const noexcept { return blockOpacity[idx()] >= 1.0; }
    constexpr inline bool isAir() const noexcept { return id == BlockType::AIR; }

    constexpr auto operator<=>(const Block& other) const = default;

    Block (): id(BlockType::AIR){} //NOLINT
    Block (BlockType bt): id(bt){}
    Block (const Block& rhs) = default;
    Block (Block&& rhs) = default;
    Block& operator=(this Block& lhs, const Block& rhs){
        lhs.id=rhs.id;
        return lhs;
    }
    Block& operator=(this Block& lhs, Block&& rhs){
        if (lhs.id==rhs.id) {
            return lhs;
        }
        lhs.id=std::move(rhs.id);
        return lhs;
    }

    static constexpr inline const auto Null() { return Block{ BlockType::null }; }
    static constexpr inline const auto Empty() { return Block{ BlockType::empty }; }
};

const inline auto AIR_BLOCK = Block{ BlockType::AIR };
