#include "DebugFormatSpecializations.hpp"
#include "ChunkMesher.hpp"
#include "Block.hpp"

#include "CommonUtils.hpp"
#include "Logger.hpp"
#include "glmWrapper.hpp"
#include "Context.hpp"
extern const std::vector<std::vector<Vertex>> defaultCubeFaces;

std::vector<std::pair<Block, Direction>>
World::getNeighbourBlocks(const vec3 world_pos) const {
    const vec3& p = world_pos;  // for readability
    return {
        { getBlock({ p.x, p.y + 1, p.z }), Direction::UP },
        { getBlock({ p.x, p.y - 1, p.z }), Direction::DOWN },
        { getBlock({ p.x - 1, p.y, p.z }), Direction::LEFT },
        { getBlock({ p.x + 1, p.y, p.z }), Direction::RIGHT },
        { getBlock({ p.x, p.y, p.z + 1 }), Direction::BACKWARD },
        { getBlock({ p.x, p.y, p.z - 1 }), Direction::FORWARD },
    };
}

static constexpr const std::vector<Vertex>&
getDefaultFaceOffsets(Direction dir) {
    return defaultCubeFaces[static_cast<i8>(dir)];
}

Mesh
ChunkMesher::mesh(const World* world_ptr, const Chunk& chunk, const TextureAtlas& atlas) {
    f64                 t0 = get_current_us();
    f64                 nearch_time = 0;
    std::vector<Vertex> vertices;
    const World&        world = *world_ptr;
    for (i32 x = 0; x < CHUNK_XWIDTH; x++) {
        for (i32 y = 0; y < CHUNK_HEIGHT; y++) {
            for (i32 z = 0; z < CHUNK_ZWIDTH; z++) {
                const vec3 chunk_pos = { x, y, z };
                const vec3 world_pos = World::chunkToWorldPos(chunk_pos);

                auto block = chunk[x, y, z];
                if (block.isAir()) {
                    continue;
                }
                f64  nt0 = get_current_us();
                auto neighbours = world.getNeighbourBlocks(world_pos);
                nearch_time += (get_current_us() - nt0);

                for (const auto& [neighbour, face_dir] : neighbours) {
                    bool faceIsVisible = neighbour == Block::Empty() || neighbour.isAir();
                    // || !neighbour.isOpaque()) {
                    if (faceIsVisible) {
                        const auto& faceOffsets = getDefaultFaceOffsets(face_dir);
                        const auto& uv_tex_coords =
                            atlas.remapUVs(block.texture_id(), face_dir, faceOffsets);
                        for (i64 i = 0; Vertex vtx : faceOffsets) {
                            vertices.emplace_back(vtx.pos + chunk_pos, uv_tex_coords[i++]);
                        }
                    }
                }
            }
        }
    }
    LOG_DEBUG("neighbour search took {}/{}us", nearch_time, get_current_us() - t0);
    Mesh chunk_mesh;
    chunk_mesh.setup(vertices);
    return chunk_mesh;  // change to single mesh return
}
const std::vector<std::vector<Vertex>> defaultCubeFaces = {
    // FRONT (-Z)
    std::vector<Vertex>{
        Vertex{.pos={-0.5f, -0.5f, -0.5f,  }, .txCoords = {0.0f, 1.0f}},
        Vertex{.pos={0.5f, -0.5f, -0.5f,  }, .txCoords = {1.0f, 1.0f}},
        Vertex{.pos={0.5f,  0.5f, -0.5f,  }, .txCoords = {1.0f, 0.0f}},
        Vertex{.pos={0.5f,  0.5f, -0.5f,  }, .txCoords = {1.0f, 0.0f}},
        Vertex{.pos={-0.5f,  0.5f, -0.5f,  }, .txCoords = {0.0f, 0.0f}},
        Vertex{.pos={-0.5f, -0.5f, -0.5f,  }, .txCoords = {0.0f, 1.0f}},
    },

    // BACK (+Z)
    std::vector<Vertex>{
        Vertex{.pos={    -0.5f, -0.5f,  0.5f,  }, .txCoords = {0.0f, 1.0f}},//{0.0f, 1.0f}
        Vertex{.pos={     0.5f, -0.5f,  0.5f,  }, .txCoords = {1.0f, 1.0f}},//{1.0f, 1.0f}
        Vertex{.pos={     0.5f,  0.5f,  0.5f,  }, .txCoords = {1.0f, 0.0f}},//{1.0f, 0.0f}
        Vertex{.pos={     0.5f,  0.5f,  0.5f,  }, .txCoords = {1.0f, 0.0f}},//{1.0f, 0.0f}
        Vertex{.pos={    -0.5f,  0.5f,  0.5f,  }, .txCoords = {0.0f, 0.0f}},//{0.0f, 0.0f}
        Vertex{.pos={    -0.5f, -0.5f,  0.5f,  }, .txCoords = {0.0f, 1.0f}},//{0.0f, 1.0f}

    },
    
    // LEFT (-X)
    std::vector<Vertex>{
        Vertex{.pos={    -0.5f,  0.5f,  0.5f,  }, .txCoords = {1.0f, 0.0f}},
        Vertex{.pos={    -0.5f,  0.5f, -0.5f,  }, .txCoords = {0.0f, 0.0f}},
        Vertex{.pos={    -0.5f, -0.5f, -0.5f,  }, .txCoords = {0.0f, 1.0f}},
        Vertex{.pos={    -0.5f, -0.5f, -0.5f,  }, .txCoords = {0.0f, 1.0f}},
        Vertex{.pos={    -0.5f, -0.5f,  0.5f,  }, .txCoords = {1.0f, 1.0f}},
        Vertex{.pos={    -0.5f,  0.5f,  0.5f,  }, .txCoords = {1.0f, 0.0f}},
    },
    
    // RIGHT (+X)
    std::vector<Vertex>{
        Vertex{.pos={     0.5f,  0.5f,  0.5f,  }, .txCoords = {0.0f, 0.0f}},
        Vertex{.pos={     0.5f,  0.5f, -0.5f,  }, .txCoords = {1.0f, 0.0f}},
        Vertex{.pos={     0.5f, -0.5f, -0.5f,  }, .txCoords = {1.0f, 1.0f}},
        Vertex{.pos={     0.5f, -0.5f, -0.5f,  }, .txCoords = {1.0f, 1.0f}},
        Vertex{.pos={     0.5f, -0.5f,  0.5f,  }, .txCoords = {0.0f, 1.0f}},
        Vertex{.pos={     0.5f,  0.5f,  0.5f,  }, .txCoords = {0.0f, 0.0f}},
    },
    
    // DOWN (-Y)
    std::vector<Vertex>{
        Vertex{.pos={    -0.5f, -0.5f, -0.5f,  }, .txCoords = {1.0f, 0.0f}},
        Vertex{.pos={     0.5f, -0.5f, -0.5f,  }, .txCoords = {0.0f, 0.0f}},
        Vertex{.pos={     0.5f, -0.5f,  0.5f,  }, .txCoords = {0.0f, 1.0f}},
        Vertex{.pos={     0.5f, -0.5f,  0.5f,  }, .txCoords = {0.0f, 1.0f}},
        Vertex{.pos={    -0.5f, -0.5f,  0.5f,  }, .txCoords = {1.0f, 1.0f}},
        Vertex{.pos={    -0.5f, -0.5f, -0.5f,  }, .txCoords = {1.0f, 0.0f}},
    }, 
    // UP (+Y)
    std::vector<Vertex>{
        Vertex{.pos={    -0.5f,  0.5f, -0.5f,  }, .txCoords = {0.0f, 1.0f}},
        Vertex{.pos={     0.5f,  0.5f, -0.5f,  }, .txCoords = {1.0f, 1.0f}},
        Vertex{.pos={     0.5f,  0.5f,  0.5f,  }, .txCoords = {1.0f, 0.0f}},
        Vertex{.pos={     0.5f,  0.5f,  0.5f,  }, .txCoords = {1.0f, 0.0f}},
        Vertex{.pos={    -0.5f,  0.5f,  0.5f,  }, .txCoords = {0.0f, 0.0f}},
        Vertex{.pos={    -0.5f,  0.5f, -0.5f,  }, .txCoords = {0.0f, 1.0f}},
    },
};
