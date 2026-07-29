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


    template<BlockShape shape>
    auto quad_texture_uvs(i64 texture_idx, [[maybe_unused]]u8 dir, const_span<Vertex, 4> vertices) const{
        QuadUVList res;
        glm::vec2 uvmin{};
        if constexpr (shape == BlockShape::CROSS){
            uvmin = get_base_cross_uv(texture_idx);
        }else{
            uvmin = get_base_cube_uv(texture_idx,dir);
        }
        for (size_t vtx = 0; vtx < VTX_PER_QUAD; vtx++) {
            f32 u = vertices[vtx].tx_coords.x;
            f32 v = vertices[vtx].tx_coords.y;
            u = uvmin.x + u * abs_sprite_w;
            v = 1 - (uvmin.y + v * abs_sprite_h);
            res[vtx] = { u, v };
        }
        return res;
    }

    glm::vec2 get_base_cube_uv(i64 tex_idx, i32 dir) const;
    glm::vec2 get_base_cross_uv(i64 tex_idx) const ;
};
