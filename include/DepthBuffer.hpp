#pragma once 

#include "Types.h"
struct DepthBuffer{
    u32 id;
    DepthBuffer()=delete;

    DepthBuffer(glm::vec2 size);
    ~DepthBuffer();
    void bind();
    void unbind();

};
