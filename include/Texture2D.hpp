#pragma once
#include "Types.h"
#include "glmWrapper.hpp"
#include "stb_image.hpp"
#include <string>
extern i64 texture_count;
struct Texture2D{
    Texture2D() = default;
    ~Texture2D() = default;



    Texture2D(const char* tex_dir, i32 image_fmt, vec4 border_color);
    void setup();
    void bind();

    u32 id;
    u32 idx=0;
    i32 pxwidth, pxheight, nchannels;
    std::string texturePath{};
    i32 imageFormat;
    vec4 borderColor;
  private:
    void        init();
    void        unbind();

    static void        setBorderColor(vec4 color);
    static void        setMinifyMode(i32 mode);
    static void        setMagnifyMode(i32 mode);
    static void        setWrapMode(i32 mode);
};
