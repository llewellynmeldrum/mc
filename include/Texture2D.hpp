#pragma once
#include <string>
#include "Types.h"
#include "glmWrapper.hpp"
#include "stb_image.hpp"
extern i64 texture_count;
struct Texture2D {
    Texture2D(){init();}
    ~Texture2D(){destroy();}

    // Loads a specified image into a texture object.
    void load(const char* tex_path, i32 image_fmt, glm::vec4 border_color);
    Texture2D(const char* tex_dir, i32 image_fmt, glm::vec4 border_color);
    void bind();
    void unbind();
    void load_empty(i32 w, i32 h) ;
    void setBorderColor(glm::vec4 color);
    void setMinifyMode(i32 mode);
    void setMagnifyMode(i32 mode);
    void setWrapMode(i32 mode);

    u32         id;

    u32         idx = 0;
    i32         pxwidth, pxheight, nchannels;

  private:
    void init();
    void destroy();

};
