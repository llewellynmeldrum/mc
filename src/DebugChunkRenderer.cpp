#include "ChunkInvariants.hpp"
#include "ChunkEntry.hpp"
#include "Renderer.hpp"
#include "Engine.hpp"
#include "DebugChunkRenderer.hpp"
#include "World.hpp"
#include <optional>
#include <print>
using namespace gl;


static constexpr glm::vec3 NNN{0,0,0};
static constexpr glm::vec3 NPN{0, CHUNK_EXTENT,0};
static constexpr glm::vec3 NNP{0,0, CHUNK_EXTENT};
static constexpr glm::vec3 PNN{ CHUNK_EXTENT,0,0};
static constexpr glm::vec3 PPN{ CHUNK_EXTENT, CHUNK_EXTENT,0};
static constexpr glm::vec3 PNP{ CHUNK_EXTENT,0, CHUNK_EXTENT};
static constexpr glm::vec3 NPP{0, CHUNK_EXTENT, CHUNK_EXTENT};
static constexpr glm::vec3 PPP{ CHUNK_EXTENT, CHUNK_EXTENT, CHUNK_EXTENT};
constexpr std::array DebugChunkVertices ={

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
constexpr std::array DebugChunkIndices{
     3U, 0U, 1U, 1U, 2U, 3U,
     7U, 4U, 5U, 5U, 6U, 7U,
    11U, 8U, 9U, 9U,10U,11U,
    15U,12U,13U,13U,14U,15U,
    19U,16U,17U,17U,18U,19U,
    23U,20U,21U,21U,22U,23U,
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

void DebugChunkRenderer::update(Camera& cam, Engine* sim){
    // solid geometry
    updateInstances(cam,sim);
    // lines
    const bool show_chunk_boundaries = sim->ui.dbg_view.showChunkBoundaries;
    const bool highlight_neighbours = sim->ui.dbg_view.showNeighbours;
    chunk_outlines.clear();
    if (show_chunk_boundaries){
        sim->world.chunkMap.entries.for_each([&](WorldChunkCoord key, ChunkEntry& entry){
            if (sim->is_chunk_in_frustum(sim->playerCam.getFrustum(), key)){
                if (DebugChunkRenderer::HIDE_CLEAN_CHUNKS && entry.is_mesh_clean()){
                    return;
                }
                if (DebugChunkRenderer::HIDE_AIR_CHUNKS && entry.block_data.isAllAir()){
                    return;
                }
                const auto& state = entry.state;
                auto color = sim->ui.dbg_view.showGenState
                        ? GenDebugOutlineColor(state.gen)
                        : MeshDebugOutlineColor(state.mesh);

                chunk_outlines.append_range(entry.bounding_box.getLines(color));
            }
        });
    }
    if (highlight_neighbours){
        auto cam_chunk = toWorldChunkCoord(sim->playerCam.pos);
        for (const auto& [dir, offset]: eachDirOffset){
            const auto neighbour = WorldChunkCoord{cam_chunk.raw()+offset};
            sim->world.chunkMap.entries.if_contains(
                neighbour,
                [&](ChunkEntry& entry){
                    const auto& state = entry.state;
                    auto color = sim->ui.dbg_view.showGenState
                            ? GenDebugOutlineColor(state.gen)
                            : MeshDebugOutlineColor(state.mesh);
                    chunk_outlines.append_range(entry.bounding_box.getLines(color,true));
                });
        }
        sim->world.chunkMap.entries.if_contains(
            cam_chunk,
            [&](ChunkEntry& entry){
                    const auto& state = entry.state;
                    auto color = sim->ui.dbg_view.showGenState
                            ? GenDebugOutlineColor(state.gen)
                            : MeshDebugOutlineColor(state.mesh);
                chunk_outlines.append_range(entry.bounding_box.getLines(color,true));
            });
    }

    vao.bind();
    instance_vbo.load<DebugChunkInstance>(instances);
    vao.unbind();
}
void DebugChunkRenderer::updateInstances(Camera& cam,  Engine* sim){
    auto cam_chunk = toWorldChunkCoord(cam.pos);
    const auto& inRadius = sim->world.chunksStatesInRadius(cam_chunk,cam.DebugChunkRenderDistance);
    instances.clear();
    if (sim->ui.dbg_view.showNeighbours){
        for (const auto& [dir, offset]: eachDirOffset){
            const auto neighbour = WorldChunkCoord{cam_chunk.raw()+offset};
//            instances.emplace_back(toWorldBlockPos(neighbour,{0,0,0}).raw(), NeighbourDebugColor());
        }
    }
    bool showGenState = sim->ui.dbg_view.showGenState;
    for (const auto& [hasStateEntry, entryCoord]: inRadius){
        auto entryColor = DefaultDebugColor();
        if (hasStateEntry){
            const auto* entry = sim->world.chunkMap.entries.at(entryCoord);
            const auto& state = entry->state;
            if (HIDE_CLEAN_CHUNKS && entry->is_mesh_clean()){
                continue; // skip, else visual clutter is too bad
            }
            auto skip = sim->world.chunkMap.entries.if_contains(
                entryCoord,
                [](ChunkEntry& entry)->bool{
                    if (HIDE_AIR_CHUNKS && entry.block_data.isAllAir()){
                        return true;
                    }
                    return false;
                }
            );
            if (skip) continue;
            entryColor = showGenState ? GenDebugColor(state.gen) :
                                         MeshDebugColor(state.mesh);
        }
        instances.emplace_back(toWorldBlockPos(entryCoord,{0,0,0}).raw(), entryColor);
    }
}
