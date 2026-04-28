#include "glbindingWrapper.hpp"
using namespace gl;

#include "Texture2D.hpp"
#include "Logger.hpp"
i64 texture_count = 0;

Texture2D::Texture2D(const char* tex_dir, i32 image_fmt = to_i32(GL_RGB),
                     vec4 border_color = { 1, 0, 1, 1 })
    : texturePath(tex_dir), imageFormat(image_fmt), borderColor(border_color) {
}

void Texture2D::setup() {
    u8* tex_pixels = stbi_load(texturePath.c_str(), &pxwidth, &pxheight, &nchannels, 0);
    if (!tex_pixels) {
        LOG_ERROR("Failed to load texture file '{}'.", texturePath);
        LOG_EXIT(EXIT_FAILURE);
        return;
    }
    LOG_EXPR(pxwidth);
    LOG_EXPR(pxheight);
    LOG_EXPR(nchannels);
    LOG_EXPR(tex_pixels);
    Texture2D::init();
    Texture2D::bind();
    Texture2D::setMinifyMode(to_i32(GL_NEAREST));
    Texture2D::setMagnifyMode(to_i32(GL_NEAREST));
    Texture2D::setWrapMode(to_i32(GL_CLAMP_TO_BORDER));
    Texture2D::setBorderColor(borderColor);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, pxwidth, pxheight, 0, to_glenum(imageFormat),
                 GL_UNSIGNED_BYTE, (const void*)tex_pixels);
    glGenerateMipmap(GL_TEXTURE_2D);
    Texture2D::unbind();

    stbi_image_free(tex_pixels);
    idx = texture_count++;
    LOG_EXPR(idx);
}
void Texture2D::bind() {
    glActiveTexture(GL_TEXTURE0 + idx);
    glBindTexture(GL_TEXTURE_2D, id);
}
void Texture2D::init() {
    glGenTextures(1, &id);
}
void Texture2D::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}
void Texture2D::setBorderColor(vec4 color) {
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, data(color));
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
