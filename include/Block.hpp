#pragma once

#include <array>
#include <print>
#include <string_view>
#include <utility>

#include "BlockLight.hpp"
#include "Direction.hpp"
#include "UnpackedLightValue.hpp"
#include "CommonConcepts.hpp"
#include "Types.h"
#include "EnumMap.hpp"
#include "glm/fwd.hpp"

constexpr BlockLight leaf_absorp{8,8,8};
// We use an X-macro here to keep the definition of all attributes centralized.
// It is the most flexible means to do this; later we could even swap to SOA instead of AOS if that is 
// necessary for performance (extremely doubtful but still). Main benefit is in serialization + reducing the
// amount of times names have to be repeated.
//
// Check the definition of `block_defs` array to see an application of this.
inline constexpr i32 MISSING_CUBE_TEX = 0;
inline constexpr i32 MISSING_CROSS_TEX = 0;
#define BLOCK_TYPE_LIST                                                                            \
    X(AIR, .rend_layer = BlockRenderLayer::NONE, .absorptance = {1,1,1})                              \
    X(DIRT_BLOCK, .shape = BlockShape::CUBE, .tex_idx = 1)                                         \
    X(GRASS_BLOCK, .shape = BlockShape::CUBE, .tex_idx = 2)                                        \
    X(STONE_BLOCK, .shape = BlockShape::CUBE, .tex_idx = 3)                                        \
    X(OAK_LOG, .shape = BlockShape::CUBE, .tex_idx = 4)                                            \
    X(OAK_LEAF, .shape = BlockShape::CUBE, .tex_idx = 5, .rend_layer = BlockRenderLayer::CUTOUT,   \
      .absorptance = leaf_absorp)                                                                         \
    X(WATER_BLOCK, .shape = BlockShape::CUBE, .tex_idx = 6,                                        \
      .rend_layer = BlockRenderLayer::BLENDED, .opacity = 155, .absorptance = {5,5,5})               \
    X(SAND_BLOCK, .shape = BlockShape::CUBE, .tex_idx = 7)                                         \
    X(SANDSTONE, .shape = BlockShape::CUBE, .tex_idx = 8)                                          \
    X(DBG_OUTLINE, .shape = BlockShape::CUBE, .tex_idx = 9)                                        \
    X(SNOW_BLOCK, .shape = BlockShape::CUBE, .tex_idx = 10)                                        \
    X(SNOW_1, .shape = BlockShape::SNOW_1_15, .tex_idx = 10   ,.absorptance = {3,3,3} )                                       \
    X(SNOW_2, .shape = BlockShape::SNOW_2_15, .tex_idx = 10   ,.absorptance = {3,3,3} )                                       \
    X(SNOW_3, .shape = BlockShape::SNOW_3_15, .tex_idx = 10   ,.absorptance = {3,3,3} )                                       \
    X(SNOW_4, .shape = BlockShape::SNOW_4_15, .tex_idx = 10   ,.absorptance = {3,3,3} )                                       \
    X(SNOW_5, .shape = BlockShape::SNOW_5_15, .tex_idx = 10   ,.absorptance = {3,3,3} )                                       \
    X(SNOW_6, .shape = BlockShape::SNOW_6_15, .tex_idx = 10   ,.absorptance = {3,3,3} )                                       \
    X(SNOW_7, .shape = BlockShape::SNOW_7_15, .tex_idx = 10   ,.absorptance = {3,3,3} )                                       \
    X(SNOW_8, .shape = BlockShape::SNOW_8_15, .tex_idx = 10   ,.absorptance = {3,3,3} )                                       \
    X(SNOW_9, .shape = BlockShape::SNOW_9_15, .tex_idx = 10   ,.absorptance = {3,3,3} )                                       \
    X(SNOW_10, .shape = BlockShape::SNOW_10_15, .tex_idx = 10 ,.absorptance = {3,3,3} )                                     \
    X(SNOW_11, .shape = BlockShape::SNOW_11_15, .tex_idx = 10 ,.absorptance = {3,3,3} )                                     \
    X(SNOW_12, .shape = BlockShape::SNOW_12_15, .tex_idx = 10 ,.absorptance = {3,3,3} )                                     \
    X(SNOW_13, .shape = BlockShape::SNOW_13_15, .tex_idx = 10 ,.absorptance = {3,3,3} )                                     \
    X(SNOW_14, .shape = BlockShape::SNOW_14_15, .tex_idx = 10 ,.absorptance = {3,3,3} )                                     \
    X(SNOW_15, .shape = BlockShape::SNOW_15_15, .tex_idx = 10 ,.absorptance = {3,3,3} )                                     \
    X(PALM_LEAF, .shape = BlockShape::CUBE, .tex_idx = 11, .rend_layer = BlockRenderLayer::CUTOUT, .absorptance = leaf_absorp) \
    X(PALM_LOG, .shape = BlockShape::CUBE, .tex_idx = 12)                                          \
    X(SPRUCE_LEAF, .shape = BlockShape::CUBE, .tex_idx = 13,                                       \
      .rend_layer = BlockRenderLayer::CUTOUT, .absorptance = leaf_absorp)                                                      \
    X(SPRUCE_LOG, .shape = BlockShape::CUBE, .tex_idx = 14)                                        \
    X(SNOW_GRASS_BLOCK, .shape = BlockShape::CUBE, .tex_idx = 15)                                  \
    X(GRAVEL_BLOCK, .shape = BlockShape::CUBE, .tex_idx = 16)                                      \
    X(DARK_GRAVEL_BLOCK, .shape = BlockShape::CUBE, .tex_idx = 17)                                 \
    X(COBBLESTONE, .shape = BlockShape::CUBE, .tex_idx = 18)                                       \
    X(MOSSY_COBBLESTONE, .shape = BlockShape::CUBE, .tex_idx = 19)                                 \
    X(MOSSY_COBBLE_BOT_HALF_SLAB, .shape = BlockShape::BOT_HALF_SLAB, .tex_idx = 0)                \
    X(CACTUS_ROOT, .shape = BlockShape::CACTUS, .tex_idx = 0,                                      \
      .rend_layer = BlockRenderLayer::CUTOUT, .absorptance = {7,7,7})                                                      \
    X(CACTUS_STEM, .shape = BlockShape::CACTUS, .tex_idx = 1,                                      \
      .rend_layer = BlockRenderLayer::CUTOUT, .absorptance = {7,7,7})                                                      \
    X(CACTUS_HEAD, .shape = BlockShape::CACTUS, .tex_idx = 2,                                      \
      .rend_layer = BlockRenderLayer::CUTOUT, .absorptance = {7,7,7})                                                      \
    X(ICE_BLOCK, .shape = BlockShape::CUBE, .tex_idx = 20,                                         \
      .rend_layer = BlockRenderLayer::BLENDED, .absorptance = {9,9,9})                                                     \
    X(A_BLOCK, .shape = BlockShape::CUBE, .tex_idx = 21, .rend_layer=BlockRenderLayer::CUTOUT)                                         \
    X(B_BLOCK, .shape = BlockShape::CUBE, .tex_idx = 22, .rend_layer=BlockRenderLayer::CUTOUT)                                         \
    X(C_BLOCK, .shape = BlockShape::CUBE, .tex_idx = 23, .rend_layer=BlockRenderLayer::CUTOUT)                                         \
    \
    X(GRASS_TUFT0, .shape = BlockShape::CROSS, .tex_idx = 1,                                       \
      .rend_layer = BlockRenderLayer::CUTOUT, .absorptance = {1,1,1})                                                      \
    X(GRASS_TUFT1, .shape = BlockShape::CROSS, .tex_idx = 2,                                       \
      .rend_layer = BlockRenderLayer::CUTOUT, .absorptance = {1,1,1})                                                      \
    X(GRASS_TUFT2, .shape = BlockShape::CROSS, .tex_idx = 3,                                       \
      .rend_layer = BlockRenderLayer::CUTOUT, .absorptance = {1,1,1})                                                      \
    X(GRASS_TUFT3, .shape = BlockShape::CROSS, .tex_idx = 4,                                       \
      .rend_layer = BlockRenderLayer::CUTOUT, .absorptance = {1,1,1})                                                      \
                                                                                                   \
    X(DEAD_BUSH, .shape = BlockShape::CROSS, .tex_idx = 5, .rend_layer = BlockRenderLayer::CUTOUT, .absorptance = {3,3,3}) \
    X(OAK_SAPLING, .shape = BlockShape::CROSS, .tex_idx = 6,                                       \
      .rend_layer = BlockRenderLayer::CUTOUT, .absorptance = {3,3,3})                                                      \
    X(BUSH, .shape = BlockShape::CROSS, .tex_idx = 7, .rend_layer = BlockRenderLayer::CUTOUT, .absorptance = {4,4,4})      \
    X(BABY_CACTUS, .shape = BlockShape::CROSS, .tex_idx = 8,                                       \
      .rend_layer = BlockRenderLayer::CUTOUT, .absorptance = {3,3,3})                                                      \
    X(SNOW_GRASS_TUFT0, .shape = BlockShape::CROSS, .tex_idx = 9,                                  \
      .rend_layer = BlockRenderLayer::CUTOUT, .absorptance = {3,3,3})                                                      \
    X(SNOW_GRASS_TUFT1, .shape = BlockShape::CROSS, .tex_idx = 10,                                 \
      .rend_layer = BlockRenderLayer::CUTOUT, .absorptance = {3,3,3})                                                      \
    X(SNOW_GRASS_TUFT2, .shape = BlockShape::CROSS, .tex_idx = 11,                                 \
      .rend_layer = BlockRenderLayer::CUTOUT, .absorptance = {3,3,3})                                                      \
    X(SNOW_GRASS_TUFT3, .shape = BlockShape::CROSS, .tex_idx = 12,                                 \
      .rend_layer = BlockRenderLayer::CUTOUT, .absorptance = {3,3,3})                                                      \
    X(RED_FLOWER, .shape = BlockShape::CROSS, .tex_idx = 13,                                       \
      .rend_layer = BlockRenderLayer::CUTOUT )                                                      \
    X(YELLOW_FLOWER, .shape = BlockShape::CROSS, .tex_idx = 14,                                    \
      .rend_layer = BlockRenderLayer::CUTOUT )                                                      \
    X(TORCH, .shape = BlockShape::CROSS, .tex_idx = 15, .rend_layer = BlockRenderLayer::CUTOUT,    \
       .absorptance={1,1,1}, .emission = { 15, 14, 8 })

enum class BlockType : u8 {
#define X(var, ...) var,
    BLOCK_TYPE_LIST
#undef X
    COUNT,
};
#define SNOW_SHAPE_LIST \
    X(SNOW_1_15) \
    X(SNOW_2_15) \
    X(SNOW_3_15) \
    X(SNOW_4_15) \
    X(SNOW_5_15) \
    X(SNOW_6_15) \
    X(SNOW_7_15) \
    X(SNOW_8_15) \
    X(SNOW_9_15) \
    X(SNOW_10_15)\
    X(SNOW_11_15)\
    X(SNOW_12_15)\
    X(SNOW_13_15)\
    X(SNOW_14_15)\
    X(SNOW_15_15)


// Each BlockShape has its own TextureAtlas, and thus its own tex_idx
enum struct BlockShape: i32{
    CUBE,
    CROSS, 
    CACTUS, 
    BOT_HALF_SLAB, 
    TOP_HALF_SLAB, 
    #define X(var) var,
        SNOW_SHAPE_LIST
    #undef X
    COUNT,
};

constexpr inline EnumMap<BlockShape, size_t> block_shape_to_texture_atlas{
    {BlockShape::CUBE, 0}, 
    {BlockShape::CROSS, 1},
    {BlockShape::CACTUS, 2},
    {BlockShape::BOT_HALF_SLAB, 3},
    {BlockShape::TOP_HALF_SLAB, 3},
    #define X(var) {BlockShape::var, 0},
        SNOW_SHAPE_LIST
    #undef X
};

template<size_t N>
constexpr inline BlockShape shape_of_snow_level{ static_cast<BlockShape>(std::to_underlying(BlockShape::SNOW_1_15) + N-1) };

// for whatever reason calls to the above map's `.at()` are not considered constant expressions.
template<BlockShape shape>
constexpr inline size_t shape_atlas_id {0};

template<> constexpr inline size_t shape_atlas_id<BlockShape::CUBE> {0};
template<> constexpr inline size_t shape_atlas_id<BlockShape::CROSS> {1};
template<> constexpr inline size_t shape_atlas_id<BlockShape::CACTUS> {2};
template<> constexpr inline size_t shape_atlas_id<BlockShape::BOT_HALF_SLAB> {3};
template<> constexpr inline size_t shape_atlas_id<BlockShape::TOP_HALF_SLAB> {3};
#define X(var) template<> constexpr inline size_t shape_atlas_id<BlockShape::var> {0};
    SNOW_SHAPE_LIST
#undef X



constexpr inline size_t N_SNOW_SHAPE_STAGES = 15;
// NOTE: MAKE SURE TO MIRROR THIS VALUE IN GLSL UNTIL I GET A GLSL&C++ COMPATIBLE HEADER WORKING
#define TEXTURE_ID_COUNT 4


enum struct BlockRenderLayer : u8{
    OPAQUE,             // Everything else
    CUTOUT,             // Contains 0 opacity texels which should be discarded by shader, e.g GLASS, OAK_DOOR
    BLENDED,            // Blends with background, i.e stained glass
    NONE,               // Skipped by mesher, e.g AIR
};

struct RGB{
    constexpr RGB()= default;
    constexpr RGB(u8 _r, u8 _g, u8 _b)
        : r(_r)
        , g(_g)
        , b(_b)
    {}
    union{
        struct{
            u8 r{};
            u8 g{};
            u8 b{};
        };
        std::array<u8,3> data;
    };
};
struct Light{
    constexpr Light()=default;
    constexpr Light(u8 _r, u8 _g, u8 _b)
        : rgb(_r,_g,_b)
    {}
    RGB rgb;
};

#define BLOCKDEF_FIELD_LIST \
name\
block_type\
shape\
tex_idx\
rend_layer\
opacity\
absorptance\
emission

struct BlockDef{
    const std::string_view name;   // DEFINED BY MACRO
    const BlockType block_type;    // DEFINED BY MACRO
    const BlockShape shape{BlockShape::CUBE};
    const i32 tex_idx{numeric_min<i32>()};
    const BlockRenderLayer rend_layer{BlockRenderLayer::OPAQUE};
    const u8 opacity{255};
    const BlockLight absorptance{15,15,15}; // light = prev * absorptance
    const BlockLight emission{0,0,0};
};

inline constexpr std::array<BlockDef, std::to_underlying(BlockType::COUNT)> block_defs{
#define X(var, ...) BlockDef {.name=#var, .block_type=BlockType:: var __VA_OPT__(,) __VA_ARGS__}, 
    BLOCK_TYPE_LIST
#undef X
};


// blocks which are opaque must have opacity = 1.0f, i.e opacity=1 || blended
#define block_def(var) block_defs[std::to_underlying(BlockType:: var)]
#define X(var, ...) static_assert(block_def(var).opacity == 255 ||                         \
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
    constexpr auto absorptance() const noexcept {
        return def().absorptance; 
    }
    // Air has no sunlight absorptance.
    constexpr auto sunlight_ray_absorptance() const noexcept {
        return type == BlockType::AIR ? u8{0} : def().absorptance.avg(); 
    }
    constexpr auto get_shape() const noexcept {
        return def().shape; 
    }
    constexpr auto get_emission() const noexcept {
        return def().emission; 
    }
    constexpr auto is_light_source() const noexcept {
        return def().emission.is_nonzero();
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

    constexpr auto not_air() const noexcept {
       return type!=BlockType::AIR; 
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

//    CUBE,
//    CROSS, 
//    CACTUS, 
//    BOT_HALF_SLAB, 
//    TOP_HALF_SLAB, 
//    #define X(var) var,
//        SNOW_SHAPE_LIST
//    #undef X
//    COUNT,
//};

template<BlockShape shape>
constexpr bool is_snow_blockshape(){
    return std::to_underlying(BlockType::SNOW_1) <=  std::to_underlying(shape) && 
           std::to_underlying(shape) <= std::to_underlying(BlockShape::SNOW_1_15);
}
template<BlockShape from_shape, BlockShape to_shape, Direction dir>
constexpr bool shape_covers_face() noexcept {
    if constexpr (dir == Direction ::NORTH){
        // only cubes fully occlude the forward face  
        return from_shape == BlockShape::CUBE;

    } else if constexpr (dir == Direction ::SOUTH){
        // only cubes fully occlude the backward face  
        return from_shape == BlockShape::CUBE;

	} else if constexpr (dir == Direction ::WEST){
        // only cubes fully occlude the lhs face  
        return from_shape == BlockShape::CUBE;

	} else if constexpr (dir == Direction ::EAST){
        // only cubes fully occlude the rhs face  
        return from_shape == BlockShape::CUBE;

	}else if constexpr (dir == Direction ::DOWN){
        // a cube, bhs, or snow block occludes EVERY face below it `
        bool occludes_any_neighbour = 
                from_shape == BlockShape::CUBE || 
               from_shape == BlockShape::BOT_HALF_SLAB 
                || is_snow_blockshape<from_shape>();

        // a cactus occludes only cactus below it
        bool occludes_specific_neighbour = 
            (from_shape == BlockShape::CACTUS && from_shape == to_shape);
        return occludes_any_neighbour || occludes_specific_neighbour;
	} else if constexpr (dir == Direction ::UP){

	} 
}
