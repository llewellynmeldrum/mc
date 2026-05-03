
#include "DebugFormatSpecializations.hpp"
#include "Renderer.hpp"

#include "Profiler.hpp"
#include "glbindingWrapper.hpp"
#include "glmWrapper.hpp"
#include "World.hpp"

using namespace gl;
using namespace glm;
void Renderer::updateViewport(int x, int y, int w, int h) {
    glViewport(x, y, w, h);
}
void Renderer::setupRenderer() {
    atlas.setupTextureAtlas();
    prog.setupShaderProgram("shaders/vs.glsl", "shaders/fs.glsl");
    prog.use();
    prog.setUniform("texture1", (int)0);
    prog.stop();
}

void Renderer::clear(const vec4 clear_color) {
    debug.reset_per_frame();
    // hollow purple reference
    glPolygonMode(GL_FRONT_AND_BACK, debug.wireframe ? GL_LINE : GL_FILL);
    glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::draw(const mat4& view, const mat4& proj) {
    prog.use();
    atlas.texture.bind();
    prog.setUniform("view", view);
    prog.setUniform("proj", proj);
    prog.setUniform("overlayOpacity", debug.blockOverlayOpacity);
    {
        ScopeTimer draw_timer{ "Renderer::draw", "draw call" };
        for (const auto& [chunk_pos, mesh] : visibleChunkMeshes) {
            const vec3 chunk_origin_world = World::chunkToWorldPos(chunk_pos);
            mat4       model = mat4(1.0f);
            model = translate(model, chunk_origin_world);
            prog.setUniform("model", model);
            debug.draw_calls += mesh.draw();
            debug.vertex_count += mesh.vertex_count;
            debug.mesh_count++;
        }
        timer_log_avg_us("Renderer::draw", debug.draw_calls);
    }
    prog.stop();
}
