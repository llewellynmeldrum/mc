#include "TextureAtlas.hpp"
#include "CommonUtils.hpp"
#include "glbindingWrapper.hpp"
#include "DEBUG.hpp"
#include "DebugFormatSpecializations.hpp"
#include "Logger.hpp"

void TextureAtlas::setupTextureAtlas(){
    texture = Texture2D(textureImagePath.c_str(), to_i32(gl::GL_RGB), {0,1,0,1});
    texture.setup();
    LOG_DEBUG("Set up texture atlas");
    spriteCols = texture.pxwidth / this->spriteSize;
    spriteRows = texture.pxheight / this->spriteSize;
    abs_sprite_w = spriteSize / (f32)texture.pxwidth;
    abs_sprite_h = spriteSize / (f32)texture.pxheight;
    LOG_DEBUG("init texture atlas");
}
TextureAtlas::TextureAtlas(const char* tex_img_path) : textureImagePath(tex_img_path){
}
constexpr i64 CUBE_FACE_COUNT = 6;
glm::vec2 TextureAtlas::getTexOffset(i64 tex_idx, Direction dir) const {
    i64 idx = tex_idx * CUBE_FACE_COUNT + static_cast<i8>(dir);
    i64  x = idx % spriteCols;
    i64  y = (idx / spriteCols);
    vec2 v = vec2{ x, y };
    return { x / (f32)spriteCols, y / (f32)spriteRows };
}
std::vector<glm::vec2> TextureAtlas::remapUVs(i64 texture_idx, Direction dir,
                                              const std::vector<Vertex>& vertices) const{
    std::vector<glm::vec2> res(6, vec2{});
    vec2 uvmin = getTexOffset(texture_idx, dir);
    for (i64 vtx = 0; vtx < 6; vtx++) {
        f32 u = vertices[vtx].txCoords.x;
        f32 v = vertices[vtx].txCoords.y;
        u = uvmin.x + u * abs_sprite_w;
        v = 1 - (uvmin.y + v * abs_sprite_h);
        res[vtx] = { u, v };
    }
    return res;
}
