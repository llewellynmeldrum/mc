#include "ChunkConstants.hpp"
#include "ChunkEntry.hpp"
#include "ChunkViewHelpers.hpp"
#include "Renderer.hpp"
#include "Engine.hpp"
#include "DebugChunkRenderer.hpp"
#include "World.hpp"
#include <optional>
#include <print>
using namespace gl;


static constexpr glm::vec3 NNN{0,             0,            0};
static constexpr glm::vec3 NPN{0,             ChunkInfo::HEIGHT, 0};
static constexpr glm::vec3 NNP{0,             0,            ChunkInfo::ZWIDTH};
static constexpr glm::vec3 PNN{ChunkInfo::XWIDTH,  0,            0};
static constexpr glm::vec3 PPN{ChunkInfo::XWIDTH,  ChunkInfo::HEIGHT, 0};
static constexpr glm::vec3 PNP{ChunkInfo::XWIDTH,  0,            ChunkInfo::ZWIDTH};
static constexpr glm::vec3 NPP{0,             ChunkInfo::HEIGHT, ChunkInfo::ZWIDTH};
static constexpr glm::vec3 PPP{ChunkInfo::XWIDTH,  ChunkInfo::HEIGHT, ChunkInfo::ZWIDTH};
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

void DebugChunkMesher::setup(){
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

GLenum DebugChunkMesher::PrimitiveType(){
    return GL_TRIANGLES;
}
void DebugChunkMesher::draw(Camera& cam){
    prog.use();
    prog.setUniform("view",cam.getViewMatrix());
    prog.setUniform("proj",cam.getProjectionMatrix());
    vao.bind();
    vao.drawElementsInstanced(MAX_INDICES_PER_CUBE, instances.size(), PrimitiveType());
    vao.unbind();
    prog.stop();
}

void DebugChunkMesher::update(Camera& cam, Engine* sim){
    // solid geometry
    updateInstances(cam,sim);
    // lines
    chunk_outlines.clear();
    if (DebugOption::outline_all_boundaries){
        sim->world.chunkMap.entries.for_each([&](WorldChunkCoord key, ChunkEntry& entry){
            if (sim->is_chunk_in_frustum(sim->player_cam.getCullFrustum(), key)){
                if (DebugOption::HIDE_CLEAN_CHUNKS && entry.is_mesh_clean()){
                    return;
                }
                if (DebugOption::HIDE_AIR_CHUNKS && is_all_air(entry.block_data.view())){
                    return;
                }
                const auto& state = entry.state;
                auto color = DebugOption::gen_state_mode
                        ? GenDebugOutlineColor(state.gen)
                        : MeshDebugOutlineColor(state.mesh);

                chunk_outlines.append_range(entry.bounding_box.getLines(color));
            }
        });
    }
    if (DebugOption::outline_neighbour_boundaries){
        auto cam_chunk = toWorldChunkCoord(sim->player_cam.pos);
        for (const auto& [dir, offset]: eachDirOffset2D){
            const auto neighbour = WorldChunkCoord{cam_chunk.raw()+offset};
            sim->world.chunkMap.entries.if_contains(
                neighbour,
                [&](ChunkEntry& entry){
                    const auto& state = entry.state;
                    auto color = DebugOption::gen_state_mode
                            ? GenDebugOutlineColor(state.gen)
                            : MeshDebugOutlineColor(state.mesh);
                    chunk_outlines.append_range(entry.bounding_box.getLines(color));
                });
        }
        sim->world.chunkMap.entries.if_contains(
            cam_chunk,
            [&](ChunkEntry& entry){
                    const auto& state = entry.state;
                    auto color = DebugOption::gen_state_mode
                            ? GenDebugOutlineColor(state.gen)
                            : MeshDebugOutlineColor(state.mesh);
                chunk_outlines.append_range(entry.bounding_box.getLines(color));
            });
    }

    vao.bind();
    instance_vbo.load<DebugChunkInstance>(instances);
    vao.unbind();
}
void DebugChunkMesher::updateInstances(Camera& cam,  Engine* sim){
    auto cam_chunk = toWorldChunkCoord(cam.pos);
    const auto& inRadius = sim->world.chunksStatesInRadius(cam_chunk,cam.DebugChunkRenderDistance);
    instances.clear();
    if (DebugOption::fill_neighbour_boundaries){
        for (const auto& [dir, offset]: eachDirOffset2D){
            const auto neighbour = WorldChunkCoord{cam_chunk.raw()+offset};
            sim->world.chunkMap.entries.if_contains(
                neighbour,
                [&](ChunkEntry& entry){
                        const auto& state = entry.state;
                        auto color = DebugOption::gen_state_mode
                                ? GenDebugColor(state.gen)
                                : MeshDebugColor(state.mesh);
                    instances.emplace_back(toWorldBlockPos(neighbour,BlockOffset{0,0,0}).raw(), color);
            });
        }
    }
    bool showGenState = DebugOption::gen_state_mode;
    if(DebugOption::fill_all_boundaries)
    for (const auto& [hasStateEntry, entryCoord]: inRadius){
        auto entryColor = DefaultDebugColor();
        if (hasStateEntry){
            const auto* entry = sim->world.chunkMap.entries.at(entryCoord);
            const auto& state = entry->state;
            if (DebugOption::HIDE_CLEAN_CHUNKS && entry->is_mesh_clean()){
                continue; // skip, else visual clutter is too bad
            }
            auto skip = sim->world.chunkMap.entries.if_contains(
                entryCoord,
                [](ChunkEntry& entry)->bool{
                    if (DebugOption::HIDE_AIR_CHUNKS && is_all_air(entry.block_data.view())){
                        return true;
                    }
                    return false;
                }
            );
            if (skip) continue;
            entryColor = showGenState ? GenDebugColor(state.gen) :
                                         MeshDebugColor(state.mesh);
        }
        instances.emplace_back(toWorldBlockPos(entryCoord,BlockOffset{0,0,0}).raw(), entryColor);
    }
}
