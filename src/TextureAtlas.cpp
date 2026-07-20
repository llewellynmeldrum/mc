
#include "CommonConcepts.hpp"
#include "TextureAtlas.hpp"
#include "CommonUtils.hpp"
#include "glbindingWrapper.hpp"
#include "Breakpoints.hpp"
#include "ChunkConstants.hpp"
#include "FormatSpecs.hpp"
#include "Logger.hpp"
#include <unordered_map>
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
static std::unordered_map<Direction, size_t> cube_tex_offset_per_direction = {
    // clang-format off
    {Direction::FORWARD, 0},
    {Direction::BACKWARD,0},
    {Direction::LEFT,    0},
    {Direction::RIGHT,   0},
    {Direction::DOWN,    1},
    {Direction::UP,      2},
    // clang-format on
};

glm::vec2 TextureAtlas::get_base_cube_uv(i64 tex_idx, Direction dir) const {
    constexpr static i64 PER_BLOCK_TEX_COUNT = 3;
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

QuadUVList TextureAtlas::apply_texture_uvs_cube(i64 texture_idx, Direction dir, const_span<Vertex, 4> vertices) const {
    vec2 uvmin = get_base_cube_uv(texture_idx, dir);
    std::array<vec2, 4> res;
    for (size_t vtx = 0; vtx < VTX_PER_QUAD; vtx++) {
        f32 u = vertices[vtx].txCoords.x;
        f32 v = vertices[vtx].txCoords.y;
        u = uvmin.x + u * abs_sprite_w;
        v = 1 - (uvmin.y + v * abs_sprite_h);
        res[vtx] = { u, v };
    }
    return res;
}

QuadUVList TextureAtlas::apply_texture_uvs_cross(i64 texture_idx, const_span<Vertex, 4> vertices) const {
    QuadUVList res;

    vec2 uvmin = get_base_cross_uv(texture_idx);
    for (size_t vtx = 0; vtx < VTX_PER_QUAD; vtx++) {
        f32 u = vertices[vtx].txCoords.x;
        f32 v = vertices[vtx].txCoords.y;
        u = uvmin.x + u * abs_sprite_w;
        v = 1 - (uvmin.y + v * abs_sprite_h);
        res[vtx] = { u, v };
    }
    return res;
}
