#include "ChunkMesher.hpp"
#include "Block.hpp"

#include "glmWrapper.hpp"
#include "CommonUtils.hpp"
extern const std::vector<std::vector<Vertex>> defaultCubeFaces;
// TODO: Fix to take in World param or smth and check adj chunks
std::vector<std::pair<Block,Direction>>  getNeighbours(const Chunk& chunk, const vec3& p){
    return{
        {chunk.getBlock(p.x,  p.y+1,p.z  ),Direction::UP},
        {chunk.getBlock(p.x,  p.y-1,p.z  ),Direction::DOWN},
        {chunk.getBlock(p.x-1,p.y,  p.z  ),Direction::LEFT},
        {chunk.getBlock(p.x+1,p.y,  p.z  ),Direction::RIGHT},
        {chunk.getBlock(p.x,  p.y,  p.z+1),Direction::BACKWARD},
        {chunk.getBlock(p.x,  p.y,  p.z-1),Direction::FORWARD},
    };
}

static constexpr const std::vector<Vertex>& getDefaultFaceOffsets(Direction dir){
    return defaultCubeFaces[static_cast<u8>(dir)];
}

Mesh ChunkMesher::mesh(const Chunk& chunk,const TextureAtlas& atlas){
    // BUG: Ignores neighbouring chunks for face culling 
    // *This is the naive mesher, which just creates a 6 faced mesh for every block. Slow ash

    // TODO: make it so that vertices are uploaded in chunk local position. -x,-z = 0,0
    std::vector<Vertex> vertices;
    for (u32 x = 0; x < CHUNK_XWIDTH; x++){
    for (u32 y = 0; y < CHUNK_HEIGHT; y++){
    for (u32 z = 0; z < CHUNK_ZWIDTH; z++){
        auto block = chunk[x,y,z];
        if (block.isAir()){
            continue;
        }
        auto neighbours = getNeighbours(chunk, vec3(x,y,z)); 
        for (const auto& [neighbour,face_dir]: neighbours){
            if (true|| neighbour.isAir() || !neighbour.isOpaque()) {
                auto faceOffsets = getDefaultFaceOffsets(face_dir);
                const auto& uv_tex_coords = atlas.remapUVs(block.texture_id(), face_dir, faceOffsets);
                for (u64 i = 0; Vertex vtx : faceOffsets) {
                    vtx.txCoords = uv_tex_coords[i++];
                    vtx.pos += vec3(x,y,z);
                    vertices.push_back(vtx);
                }
            }
        }
    }
    }
    }
    Mesh chunk_mesh;
    chunk_mesh.setup(vertices);
    return chunk_mesh; // change to single mesh return
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
