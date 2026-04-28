#include "DebugFormatSpecializations.hpp"
#include "ChunkMesher.hpp"
#include "Block.hpp"

#include "CommonUtils.hpp"
#include "Logger.hpp"
#include "glmWrapper.hpp"
#include "Context.hpp"
#include "Profiler.hpp"
extern const std::vector<std::vector<Vertex>> defaultCubeFaces;

static constexpr const std::vector<Vertex>&
getDefaultFaceOffsets(Direction dir) {
    return defaultCubeFaces[static_cast<i8>(dir)];
}

Mesh
ChunkMesher::mesh(const World* world_ptr, const Chunk& chunk, const ivec3 chunk_offset,
                  const TextureAtlas& atlas) {
    std::vector<Vertex> vertices;
    const auto&         world = *world_ptr;
    auto                neighbour_chunks = world.chunks.getNeighbours(chunk_offset);

    // clang-format off
    const auto*  front_neighbour_chunk  = neighbour_chunks[0];
    const auto*  back_neighbour_chunk =   neighbour_chunks[1];
    const auto*  left_neighbour_chunk =   neighbour_chunks[2];
    const auto*  right_neighbour_chunk  = neighbour_chunks[3];
    const auto*  down_neighbour_chunk =   neighbour_chunks[4];
    const auto*  up_neighbour_chunk =     neighbour_chunks[5];

    for (i32 x = 0; x < CHUNK_XWIDTH; x++) {
        for (i32 y = 0; y < CHUNK_HEIGHT; y++) {
            for (i32 z = 0; z < CHUNK_ZWIDTH; z++) {
                const vec3 chunk_pos = { x, y, z };
                auto       block = chunk[x, y, z];
                if (block.isAir()) {
                    // no meshing for air blocks
                    continue;
                }
                // FORWARD,   // -z
                // BACKWARD,  // +z
                // LEFT,      // -x
                // RIGHT,     // +x
                // DOWN,      // -y
                // UP,        // +y
                auto get_neighbour_block = [&chunk, x, y, z](bool neighbourOutsideChunk, vec3 pos, vec3 neigh_pos,
                                                            const Chunk* neighbour_chunk) -> Block {
                    if (!neighbourOutsideChunk) [[likely]] {  // blk is inside chunk
                        return chunk[pos.x, pos.y, pos.z];                // captured
                    } else [[unlikely]]{
                        if (!neighbour_chunk) [[unlikely]] {
                            return Block::Empty();
                        } else [[likely]]{
                            return (*neighbour_chunk)[neigh_pos.x, neigh_pos.y, neigh_pos.z];
                        }
                    }
                };
                // clang-format on
                using NeighbourList = std::array<std::pair<Block, Direction>, NUM_NEIGHBOURS>;

                // clang-format off
                NeighbourList block_neighbours = {
                    std::make_pair(get_neighbour_block(z == 0,
                                            {x, y, z-1},
                                            {x, y, CHUNK_ZWIDTH-1},
                                            front_neighbour_chunk), Direction::FORWARD),

                    std::make_pair(get_neighbour_block(z == CHUNK_ZWIDTH-1,
                                            {x, y, z+1},
                                            {x, y, 0},
                                            back_neighbour_chunk), Direction::BACKWARD),

                    std::make_pair(get_neighbour_block(x == 0,
                                            {x-1,y,z},
                                            {CHUNK_XWIDTH-1, y, z},
                                            left_neighbour_chunk), Direction::LEFT),
                    std::make_pair(get_neighbour_block(x == CHUNK_XWIDTH-1,
                                            {x+1,y,z},
                                            {0, y, z},
                                            right_neighbour_chunk), Direction::RIGHT),

                    std::make_pair(get_neighbour_block(y == 0,
                                            {x,y-1,z},
                                            {x, CHUNK_HEIGHT-1, z},
                                            down_neighbour_chunk), Direction::DOWN),
                    std::make_pair(get_neighbour_block(y == CHUNK_HEIGHT-1,
                                            {x,y+1,z},
                                            {x, 0, z},
                                            up_neighbour_chunk), Direction::UP),
                };
                // clang-format on

                for (const auto [neighbour, face_dir] : block_neighbours) {
                    bool faceFacesAir = neighbour == Block::Empty() || neighbour.isAir();
                    // || !neighbour.isOpaque()) {
                    if (faceFacesAir) {
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
    Mesh chunk_mesh;
    chunk_mesh.setup(vertices);
    return chunk_mesh;
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
