#include "TextureAtlas.hpp"

TextureAtlas::TextureAtlas(const char* tex_img_path) : texture(tex_img_path, GL_RGB) {
    spriteCols = texture.pxwidth / this->spriteSize;
    spriteRows = texture.pxheight / this->spriteSize;
    abs_sprite_w = spriteSize / (f32)texture.pxwidth;
    abs_sprite_h = spriteSize / (f32)texture.pxheight;
}
glm::vec2 TextureAtlas::getTexOffset(u64 idx) {
    LOG_EXPR(idx);
    u64  x = idx % spriteCols;
    u64  y = (idx / spriteCols);
    auto v = vec2{ x, y };
    LOG_EXPR(v);
    return { x / (f32)spriteCols, y / (f32)spriteRows };
}
std::vector<glm::vec2> TextureAtlas::remapUVs(u64 texture_idx, u64 face_idx,
                                              const std::vector<Vertex>& vertices) {
    std::vector<glm::vec2> res(6, vec2{});
    LOG_EXPR(vertices);
    vec2 uvmin = getTexOffset(texture_idx * 6 + face_idx);
    for (u64 vtx = 0; vtx < 6; vtx++) {
        f32 u = vertices[vtx].txCoords.x;
        f32 v = vertices[vtx].txCoords.y;
        u = uvmin.x + u * abs_sprite_w;
        v = 1 - (uvmin.y + v * abs_sprite_h);
        res[vtx] = { u, v };
    }
    LOG_EXPR(res);
    return res;
}
