#include "ChunkConstants.hpp"
#include "ChunkEntry.hpp"
#include "ChunkViewHelpers.hpp"
#include "CoordIteration.hpp"
#include "DebugOptions.hpp"
#include "Renderer.hpp"
#include "Engine.hpp"
#include "DebugChunkRenderer.hpp"
#include "VertexLayoutHelpers.hpp"
#include "World.hpp"
#include <optional>
#include <print>
#include <span>
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

    prog.load_vtx_and_frag("shaders/debug_chunks_vs.glsl", "shaders/debug_chunks_fs.glsl");

    vao.bind();

    cube_vbo.bind();
    cube_vbo.load_vertices(DebugChunkVertices);
    apply_vertex_layout<DebugChunkVertex>();

    instance_vbo.bind();
    apply_vertex_layout<DebugChunkInstance>();

    cube_ebo.bind();
    cube_ebo.load_indices(DebugChunkIndices);
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
    auto outline_color = [](ChunkEntry const & entry){
        switch (DebugOption::render_state_mode){
        case DebugOption::DebugRenderStateTarget::MESH:
            return PipelineStateOutlineColor(entry.mesh_pipeline_state());
        break;
        case DebugOption::DebugRenderStateTarget::GEN:
            return PipelineStateOutlineColor(entry.gen_pipeline_state());
        break;
        case DebugOption::DebugRenderStateTarget::LIGHTING:
            return PipelineStateOutlineColor(entry.lighting_pipeline_state());
        break;
        }
    };
    updateInstances(cam,sim);
    // lines
    chunk_outlines.clear();
    if (DebugOption::outline_all_boundaries){
        sim->world.chunkMap.entries.for_each([&](WorldChunkCoord key, ChunkEntry& entry){
            if (sim->is_chunk_in_frustum(sim->player_cam.getCullFrustum(), key)){
                if (DebugOption::HIDE_CLEAN_CHUNKS && entry.mesh.is_clean()){
                    return;
                }
                if (DebugOption::HIDE_AIR_CHUNKS && is_all_air(entry.block_data.read().view())){
                    return;
                }
                auto color = outline_color(entry);
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
                    auto color = outline_color(entry);
                    chunk_outlines.append_range(entry.bounding_box.getLines(color));
                });
        }
        sim->world.chunkMap.entries.if_contains(
            cam_chunk,
            [&](ChunkEntry& entry){
                auto color = outline_color(entry);
                chunk_outlines.append_range(entry.bounding_box.getLines(color));
            });
    }

    vao.bind();
    instance_vbo.load_vertices(instances);
    vao.unbind();
}
void DebugChunkMesher::updateInstances(Camera& cam,  Engine* sim){
    auto fill_color = [](ChunkEntry const & entry){
        switch (DebugOption::render_state_mode){
        case DebugOption::DebugRenderStateTarget::MESH:
            return PipelineStateColor(entry.mesh_pipeline_state());
        break;
        case DebugOption::DebugRenderStateTarget::GEN:
            return PipelineStateColor(entry.gen_pipeline_state());
        break;
        case DebugOption::DebugRenderStateTarget::LIGHTING:
            return PipelineStateColor(entry.lighting_pipeline_state());
        break;
        }
    };
    auto cam_chunk = toWorldChunkCoord(cam.pos);
    instances.clear();
    if (DebugOption::fill_neighbour_boundaries){
        for (const auto& [dir, offset]: eachDirOffset2D){
            const auto neighbour = WorldChunkCoord{cam_chunk.raw()+offset};
            sim->world.chunkMap.entries.if_contains(
                neighbour,
                [&](ChunkEntry& entry){
                    auto color = fill_color(entry);
                    instances.emplace_back(toWorldBlockPos(neighbour,BlockOffset{0,0,0}).raw(), color);
            });
        }
    }
    if(DebugOption::fill_all_boundaries){
        for_each_spiral(
            cam_chunk, 
            cam.DebugChunkRenderDistance,
            [&](i32 x, i32 y){
                auto coord = WorldChunkCoord{x,y};
                auto entryColor = DefaultDebugColor();
                auto* entry = sim->world.chunkMap.entries.try_get(coord);
                if (entry){
                    if (DebugOption::HIDE_CLEAN_CHUNKS && entry->mesh.is_clean()){
                        return true; // skip, else visual clutter is too bad
                    }else if (DebugOption::HIDE_AIR_CHUNKS && is_all_air(entry->block_data.read().view())){
                        return true;
                    }else{
                        entryColor = fill_color(*entry);
                    }

                }
                instances.emplace_back(toWorldBlockPos(coord,BlockOffset{0,0,0}).raw(), entryColor);
                return true;
            }
        );
    }
}
