#include "ChunkMesher.hpp"
#include "Block.hpp"

extern const std::vector<std::vector<Vertex>> defaultCubeFaces;
// TODO: Fix to take in World param or smth and check adj chunks
std::vector<Block>  getNeighbours(const Chunk& chunk, const Block& block){
    return {(Block){.id=BlockType::AIR}};
}
static constexpr u64 CUBE_FACE_COUNT = 6;
std::vector<Mesh> ChunkMesher::mesh(const Chunk& chunk,const TextureAtlas& atlas){
    // BUG: Ignores neighbouring chunks for face culling 
    // *This is the naive mesher, which just creates a 6 faced mesh for every block. Slow ash
    
    std::vector<Mesh> res;
    for (const auto& block: chunk.data){
        std::vector<Vertex> vertices;
        std::vector<Block> neighbours = getNeighbours(chunk,block);
        for (u64 faceID = 0; const auto& neighbour: neighbours){
            if (neighbour.isAir() || !neighbour.isOpaque()) {
                const auto& uv_tex_coords = atlas.remapUVs(0, faceID, defaultCubeFaces[faceID]);
                for (u64 i = 0; Vertex vtx : defaultCubeFaces[faceID]) {
                    vtx.txCoords = uv_tex_coords[i++];
                    vertices.push_back(vtx);
                }
            }
        }
        Mesh block_mesh;
        block_mesh.setup(vertices);
        res.push_back(block_mesh);

    }
    return res;
}
const std::vector<std::vector<Vertex>> defaultCubeFaces = {
    // FRONT (+X)
    std::vector<Vertex>{
        Vertex{.pos={-0.5f, -0.5f, -0.5f,  }, .txCoords = {0.0f, 1.0f}},
        Vertex{.pos={0.5f, -0.5f, -0.5f,  }, .txCoords = {1.0f, 1.0f}},
        Vertex{.pos={0.5f,  0.5f, -0.5f,  }, .txCoords = {1.0f, 0.0f}},
        Vertex{.pos={0.5f,  0.5f, -0.5f,  }, .txCoords = {1.0f, 0.0f}},
        Vertex{.pos={-0.5f,  0.5f, -0.5f,  }, .txCoords = {0.0f, 0.0f}},
        Vertex{.pos={-0.5f, -0.5f, -0.5f,  }, .txCoords = {0.0f, 1.0f}},
    },

    std::vector<Vertex>{
        Vertex{.pos={    -0.5f, -0.5f,  0.5f,  }, .txCoords = {0.0f, 1.0f}},//{0.0f, 1.0f}
        Vertex{.pos={     0.5f, -0.5f,  0.5f,  }, .txCoords = {1.0f, 1.0f}},//{1.0f, 1.0f}
        Vertex{.pos={     0.5f,  0.5f,  0.5f,  }, .txCoords = {1.0f, 0.0f}},//{1.0f, 0.0f}
        Vertex{.pos={     0.5f,  0.5f,  0.5f,  }, .txCoords = {1.0f, 0.0f}},//{1.0f, 0.0f}
        Vertex{.pos={    -0.5f,  0.5f,  0.5f,  }, .txCoords = {0.0f, 0.0f}},//{0.0f, 0.0f}
        Vertex{.pos={    -0.5f, -0.5f,  0.5f,  }, .txCoords = {0.0f, 1.0f}},//{0.0f, 1.0f}

    },
    
    std::vector<Vertex>{
        Vertex{.pos={    -0.5f,  0.5f,  0.5f,  }, .txCoords = {1.0f, 0.0f}},
        Vertex{.pos={    -0.5f,  0.5f, -0.5f,  }, .txCoords = {0.0f, 0.0f}},
        Vertex{.pos={    -0.5f, -0.5f, -0.5f,  }, .txCoords = {0.0f, 1.0f}},
        Vertex{.pos={    -0.5f, -0.5f, -0.5f,  }, .txCoords = {0.0f, 1.0f}},
        Vertex{.pos={    -0.5f, -0.5f,  0.5f,  }, .txCoords = {1.0f, 1.0f}},
        Vertex{.pos={    -0.5f,  0.5f,  0.5f,  }, .txCoords = {1.0f, 0.0f}},
    },
    
    std::vector<Vertex>{
        Vertex{.pos={     0.5f,  0.5f,  0.5f,  }, .txCoords = {0.0f, 0.0f}},
        Vertex{.pos={     0.5f,  0.5f, -0.5f,  }, .txCoords = {1.0f, 0.0f}},
        Vertex{.pos={     0.5f, -0.5f, -0.5f,  }, .txCoords = {1.0f, 1.0f}},
        Vertex{.pos={     0.5f, -0.5f, -0.5f,  }, .txCoords = {1.0f, 1.0f}},
        Vertex{.pos={     0.5f, -0.5f,  0.5f,  }, .txCoords = {0.0f, 1.0f}},
        Vertex{.pos={     0.5f,  0.5f,  0.5f,  }, .txCoords = {0.0f, 0.0f}},
    },
    
    std::vector<Vertex>{
        Vertex{.pos={    -0.5f, -0.5f, -0.5f,  }, .txCoords = {1.0f, 0.0f}},
        Vertex{.pos={     0.5f, -0.5f, -0.5f,  }, .txCoords = {0.0f, 0.0f}},
        Vertex{.pos={     0.5f, -0.5f,  0.5f,  }, .txCoords = {0.0f, 1.0f}},
        Vertex{.pos={     0.5f, -0.5f,  0.5f,  }, .txCoords = {0.0f, 1.0f}},
        Vertex{.pos={    -0.5f, -0.5f,  0.5f,  }, .txCoords = {1.0f, 1.0f}},
        Vertex{.pos={    -0.5f, -0.5f, -0.5f,  }, .txCoords = {1.0f, 0.0f}},
    }, 
    std::vector<Vertex>{
        Vertex{.pos={    -0.5f,  0.5f, -0.5f,  }, .txCoords = {0.0f, 1.0f}},
        Vertex{.pos={     0.5f,  0.5f, -0.5f,  }, .txCoords = {1.0f, 1.0f}},
        Vertex{.pos={     0.5f,  0.5f,  0.5f,  }, .txCoords = {1.0f, 0.0f}},
        Vertex{.pos={     0.5f,  0.5f,  0.5f,  }, .txCoords = {1.0f, 0.0f}},
        Vertex{.pos={    -0.5f,  0.5f,  0.5f,  }, .txCoords = {0.0f, 0.0f}},
        Vertex{.pos={    -0.5f,  0.5f, -0.5f,  }, .txCoords = {0.0f, 1.0f}},
    },
};
