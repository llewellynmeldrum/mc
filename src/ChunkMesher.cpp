#include "DebugFormatSpecializations.hpp"
#include "ChunkMesher.hpp"
#include "Block.hpp"

#include "CommonUtils.hpp"
#include "Logger.hpp"
#include "glmWrapper.hpp"
extern const std::vector<std::vector<Vertex>> defaultCubeFaces;
std::vector<std::pair<Block, Direction>>      getNeighbours(const Chunk& chunk, const vec3& p) {
    // TODO: Make this a world method that takes into account neighbouring chunks if block is on the
    // chunk edge
    return {
        { chunk.getBlock(p.x, p.y + 1, p.z), Direction::UP },
        { chunk.getBlock(p.x, p.y - 1, p.z), Direction::DOWN },
        { chunk.getBlock(p.x - 1, p.y, p.z), Direction::LEFT },
        { chunk.getBlock(p.x + 1, p.y, p.z), Direction::RIGHT },
        { chunk.getBlock(p.x, p.y, p.z + 1), Direction::BACKWARD },
        { chunk.getBlock(p.x, p.y, p.z - 1), Direction::FORWARD },
    };
}

static constexpr const std::vector<Vertex>& getDefaultFaceOffsets(Direction dir) {
    return defaultCubeFaces[static_cast<i8>(dir)];
}

Mesh ChunkMesher::mesh(const Chunk& chunk, const TextureAtlas& atlas) {
    f64                 t0 = get_current_us();
    f64                 nearch_time = 0;
    std::vector<Vertex> vertices;
    for (i32 x = 0; x < CHUNK_XWIDTH; x++) {
        for (i32 y = 0; y < CHUNK_HEIGHT; y++) {
            for (i32 z = 0; z < CHUNK_ZWIDTH; z++) {
                vec3 chunk_offset = { x, y, z };
                auto block = chunk[x, y, z];
                if (block.isAir()) {
                    continue;
                }
                f64  nt0 = get_current_us();
                auto neighbours = getNeighbours(chunk, vec3(x, y, z));
                nearch_time += (get_current_us() - nt0);

                for (const auto& [neighbour, face_dir] : neighbours) {
                    if (neighbour.isAir()) {  // || !neighbour.isOpaque()) {
                        const auto& faceOffsets = getDefaultFaceOffsets(face_dir);
                        const auto& uv_tex_coords =
                            atlas.remapUVs(block.texture_id(), face_dir, faceOffsets);
                        for (i64 i = 0; Vertex vtx : faceOffsets) {
                            vertices.emplace_back(vtx.pos + chunk_offset, uv_tex_coords[i++]);
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
