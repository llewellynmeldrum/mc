#include "App.hpp"
#include "Context.hpp"
#include "DEBUG.hpp"
#define _DEBUG

void Context::draw(){
    rend.draw(cam.getViewMatrix(), cam.getProjectionMatrix());
    win.swap();
}


void App::setup(){
    Chunk chunk;
    for (u64 x =0; x<CHUNK_XWIDTH; x++){
        for (u64 y =0; y<CHUNK_HEIGHT-1; y++){
            for (u64 z =0; z<CHUNK_ZWIDTH; z++){
                    chunk.placeBlock(BlockType::DIRT_BLOCK, x,y,z);
            }
        }
    }
    for (u64 z =0; z<CHUNK_ZWIDTH; z++){
        for (u64 x =0; x<CHUNK_XWIDTH; x++){
            chunk.placeBlock(BlockType::GRASS_BLOCK, x,CHUNK_HEIGHT-1,z);
        }
    }
    //chunk.placeBlock(BlockType::DIRT_BLOCK, 0,-2,0);
    //chunk.placeBlock(BlockType::DIRT_BLOCK, 0,-3,0);
    // for testing, this will later be in some sort of World abstraction
    ctx.rend.chunkMeshes = ctx.rend.mesher.mesh(chunk,ctx.rend.atlas);
}

void App::loop(){
    ctx.input.poll();
    ctx.handleInputs();
    ctx.draw();
    ctx.time.update();
}

bool App::shouldClose(){
    return ctx.win.shouldClose();
}


int main(int argc, char** argv) {
    App app;
    app.ctx.setupContext();
    app.setup();
    while(!app.shouldClose()){
        app.loop();
        app.frameCount++;
    }
    app.exit(EXIT_SUCCESS);
}

#include "GLFWWrapper.hpp"

void App::exit(i32 exit_code){
    glfwTerminate();
    std::exit(exit_code);
}



