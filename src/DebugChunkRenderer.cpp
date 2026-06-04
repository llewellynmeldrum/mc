#include "ChunkConstants.hpp"
#include "ChunkEntry.hpp"
#include "Renderer.hpp"
#include "DebugChunkRenderer.hpp"
#include "World.hpp"
using namespace gl;


static constexpr glm::vec3 NNN{0,0,0};
static constexpr glm::vec3 NPN{0, CHUNK_EXTENT,0};
static constexpr glm::vec3 NNP{0,0, CHUNK_EXTENT};
static constexpr glm::vec3 PNN{ CHUNK_EXTENT,0,0};
static constexpr glm::vec3 PPN{ CHUNK_EXTENT, CHUNK_EXTENT,0};
static constexpr glm::vec3 PNP{ CHUNK_EXTENT,0, CHUNK_EXTENT};
static constexpr glm::vec3 NPP{0, CHUNK_EXTENT, CHUNK_EXTENT};
static constexpr glm::vec3 PPP{ CHUNK_EXTENT, CHUNK_EXTENT, CHUNK_EXTENT};
constexpr std::array<DebugChunkVertex, MAX_VERTICES_PER_CUBE> DebugChunkVertices ={

        DebugChunkVertex{PNN},
        DebugChunkVertex{NNN},
        DebugChunkVertex{NPN},
        DebugChunkVertex{PPN},
                            
        DebugChunkVertex{NNP},
        DebugChunkVertex{PNP},
        DebugChunkVertex{PPP},
        DebugChunkVertex{NPP},
                            
        DebugChunkVertex{NNN},
        DebugChunkVertex{NNP},
        DebugChunkVertex{NPP},
        DebugChunkVertex{NPN},
                            
        DebugChunkVertex{PNP},
        DebugChunkVertex{PNN},
        DebugChunkVertex{PPN},
        DebugChunkVertex{PPP},
                            
        DebugChunkVertex{NNN},
        DebugChunkVertex{PNN},
        DebugChunkVertex{PNP},
        DebugChunkVertex{NNP},
                            
        DebugChunkVertex{NPP},
        DebugChunkVertex{PPP},
        DebugChunkVertex{PPN},
        DebugChunkVertex{NPN},
};
constexpr std::array<u32,MAX_INDICES_PER_CUBE> DebugChunkIndices{
     3, 0, 1, 1, 2, 3,
     7, 4, 5, 5, 6, 7,
    11, 8, 9, 9,10,11,
    15,12,13,13,14,15,
    19,16,17,17,18,19,
    23,20,21,21,22,23,
};

void DebugChunkRenderer::setup(){
    vao.make();
    cube_vbo.make();
    cube_ebo.make();
    instance_vbo.make();

    prog.setup("shaders/debug_chunks_vs.glsl", "shaders/debug_chunks_fs.glsl");

    vao.bind();

    cube_vbo.bind();
    cube_vbo.load<DebugChunkVertex>(DebugChunkVertices);
    vao.apply_layout(DebugChunkVertex::layout());

    instance_vbo.bind();
    vao.apply_layout(DebugChunkInstance::layout());

    cube_ebo.bind();
    cube_ebo.load(DebugChunkIndices);
    vao.unbind();
}

GLenum DebugChunkRenderer::PrimitiveType(){
    return GL_TRIANGLES;
}
void DebugChunkRenderer::draw(Camera& cam){
    prog.use();
    prog.setUniform("view",cam.getViewMatrix());
    prog.setUniform("proj",cam.getProjectionMatrix());
    vao.bind();
    vao.drawElementsInstanced(MAX_INDICES_PER_CUBE, instances.size(), PrimitiveType());
    vao.unbind();
    prog.stop();
}

void DebugChunkRenderer::update(Camera& cam, World& world){
    updateInstances(cam,world);

    vao.bind();
    instance_vbo.load<DebugChunkInstance>(instances);
    vao.unbind();
}
void DebugChunkRenderer::updateInstances(Camera& cam, World& world){
    const auto& inRadius = world.chunksInRadius(toWorldChunkCoord(cam.pos),cam.DebugChunkRenderDistance);
    instances.clear();
    for (const auto& [hasEntry, entryCoord]: inRadius){
        auto entryColor = ChunkEntryStatus::UnGeneratedColor;
        if (hasEntry){
            const auto& entry = world.chunkMap.get_entry(entryCoord);
            entryColor = entry->status.DebugColor();
            if (entry->status.isCleanMeshed()){
                continue; // skip, else visual clutter is too bad
            }
        }
        instances.emplace_back(toWorldBlockPos(entryCoord,{0,0,0}).raw(), entryColor);
    }
}
