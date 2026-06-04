#pragma once 
#include "DepthBuffer.hpp"
#include "FrameBuffer.hpp"
#include "Texture2D.hpp"
#include "Types.h"
struct RenderTargetView{
    glm::ivec2 pos;
    glm::ivec2 size;
    u32 framebuffer_id{0}; // 0 = screen
    bool isOverlay{false};
    void use();
    void stop();
private:
    void bind();
    void unbind();
};

struct ScreenTarget{
    glm::ivec2 pos;
    glm::ivec2 size;
    inline RenderTargetView view(){
        return {
            .pos = pos,
            .size = size,
            .framebuffer_id=0,
            .isOverlay=true,
        };
    }
};
struct TextureTarget{
    TextureTarget(glm::vec2 _pos, glm::vec2 _size);
    TextureTarget();
    ~TextureTarget()=default;

    Texture2D texture;
    FrameBuffer framebuffer;
    DepthBuffer depthbuffer;
    glm::vec2 pos;
    glm::vec2 size;

    inline RenderTargetView view(){
        return {
            .pos = pos,
            .size = size,
            .framebuffer_id=framebuffer.id,
        };
    }
};
