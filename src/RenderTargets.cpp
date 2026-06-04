
#include "FrameBuffer.hpp"
#include "DepthBuffer.hpp"
#include "DepthBuffer.hpp"
#include "Logger.hpp"
#include "RenderTargets.hpp"
#include "glbindingWrapper.hpp"
#include "glbinding/gl/enum.h"
using namespace gl;
void FrameBuffer::bind(){
    glBindFramebuffer(GL_FRAMEBUFFER,id);
}

void FrameBuffer::unbind(){
    glBindFramebuffer(GL_FRAMEBUFFER,0);
}
FrameBuffer::FrameBuffer(){
    glGenFramebuffers(1, &id);
}
FrameBuffer::~FrameBuffer(){
    glDeleteFramebuffers(1,&id);
}
void FrameBuffer::attach_depth_buffer(DepthBuffer& depth_buffer){
    depth_buffer.bind();
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_buffer.id);
    depth_buffer.unbind();
}
void FrameBuffer::attach_texture2d(Texture2D& texture2d, i32 idx){
    texture2d.bind();
    constexpr i32 mipmap_lvl = 0;
    glFramebufferTexture(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0+idx,texture2d.id, mipmap_lvl);
    texture2d.unbind();

}

void DepthBuffer::bind(){
    glBindRenderbuffer(GL_RENDERBUFFER,id);
}

void DepthBuffer::unbind(){
    glBindRenderbuffer(GL_RENDERBUFFER,0);
}
DepthBuffer::DepthBuffer(glm::vec2 size){
    glGenRenderbuffers(1, &id);
    this->bind();
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, size.x,size.y);
    this->unbind();
}
DepthBuffer::~DepthBuffer(){
    glDeleteRenderbuffers(1,&id);
}


static void checkStatus() {
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    switch (status) {
        case GL_FRAMEBUFFER_COMPLETE:
            return;

        case GL_FRAMEBUFFER_UNDEFINED:
            LOG_ERROR("GL_FRAMEBUFFER_UNDEFINED");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            LOG_ERROR("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            LOG_ERROR("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
            LOG_ERROR("GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
            LOG_ERROR("GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER");
            break;
        case GL_FRAMEBUFFER_UNSUPPORTED:
            LOG_ERROR("GL_FRAMEBUFFER_UNSUPPORTED");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
            LOG_ERROR("GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE");
            break;
        default:
            LOG_ERROR("Unknown framebuffer error");
            break;
    }

    LOG_EXIT(1);
}

// for textures
TextureTarget::TextureTarget(glm::vec2 _pos, glm::vec2 _size): depthbuffer(_size), pos(_pos), size(_size){
    framebuffer.bind();
    texture.bind();
    texture.load_empty(size.x,size.y);
//    LOG_EXPR(size);
    texture.setMagnifyMode(to_i32(GL_NEAREST));
    texture.setMinifyMode(to_i32(GL_NEAREST));
    framebuffer.attach_texture2d(texture);
    framebuffer.attach_depth_buffer(depthbuffer);
    std::array<GLenum,1> DrawBuffers = {GL_COLOR_ATTACHMENT0};    
    glDrawBuffers(1,DrawBuffers.data());
    framebuffer.bind();
    checkStatus();
}

void RenderTargetView::use(){
    this->bind();
    glViewport(pos.x, pos.y, size.x, size.y);
}

void RenderTargetView::stop(){
    this->unbind();
}

void RenderTargetView::bind(){
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_id);
}
void RenderTargetView::unbind(){
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
