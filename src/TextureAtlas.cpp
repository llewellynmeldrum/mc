
#include "CommonConcepts.hpp"
#include "TextureAtlas.hpp"
#include "CommonUtils.hpp"
#include "GlobalDebugLog.hpp"
#include "glbindingWrapper.hpp"
#include "Breakpoints.hpp"
#include "ChunkConstants.hpp"
#include "FormatSpecs.hpp"
#include "Logger.hpp"
#include <unordered_map>
#include <utility>
#include "Assertion.hpp"

using namespace glm;

void TextureAtlas::load_texture(const char* tex_img_path){
    texture.load(tex_img_path, { 0, 1, 0, 1 },true);
    LOG_DEBUG("Set up texture atlas ({})",tex_img_path);
    spriteCols = texture.pxwidth / this->sprite_sz_px;
    spriteRows = texture.pxheight / this->sprite_sz_px;
    abs_sprite_w = sprite_sz_px / (f32)texture.pxwidth;
    abs_sprite_h = sprite_sz_px / (f32)texture.pxheight;
}

constexpr inline std::array<i64, std::to_underlying(Direction::COUNT)> cube_tex_offset_per_direction {
    0,
    0,
    0,
    0,
    1,
    2,
};

glm::vec2 TextureAtlas::get_base_cube_uv(i64 tex_idx, i32 dir) const {
    constexpr static i64 PER_BLOCK_TEX_COUNT = 3;
    assert(dir >=0);
    assert(dir < cube_tex_offset_per_direction.size());
    i64 offset = cube_tex_offset_per_direction[dir];
    i64 idx = tex_idx * PER_BLOCK_TEX_COUNT + offset;
    i64 x = idx % spriteCols;
    i64 y = (idx / spriteCols);
    return { x / (f32)spriteCols, y / (f32)spriteRows };
}
glm::vec2 TextureAtlas::get_base_cross_uv(i64 idx) const {
    i64 x = idx % spriteCols;
    i64 y = (idx / spriteCols);
    return { x / (f32)spriteCols, y / (f32)spriteRows };
}

