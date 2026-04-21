#pragma once
#include "Types.h"

#include "glmWrapper.hpp"
#include "stb_image.hpp"
#include "glbinding/gl/enum.h"
extern u64 texture_count;
using namespace gl;
struct Texture2D{
    u32 id;
    u32 idx=0;
    i32 pxwidth, pxheight, nchannels;
    Texture2D() = delete;
    Texture2D(const char* tex_dir, GLenum image_fmt, vec4 border_color = { 1, 0, 1, 1 });
    void bind();

  private:
    void        init();
    void        unbind();

    static void        setBorderColor(vec4 color);
    static void        setMinifyMode(GLenum mode);
    static void        setMagnifyMode(GLenum mode);
    static void        setWrapMode(GLenum mode);
};
