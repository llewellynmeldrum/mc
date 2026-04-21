#include <cfloat>
#include <cassert>
#include <format>
#include <limits>
#include <concepts>
#include <print>
#include <string>
#include <unordered_map>

#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>


#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <utility>

#include "../external/stb_image.hpp"

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


TextureAtlas atlas{"resources/textures/test.png"};
using namespace glm;
using namespace gl;

#define _DEBUG
#define _LIBCPP_DEBUG
#define _LIBCPP_DEBUG_USE_EXCEPTIONS

u64 texture_count = 0;
u64 program_epoch_ns;
Context ctx = Context(Camera((vec3){0,0,6}));





// cube mesh struct

int main(int argc, char** argv) {
    init_window();
    init_opengl();

    Chunk chunk;
    chunk.placeBlock(BlockType::GRASS_BLOCK, 0,0,0);
    chunk.placeBlock(BlockType::DIRT_BLOCK, 0,-1,0);



    std::vector<Mesh> chunk_meshes;



    ShaderProgram prog("shaders/vs.glsl","shaders/fs.glsl");
    prog.use();
    prog.setUniform("texture1", (int)0);
    prog.stop();
    mat4 model_matrix = mat4(1.0f);

    u64 frameCount = 0;
    ctx.time.init();
    while (!glfwWindowShouldClose(ctx.win.ptr)){
        //update();
        handleInputs(ctx);
        //draw();
        glPolygonMode(GL_FRONT_AND_BACK, ctx.wireframe ? GL_LINE : GL_FILL); 
        glClearColor(0.25, 0.5, 0.96, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        prog.use();
            atlas.texture.bind();
            for (const auto& mesh: chunk_meshes){
                mesh.draw(prog,model_matrix);
            }
        prog.stop();


        glfwSwapBuffers(ctx.win.ptr);
        glfwPollEvents();
        ctx.time.update();
        frameCount++;
    }
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
void Timer::init() {
    glfwSetTime(0.0);
}
void Timer::update() {
    f64 prev_elapsed_s = elapsed_s;
    elapsed_s = glfwGetTime();
    dt = elapsed_s - prev_elapsed_s;
}
Context::Context(Camera _cam) : cam(std::move(_cam)) {
    init_keybinds();
}

