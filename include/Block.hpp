#pragma once

#include "Types.h"
#include <array>
enum class BlockType : u8 {
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
    Block (): type(BlockType::AIR){} //NOLINT
    Block (BlockType bt): type(bt){}
    Block (const Block& rhs) = default;
    Block (Block&& rhs) = default;
    BlockType            type{ BlockType::AIR };
    constexpr inline u64 idx() const noexcept { return static_cast<i64>(type); }
    constexpr inline i64 texture_id() const noexcept {
        return idx() - 1;  // this is hacky idk why i have to do this
    }
    constexpr inline bool isOpaque() const noexcept { return blockOpacity[idx()] >= 1.0; }
    constexpr inline bool isAir() const noexcept { return type == BlockType::AIR; }

    constexpr auto operator<=>(const Block& other) const = default;

    Block& operator=(const Block& rhs)= default;
    Block& operator=(Block&& rhs){
        if (this->type==rhs.type) {
            return *this;
        }
        this->type = std::move(rhs.type);
        return *this;
    }
    Block& operator=(BlockType rhs){
        // BUG: segfault here, this is invalid

        if (this->type==rhs) {
            return *this;
        }
        this->type = rhs;
        return *this;
    }

    static constexpr inline const auto Null() { return Block{ BlockType::null }; }
    static constexpr inline const auto Empty() { return Block{ BlockType::empty }; }
};

const inline auto AIR_BLOCK = Block{ BlockType::AIR };
