#pragma once 

#include "Texture2D.hpp"
#include "Vertex.hpp"
// owns a texture
// TODO: Continue making all existing classes out of line to fix include bugs
struct TextureAtlas{
    Texture2D texture;
    u64 spriteCols;
    u64 spriteRows;
    f32 abs_sprite_w;
    f32 abs_sprite_h;
    constexpr static u64 spriteSize = 16;
    TextureAtlas(const char* tex_img_path);
    vec2 getTexOffset(u64 idx);
    std::vector<glm::vec2> remapUVs(u64 texture_idx, u64 face_idx,
                                    const std::vector<Vertex>& vertices);
};
extern TextureAtlas atlas;
