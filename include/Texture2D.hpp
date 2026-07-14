#pragma once
#include <string>
#include "Types.h"
#include "glmWrapper.hpp"
#include "stb_image.hpp"
extern i64 texture_count;
struct Texture2D {
    Texture2D()= default;
    ~Texture2D()= default;

    // Loads a specified image into a texture object.
    void load(const char* tex_path, glm::vec4 border_color, bool has_alpha);
    Texture2D(const char* tex_path, glm::vec4 border_color, bool has_alpha);
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

    void init();
    void destroy();

};
