#include "App.hpp"
#include "Block.hpp"
#include "Context.hpp"
#include "DEBUG.hpp"
#define _DEBUG

constexpr const u32 RENDER_DIST = 4;

// TODO: move to src/Context.cpp
void Context::drawScene(){
    if (cam.requestsMeshRegen){
//        LOG_DEBUG("REGENERATING CHUNK MESHES");
        auto camera_chunk_pos = Chunk::worldToChunkPos(cam.pos);
        std::vector<ChunkView> visibleChunks = world.getChunksInRadius(camera_chunk_pos,RENDER_DIST);
        int i =0;
        LOG_DEBUG("visible chunks: {}",visibleChunks.size());
        for (const auto& [chunk_pos, chunk]: visibleChunks){
            rend.visibleChunkMeshes.insert({
                chunk_pos,
                rend.mesher.mesh(*chunk,rend.atlas)
            });
        }
    }
    rend.draw(cam.getViewMatrix(), cam.getProjectionMatrix());
}



void App::setup(){
    {
    ctx.world.genChunk({0,-1});
    Chunk& chunk = ctx.world.getChunk({0,-1});
    chunk.setBlocks(BlockType::GRASS_BLOCK, {0,0,0}, {2,2,2});
    chunk.setBlocks(BlockType::GRASS_BLOCK, {5,0,0}, {6,1,1});
    chunk.setBlock(BlockType::GRASS_BLOCK,  {8,0,0});
    }

    {
    ctx.world.genChunk({0,+1});
    Chunk& chunk = ctx.world.getChunk({0,+1});
    chunk.setBlocks(BlockType::DIRT_BLOCK, {0,0,0}, {2,2,2});
    chunk.setBlocks(BlockType::DIRT_BLOCK, {5,0,0}, {6,1,1});
    chunk.setBlock(BlockType::DIRT_BLOCK,  {8,0,0});
    }
}

void App::loop(){
    ctx.time.update();
    ctx.input.poll();
    ctx.handleInputs();

    ctx.rend.clear({0.25, 0.5, 0.85, 1.0});
    ctx.drawScene();

    ctx.ui.drawDebugUI();
    ctx.ui.render();
    ctx.win.swapBuffers();
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
    ctx.ui.destroyDebugUI();
    glfwTerminate();
    std::exit(exit_code);
}



