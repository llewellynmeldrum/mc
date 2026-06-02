
#include "ChunkConcurrency.hpp"
#include "DebugFormatSpecializations.hpp"
#include "Renderer.hpp"

#include "Profiler.hpp"
#include "glbinding/gl/functions.h"
#include "glbindingWrapper.hpp"
#include "glmWrapper.hpp"
#include "World.hpp"
#include <algorithm>

using namespace gl;
using namespace glm;


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
    LOG_EXPR(size);
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


void Renderer::updateViewport(int x, int y, int w, int h) {
    glViewport(x, y, w, h);
}
Renderer::Renderer() {
    prog.setup("shaders/vs.glsl", "shaders/fs.glsl");
    prog.use();
    prog.setUniform("texture1", (int)0);
    prog.stop();
    meshers.launch();

    dbg_rend.setup();
    glEnable(GL_DEPTH_TEST);  // perform depth testing, i.e refuse draw calls which would cause a
                              // vertex further away to overwrite a closer one
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
}

void Renderer::clear(const vec4 clear_color) {
    debug.reset_per_frame();
    // hollow purple reference
    glPolygonMode(GL_FRONT_AND_BACK, debug.wireframe ? GL_LINE : GL_FILL);
    glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::draw_debugChunks(Camera& cam, World& world){
    dbg_rend.update(cam,world);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    dbg_rend.draw(cam);
    glPolygonMode(GL_FRONT_AND_BACK, debug.wireframe ? GL_LINE : GL_FILL);
}
// TODO: we need to resort chunk meshes/ update cam pos to get transparency 100% correct

void Renderer::draw_to(Camera& cam, RenderTargetView target){
    target.use();
    clear(clear_color);

    debug.reset_per_frame();
    auto& view = cam.getViewMatrix();
    const auto& proj = cam.getProjectionMatrix();
    prog.use();
    atlas.texture.bind();
    prog.setUniform("view", view);
    prog.setUniform("proj", proj);
//    prog.setUniform("overlayOpacity", debug.blockOverlayOpacity);
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE); // Enable depth mask for opaque
        glDisable(GL_BLEND);
        drawOpaque(cam);

        //glDepthMask(GL_FALSE); // Disable depth mask for transparent
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
        drawTransparent(cam);

        glDepthMask(GL_TRUE); // Enable depth mask for opaque
    prog.stop();

    target.stop();

}
void Renderer::drawTransparent(Camera& cam){
    ScopeTimer draw_timer{ "Renderer::draw", "draw call" };
    // sort the vector by nearest chunks (std::less)
    auto sqdist = [](const auto& a, const auto& b) -> i32{
        auto sq = [](i32 v){ return v*v;};
        return sq(b.x-a.x) + sq(b.y-a.y) + sq(b.z-a.z);
    };
    auto source = toWorldChunkCoord(cam.pos);
    //NOTE: SORTS FURTHEST TO NEAREST (transparent)
    std::ranges::sort(transparentChunkMeshes, [sqdist, source](const auto& lhs, const auto& rhs){
        // sort ascending, i.e mesh closest first.
        // Will have to flip when i do transperancy
        return sqdist(source,lhs.chunkCoord) > sqdist(source,rhs.chunkCoord);
    });

    for (const auto& mesh : transparentChunkMeshes) {
        const auto chunkFloatWorldPos = WorldFloatPos{toWorldOrigin(mesh.chunkCoord).raw()};
//           const auto chunkDist = glm::distance(chunkWorldPos,cam.pos);
//            std::println("{}",chunkDist);
        auto model = mat4(1.0f);
        model = glm::translate(model, chunkFloatWorldPos.raw());
        prog.setUniform("model", model);
        mesh.draw();
        debug.vertex_count += mesh.vertex_count;
        debug.mesh_count++;
        debug.draw_calls++;
    }
//        timer_log_avg_us("Renderer::draw", debug.draw_calls);
}

void Renderer::drawOpaque(Camera& cam){
    ScopeTimer draw_timer{ "Renderer::draw", "draw call" };
    // sort the vector by nearest chunks (std::less)
    auto sqdist = [](const auto& a, const auto& b) -> i32{
        auto sq = [](i32 v){ return v*v;};
        return sq(b.x-a.x) + sq(b.y-a.y) + sq(b.z-a.z);
    };
    auto source = toWorldChunkCoord(cam.pos);
    //NOTE: SORTS CLOSEST TO FURTHEST (opaque)
    std::ranges::sort(opaqueChunkMeshes, [sqdist, source](const auto& lhs, const auto& rhs){
        return sqdist(source,lhs.chunkCoord) < sqdist(source,rhs.chunkCoord);
    });
    for (const auto& mesh : opaqueChunkMeshes) {
        const auto chunkFloatWorldPos = WorldFloatPos{toWorldOrigin(mesh.chunkCoord).raw()};
//           const auto chunkDist = glm::distance(chunkWorldPos,cam.pos);
//            std::println("{}",chunkDist);
        auto model = mat4(1.0f);
        model = glm::translate(model, chunkFloatWorldPos.raw());
        prog.setUniform("model", model);
        mesh.draw();
        debug.vertex_count += mesh.offset_count;
        debug.mesh_count++;
        debug.draw_calls++;
    }
//        timer_log_avg_us("Renderer::draw", debug.draw_calls);
}
