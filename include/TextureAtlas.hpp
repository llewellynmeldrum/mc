#pragma once 

#include "CommonUtils.hpp"
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
    constexpr static i64 spriteSize = 16;
    glm::vec2 getTexOffset(i64 tex_idx, Direction dir) const ;
    std::vector<glm::vec2> remapUVs(i64 texture_idx, Direction dir,
                                              const std::vector<Vertex>& vertices) const;
};
