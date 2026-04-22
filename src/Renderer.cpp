#include "Renderer.hpp"


#include "glmWrapper.hpp"
#include "glbindingWrapper.hpp"

using namespace gl;
using namespace glm;
void Renderer::updateViewport(int x, int y, int w, int h){
    glViewport(x,y,w,h);
}
void Renderer::setupRenderer(){
    atlas.setupTextureAtlas();
    prog.setupShaderProgram("shaders/vs.glsl", "shaders/fs.glsl");
    prog.use();
    prog.setUniform("texture1", (int)0);
    prog.stop();
}

void Renderer::draw(const mat4& view, const mat4& proj){
    glPolygonMode(GL_FRONT_AND_BACK, debug.wireframe ? GL_LINE : GL_FILL); 

    glClearColor(0.25, 0.5, 0.85, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // TODO:
    // is this how you construct the default model transform?
    // validate all the matrices are going in properly, also check what the 
    // gl error could mean.
    // GL_INVALID_OPERATION
    // wiki page
    //	glUniformMatrix4fv(0x600000b35050, 0x600000b342a0, 0x600000b34fb0, 0x600000b34f00)
    mat4 model = mat4(1.0f);
    vec3 chunk_origin_world = vec3(0.0f);
    model = translate(model, chunk_origin_world);
    prog.use();
        atlas.texture.bind();
        // will be changed with World abstraction
        for (const auto& mesh: chunkMeshes){
            mesh.draw(prog,model,view,proj);
        }
    prog.stop();

}

