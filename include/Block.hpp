#pragma once

#include <array>
#include <print>
#include <string_view>
#include <utility>

#include "CommonConcepts.hpp"
#include "Types.h"

// We use an X-macro here to keep the definition of all attributes centralized.
// It is the most flexible means to do this; later we could even swap to SOA instead of AOS if that is 
// necessary for performance (extremely doubtful but still). Main benefit is in serialization + reducing the
// amount of times names have to be repeated.
//
// Check the definition of `block_defs` array to see an application of this.
inline constexpr i32 MISSING_CUBE_TEX = 0;
inline constexpr i32 MISSING_CROSS_TEX = 0;
#define BLOCK_TYPE_LIST                                                                            \
    X(AIR, .rend_layer = BlockRenderLayer::NONE)                                                   \
    X(DIRT_BLOCK, .shape = BlockShape::CUBE, .tex_idx = 1)                                         \
    X(GRASS_BLOCK, .shape = BlockShape::CUBE, .tex_idx = 2)                                        \
    X(STONE_BLOCK, .shape = BlockShape::CUBE, .tex_idx = 3)                                        \
    X(OAK_LOG, .shape = BlockShape::CUBE, .tex_idx = 4)                                            \
    X(OAK_LEAF, .shape = BlockShape::CUBE, .tex_idx = 5, .rend_layer = BlockRenderLayer::CUTOUT)   \
    X(WATER_BLOCK, .shape = BlockShape::CUBE, .tex_idx = 6, .rend_layer = BlockRenderLayer::BLENDED, .opacity = 0.6)\
    X(SAND_BLOCK, .shape = BlockShape::CUBE, .tex_idx = 7)                                         \
    X(SANDSTONE, .shape = BlockShape::CUBE, .tex_idx = 8)                                          \
    X(DBG_OUTLINE, .shape = BlockShape::CUBE, .tex_idx = 9)                                        \
    X(SNOW_BLOCK, .shape = BlockShape::CUBE, .tex_idx = 10)                                        \
    X(PALM_LEAF, .shape = BlockShape::CUBE, .tex_idx = 11, .rend_layer = BlockRenderLayer::CUTOUT) \
    X(PALM_LOG, .shape = BlockShape::CUBE, .tex_idx = 12)                                          \
    X(SPRUCE_LEAF, .shape = BlockShape::CUBE, .tex_idx = 13, .rend_layer = BlockRenderLayer::CUTOUT)\
    X(SPRUCE_LOG, .shape = BlockShape::CUBE, .tex_idx = 14)                                        \
    X(SNOW_GRASS_BLOCK, .shape = BlockShape::CUBE, .tex_idx = 15)                                  \
    X(GRAVEL_BLOCK, .shape = BlockShape::CUBE, .tex_idx = 16)                                  \
    X(DARK_GRAVEL_BLOCK, .shape = BlockShape::CUBE, .tex_idx = 17)                                  \
    X(COBBLESTONE, .shape = BlockShape::CUBE, .tex_idx = 18)\
    X(MOSSY_COBBLESTONE, .shape = BlockShape::CUBE, .tex_idx = 19)\
    X(MOSSY_COBBLE_BOT_HALF_SLAB, .shape = BlockShape::BOT_HALF_SLAB, .tex_idx = 0)\
    X(CACTUS_ROOT, .shape = BlockShape::CACTUS, .tex_idx = 0, .rend_layer = BlockRenderLayer::CUTOUT)\
    X(CACTUS_STEM, .shape = BlockShape::CACTUS, .tex_idx = 1, .rend_layer = BlockRenderLayer::CUTOUT)\
    X(CACTUS_HEAD, .shape = BlockShape::CACTUS, .tex_idx = 2, .rend_layer = BlockRenderLayer::CUTOUT)\
                                                                                                   \
    X(GRASS_TUFT0, .shape = BlockShape::CROSS, .tex_idx = 1,                                       \
      .rend_layer = BlockRenderLayer::CUTOUT)                                                      \
    X(GRASS_TUFT1, .shape = BlockShape::CROSS, .tex_idx = 2,                                       \
      .rend_layer = BlockRenderLayer::CUTOUT)                                                      \
    X(GRASS_TUFT2, .shape = BlockShape::CROSS, .tex_idx = 3,                                       \
      .rend_layer = BlockRenderLayer::CUTOUT)                                                      \
    X(GRASS_TUFT3, .shape = BlockShape::CROSS, .tex_idx = 4,                                       \
      .rend_layer = BlockRenderLayer::CUTOUT)                                                      \
\
                                                                                                   \
    X(DEAD_BUSH, .shape = BlockShape::CROSS, .tex_idx = 5, .rend_layer = BlockRenderLayer::CUTOUT) \
    X(OAK_SAPLING, .shape = BlockShape::CROSS, .tex_idx = 6,                                       \
      .rend_layer = BlockRenderLayer::CUTOUT)                                                      \
    X(BUSH, .shape = BlockShape::CROSS, .tex_idx = 7, .rend_layer = BlockRenderLayer::CUTOUT)      \
    X(BABY_CACTUS, .shape = BlockShape::CROSS, .tex_idx = 8, .rend_layer = BlockRenderLayer::CUTOUT)\
    X(SNOW_GRASS_TUFT0, .shape = BlockShape::CROSS, .tex_idx = 9,                                  \
      .rend_layer = BlockRenderLayer::CUTOUT)                                                      \
    X(SNOW_GRASS_TUFT1, .shape = BlockShape::CROSS, .tex_idx = 10,                                 \
      .rend_layer = BlockRenderLayer::CUTOUT)                                                      \
    X(SNOW_GRASS_TUFT2, .shape = BlockShape::CROSS, .tex_idx = 11,                                 \
      .rend_layer = BlockRenderLayer::CUTOUT)                                                      \
    X(SNOW_GRASS_TUFT3, .shape = BlockShape::CROSS, .tex_idx = 12,                                 \
      .rend_layer = BlockRenderLayer::CUTOUT)                                                      \

// HACK: for the time being set OAK_LEAF to 0% transparency as i fix up some issues with pending writes

enum class BlockType : u8 {
#define X(var, ...) var,
    BLOCK_TYPE_LIST
#undef X
    COUNT,
};

// Each BlockShape has its own TextureAtlas, and thus its own tex_idx
enum struct BlockShape: i32{
    CUBE,
    CROSS, 
    CACTUS, 
    BOT_HALF_SLAB, 
};
enum struct BlockRenderLayer : u8{
    OPAQUE,             // Everything else
    CUTOUT,             // Contains 0 opacity texels which should be discarded by shader, e.g GLASS, OAK_DOOR
    BLENDED,            // Blends with background, i.e stained glass
    NONE,               // Skipped by mesher, e.g AIR
};

struct BlockDef{
    const std::string_view name;   // DEFINED BY MACRO
    const BlockType block_type;    // DEFINED BY MACRO
    const BlockShape shape{BlockShape::CUBE};
    const i32 tex_idx{numeric_min<i32>()};
    const BlockRenderLayer rend_layer{BlockRenderLayer::OPAQUE};
    const f32 opacity{1.0f};
};

inline constexpr std::array<BlockDef, std::to_underlying(BlockType::COUNT)> block_defs{
#define X(var, ...) BlockDef {.name=#var, .block_type=BlockType:: var __VA_OPT__(,) __VA_ARGS__}, 
    BLOCK_TYPE_LIST
#undef X
};


// blocks which are opaque must have opacity = 1.0f, i.e opacity=1 || blended
#define block_def(var) block_defs[std::to_underlying(BlockType:: var)]
#define X(var, ...) static_assert(block_def(var).opacity == 1.0f ||                         \
                                  block_def(var).rend_layer == BlockRenderLayer::BLENDED);

    BLOCK_TYPE_LIST
#undef X
#undef block_def






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
    constexpr BlockRenderLayer render_layer() const noexcept {
        return def().rend_layer;
    }
    constexpr BlockShape shape() const noexcept {
        return def().shape;
    }

    constexpr auto texture_id() const noexcept {
        return def().tex_idx;
    }
    constexpr auto get_opacity() const noexcept {
        return def().opacity; 
    }
    constexpr auto get_shape() const noexcept {
        return def().shape; 
    }

    constexpr auto is_opaque() const noexcept {
        return def().rend_layer==BlockRenderLayer::OPAQUE;
    }
    constexpr auto is_blended() const noexcept {
        return def().rend_layer==BlockRenderLayer::BLENDED;
    }
    constexpr auto is_cutout() const noexcept {
        return def().rend_layer==BlockRenderLayer::CUTOUT;
    }

    constexpr auto is_air() const noexcept {
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

