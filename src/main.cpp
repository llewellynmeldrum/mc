
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "GLFWWrapper.hpp"
#include "glmWrapper.hpp"
#include "Logger.hpp"
#include "Types.h"
#include "UnixHelpers.hpp"
#include "glHelpers.hpp"
#include "UnixHelpers.hpp"

#include "Texture2D.hpp"
#include "Vertex.hpp"
#include "TextureAtlas.hpp"
#include "Camera.hpp"
#include "Context.hpp"
#include "Shaders.hpp"
#include "CommonUtils.hpp"
#include "Mesh.hpp"

#include "App.hpp"
#define _DEBUG
using namespace glm;
using namespace gl;

TextureAtlas atlas{"resources/textures/test.png"};

u64 texture_count = 0;
Context ctx = Context(Camera((vec3){0,0,6}));


void Context::swapBuffers(){
    glfwPollEvents();
    glfwSwapBuffers(win.ptr);
}

void Context::draw(){
    rend.draw();
    rend.swap();
    rend.present();
}



App app;
void App::setup(){
    ctx.setup();
    Chunk chunk;
    chunk.placeBlock(BlockType::GRASS_BLOCK, 0,0,0);
    chunk.placeBlock(BlockType::DIRT_BLOCK, 0,-1,0);
    std::vector<Mesh> chunk_meshes;

    ShaderProgram prog("shaders/vs.glsl","shaders/fs.glsl");
    prog.use();
    prog.setUniform("texture1", (int)0);
    prog.stop();
    mat4 model_matrix = mat4(1.0f);

    ctx.time.setup();

}

void App::loop(){
    ctx.input.poll();
    ctx.handleInputs();
    ctx.draw();
    ctx.time.update();
}


int main(int argc, char** argv) {
    app.setup();
    while(!app.shouldClose()){
        app.loop();
        app.frameCount++;
    }
    app.exit(EXIT_SUCCESS);
}


void App::exit(i32 exit_code){
    glfwTerminate();
    exit(exit_code);
}
