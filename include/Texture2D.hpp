#pragma once
#include "Types.h"
#include "glmWrapper.hpp"
#include "stb_image.hpp"
#include <string>
extern i64 texture_count;
struct Texture2D {
    Texture2D() = default;
    ~Texture2D() = default;

    Texture2D(const char* tex_dir, i32 image_fmt, glm::vec4 border_color);
    void setup();
    void bind();

    u32         id;
    u32         idx = 0;
    i32         pxwidth, pxheight, nchannels;
    std::string texturePath{};
    i32         imageFormat;
    glm::vec4        borderColor;

  private:
    void init();
    void unbind();

    void setBorderColor(glm::vec4 color);
    void setMinifyMode(i32 mode);
    void setMagnifyMode(i32 mode);
    void setWrapMode(i32 mode);
};
