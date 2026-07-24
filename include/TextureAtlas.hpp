#pragma once

#include "CommonConcepts.hpp"
#include "CommonUtils.hpp"
#include "Texture2D.hpp"
#include "Vertex.hpp"
#include "Direction.hpp"
#include <string>

using QuadUVList = std::array<glm::vec2, 4>;
struct TextureAtlas {
    TextureAtlas() = default;
    ~TextureAtlas() = default;
    TextureAtlas(const char* tex_img_path) { load_texture(tex_img_path); }
    void load_texture(const char* tex_img_path);

    std::string textureImagePath{};

    Texture2D texture;

    u64 spriteCols;
    u64 spriteRows;
    f32 abs_sprite_w;
    f32 abs_sprite_h;

    static constexpr i64     sprite_sz_px = 16;

    std::array<glm::vec2, 4> get_texture_uvs_cross(i64 texture_idx, const_span<Vertex, 4> vertices) const;
    std::array<glm::vec2, 4> apply_texture_uvs_cube(i64 texture_idx, Direction dir, const_span<Vertex, 4> vertices) const ;

    glm::vec2 get_base_cube_uv(i64 tex_idx, i32 dir) const;
    glm::vec2 get_base_cross_uv(i64 tex_idx) const ;
};
