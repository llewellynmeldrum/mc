#include "ChunkConstants.hpp"
#include "ChunkEntry.hpp"
#include "Renderer.hpp"
#include "Simulation.hpp"
#include "DebugChunkRenderer.hpp"
#include "World.hpp"
#include <optional>
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

void DebugChunkRenderer::update(Camera& cam, Simulation* sim){
    updateInstances(cam,sim);

    vao.bind();
    instance_vbo.load<DebugChunkInstance>(instances);
    vao.unbind();
}
void DebugChunkRenderer::updateInstances(Camera& cam,  Simulation* sim){
    const auto& inRadius = sim->world.chunksStatesInRadius(toWorldChunkCoord(cam.pos),cam.DebugChunkRenderDistance);
    instances.clear();
    bool showGenState = sim->ui.dbg_view.showGenState;
    for (const auto& [hasStateEntry, entryCoord]: inRadius){
        auto entryColor = showGenState ? GenDebugColor(std::nullopt) :
                                         MeshDebugColor(ChunkMeshState::NoMesh);
        if (hasStateEntry){
            const auto* state = sim->world.chunkMap.get_state(entryCoord);
            const auto entry = sim->world.chunkMap.try_get_entry(entryCoord);
            if (HIDE_CLEAN_CHUNKS && state->isCleanMeshed()){
                continue; // skip, else visual clutter is too bad
            }
            if (entry.has_value()){
                if (HIDE_AIR_CHUNKS && (*entry)->block_data.isAllAir()){
                    continue; // skip, else visual clutter is too bad
                }
            }
            entryColor = showGenState ? GenDebugColor(state->gen) :
                                         MeshDebugColor(state->mesh);
        }
        instances.emplace_back(toWorldBlockPos(entryCoord,{0,0,0}).raw(), entryColor);
    }
}
