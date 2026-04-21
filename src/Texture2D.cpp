#define GL_SILENCE_DEPRECATION
#include "glbinding/gl/enum.h"
#include "glbinding/glbinding.h"
#include "glbinding/gl/gl.h"
using namespace gl;

#include "Texture2D.hpp"
#include "Logger.hpp"

Texture2D::Texture2D(const char* tex_dir, GLenum image_fmt=GL_RGB, vec4 border_color) {
    mat4::length();
    // stbi_load returns row major 2d pixels array.
    u8* tex_pixels = stbi_load(tex_dir, &pxwidth, &pxheight, &nchannels, 0);
    if (!tex_pixels) {
        LOG_ERROR("Failed to load texture file '{}'.", tex_dir);
        return;
    }
    Texture2D::init();
    Texture2D::bind();
    Texture2D::setMinifyMode(GL_LINEAR_MIPMAP_NEAREST);
    Texture2D::setMagnifyMode(GL_NEAREST);
    Texture2D::setWrapMode(GL_CLAMP_TO_BORDER);
    Texture2D::setBorderColor(border_color);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, pxwidth, pxheight, 0, image_fmt, GL_UNSIGNED_BYTE,
                 (const void*)tex_pixels);
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
void Texture2D::setMinifyMode(GLenum mode) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mode);
}
void Texture2D::setMagnifyMode(GLenum mode) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mode);
}
void Texture2D::setWrapMode(GLenum mode) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mode);
}
