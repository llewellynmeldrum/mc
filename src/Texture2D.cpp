#include "glbindingWrapper.hpp"
using namespace gl;

#include "Texture2D.hpp"
#include "Logger.hpp"
#include "glmWrapper.hpp"
#include "GlobalDebugLog.hpp"
i64 texture_count = 0;

using namespace glm;

// assumes 8 bit rgb or rgba
void Texture2D::load(const char* tex_path, vec4 border_color, bool has_alpha){
    auto gpu_pixel_fmt = has_alpha ? GL_RGBA8 : GL_RGB8;
    auto cpu_pixel_fmt = has_alpha ? GL_RGBA : GL_RGB;
    u8* tex_pixels = stbi_load(tex_path, &pxwidth, &pxheight, &nchannels, STBI_rgb_alpha);
    if (!tex_pixels) {
        LOG_ERROR("Failed to load texture file '{}'.", tex_path);
        LOG_EXIT(EXIT_FAILURE);
        return;
    }
//    LOG_EXPR(pxwidth);
//    LOG_EXPR(pxheight);
//    LOG_EXPR(nchannels);
//    LOG_EXPR(tex_pixels);
    idx = texture_count++;
    u8 min_alpha = 255;
    u8 max_alpha = 0;

    for (int i = 0; i < pxwidth * pxheight; ++i) {
        u8 alpha = tex_pixels[i * 4 + 3];
        min_alpha = min(min_alpha, alpha);
        max_alpha = max(max_alpha, alpha);
    }
    LOG_DEBUG("img: '{}' loaded with {}x{}, channels:{}, min/max alpha: {}/{}",tex_path,pxwidth,pxheight,nchannels,min_alpha,max_alpha);
    Texture2D::init();
    Texture2D::bind();
    Texture2D::setMinifyMode(to_i32(GL_NEAREST));
    Texture2D::setMagnifyMode(to_i32(GL_NEAREST));
    Texture2D::setWrapMode(to_i32(GL_CLAMP_TO_BORDER));
    Texture2D::setBorderColor(border_color);

    glTexImage2D(GL_TEXTURE_2D, 0, gpu_pixel_fmt, pxwidth, pxheight, 0, cpu_pixel_fmt, GL_UNSIGNED_BYTE, (const void*)tex_pixels); 
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(tex_pixels);
    LOG_DEBUG("{} bound to texture from {}",idx,tex_path);
}
Texture2D::Texture2D(const char* tex_path, vec4 border_color, bool has_alpha){
    load(tex_path, border_color, has_alpha);
}
void Texture2D::load_empty(i32 w, i32 h) {
    glTexImage2D(GL_TEXTURE_2D,0, GL_RGB,w,h,0,GL_RGB,GL_UNSIGNED_BYTE,nullptr );
}
void Texture2D::bind() {
    glActiveTexture(GL_TEXTURE0 + idx);
    glBindTexture(GL_TEXTURE_2D, id);
}
void Texture2D::init() {
    glGenTextures(1, &id);
}
void Texture2D::destroy() {
    glDeleteTextures(1, &id);
}
void Texture2D::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}
void Texture2D::setBorderColor(vec4 color = { 0, 0, 0, 0 }) {
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &color.x);
}
void Texture2D::setMinifyMode(i32 mode) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mode);
}
void Texture2D::setMagnifyMode(i32 mode) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mode);
}
void Texture2D::setWrapMode(i32 mode) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mode);
}
