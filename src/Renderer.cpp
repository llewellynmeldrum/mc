#include "Renderer.hpp"

#include "glbindingWrapper.hpp"

using namespace gl;
void Renderer::setup(){

}

void Renderer::draw(){
    glPolygonMode(GL_FRONT_AND_BACK, debug.wireframe ? GL_LINE : GL_FILL); 
    glClearColor(0.25, 0.5, 0.96, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    prog.use();
        atlas.texture.bind();
        for (const auto& mesh: chunk_meshes){
            mesh.draw(prog,model_matrix);
        }
    prog.stop();

}

void Renderer::swap(){

}

void Renderer::present(){

}
