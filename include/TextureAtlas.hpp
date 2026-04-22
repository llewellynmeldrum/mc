#pragma once 

#include "Texture2D.hpp"
#include "Vertex.hpp"
#include <string>

struct TextureAtlas{
    TextureAtlas(const char* tex_img_path);
    ~TextureAtlas()=default;
    std::string textureImagePath{};

    void setupTextureAtlas();
    Texture2D texture;
    u64 spriteCols;
    u64 spriteRows;
    f32 abs_sprite_w;
    f32 abs_sprite_h;
    constexpr static u64 spriteSize = 16;
    vec2 getTexOffset(u64 idx)const;
    std::vector<glm::vec2> remapUVs(u64 texture_idx, u64 face_idx, const std::vector<Vertex>& vertices) const;
};
