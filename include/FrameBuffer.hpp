#pragma once 
#include "Types.h"
#include "cppslop.hpp"
FORWARD_DECL_STRUCT(Texture2D)
FORWARD_DECL_STRUCT(DepthBuffer)
struct FrameBuffer{
    u32 id;

    void attach_depth_buffer(DepthBuffer& depth_buffer);
    void attach_texture2d(Texture2D& texture2d,i32 idx=0);
    FrameBuffer();
    ~FrameBuffer();
    void bind();
    void unbind();

};
