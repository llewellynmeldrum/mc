#pragma once

#include "Types.h"
#include <array>
#include <string_view>
#include <utility>



// We use an X-macro here to keep the definition of all attributes centralized.
// It is the most flexible means to do this; later we could even swap to SOA instead of AOS if that is 
// necessary for performance (extremely doubtful but still). Main benefit is in serialization + reducing the
// amount of times names have to be repeated.
//
// Check the definition of `block_defs` array to see an application of this.
#define BLOCK_TYPE_LIST \
    X(AIR, .opacity = 0.0f)              \
    X(DIRT_BLOCK, .opacity = 1.0f)       \
    X(GRASS_BLOCK, .opacity = 1.0f)      \
    X(STONE_BLOCK, .opacity = 1.0f)      \
    X(WATER_BLOCK, .opacity = 0.6f)      \
    X(OAK_LOG, .opacity = 1.0f)          \
    X(OAK_LEAF, .opacity = 1.0f) 

// HACK: for the time being set OAK_LEAF to 0% transparency as i fix up some issues with pending writes

enum class BlockType : u8 {
#define X(var, ...) var,
    BLOCK_TYPE_LIST
#undef X
    COUNT,
};

struct BlockDef{
    const std::string_view name; // defined by macro
    const BlockType block_type;       // defined by macro
    const f32 opacity{1.0f};
};
inline constexpr std::array<BlockDef, std::to_underlying(BlockType::COUNT)> block_defs{
#define X(var, ...) BlockDef {.name=#var, .block_type=BlockType:: var __VA_OPT__(,) __VA_ARGS__},
    BLOCK_TYPE_LIST
#undef X
};
// i would like to have some list of blocks which contains all of their data, for example:







struct Block {
    Block ():type(BlockType::AIR){}; // NOLINT
    Block (BlockType rhs):type(rhs){}; // NOLINT
    Block (const Block& rhs) = default;

    Block (Block&& rhs) = default;

    BlockType type{ BlockType::AIR };



    Block& operator=(const Block& rhs)= default;

    Block& operator=(Block&& rhs){
        if (this->type==rhs.type) {
            return *this;
        }
        this->type = std::move(rhs.type);
        return *this;
    }

    Block& operator=(BlockType rhs){
        if (this->type==rhs) {
            return *this;
        }
        this->type = rhs;
        return *this;
    }
    auto operator<=>(const Block& rhs)const noexcept = default;

    // =========
    // Helpers
    // =========
    constexpr inline i64 texture_id() const noexcept {
        return idx() - 1;  // this is hacky idk why i have to do this
    }
    constexpr float getOpacity() const noexcept {
        return def().opacity; 
    }
    constexpr bool isOpaque() const noexcept {
        return getOpacity() >= 1.0f;
    }
    constexpr bool isTransparent() const noexcept {
        return getOpacity() < 1.0; 
    }
    constexpr bool isAir() const noexcept {
       return type==BlockType::AIR; 
    }

    // =========
    // Meta-Helpers
    // =========
    constexpr const BlockDef& def() const noexcept{
        return block_defs[idx()]; 
    }
    constexpr inline u64 idx() const noexcept { 
        return std::to_underlying(type); 
    }
    constexpr inline std::string_view to_string() const noexcept{
        return def().name;
    }
};

