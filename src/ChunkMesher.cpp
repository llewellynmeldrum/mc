#include "Chunk.hpp"
#include "CoordIteration.hpp"
#include "Breakpoints.hpp"
#include "Direction.hpp"
#include "FormatSpecs.hpp"
#include "ChunkMesher.hpp"
#include "Block.hpp"
#include "ChunkHelpers.hpp"
#include "ChunkInvariants.hpp"

// include/Chunk
#include "CommonUtils.hpp"
#include "Logger.hpp"
#include "glmWrapper.hpp"
#include "Engine.hpp"
#include "Profiler.hpp"
#include "LM.hpp"
#include <ranges>
#include <tuple>
#include "Concurrency.hpp"

#include "Logger.hpp"
#include "Assertion.hpp"


#include "tracy/Tracy.hpp"
using std::views::enumerate;


extern const std::array<std::array<Vertex, VTX_PER_FACE>, FACES_PER_CUBE> cube_vertices;
extern const std::array<u32, INDICES_PER_FACE>                   cube_indices;

static const auto& get_default_face_vertices(Direction dir) {
    return cube_vertices[static_cast<i8>(dir)];
}


std::array<Block, DirectionCount> get_surrounding_blocks(const MeshJob& job, ChunkBlockPos chunk_local_block);



// TODO: Implement transperant meshing.
// Correct transperancy requires that the transparent meshes are 
// rendered furthest to closest.
// But not only does this have to happen in chunk order, it also has to happen in block order.
//
// TODO: 
// The draw order must be:
//  1. Enable depth mask, disable blending, enable backface culling
//  2. Draw opaque meshes 
//  3. Disable depth mask, enable blending, disable backface culling
//  4. draw transperant meshes in sorted order (furthest first)
//  5. 

template<typename MeshDataType>
auto meshTypePredicate(ChunkStore& chunk){
    if constexpr(same_as_nocvref<OpaqueMeshData,MeshDataType>){
        return [&chunk](auto xyz){
            auto [x,y,z]=xyz;
            return chunk.at(x,y,z).isOpaque();
        };
    }else{
        return[&chunk](auto xyz){
            auto [x,y,z]=xyz;
            return chunk.at(x,y,z).isTransparent();
        };
    }
}
template<typename MeshDataType>
MeshDataType meshChunk(const MeshJob& job){
    MeshDataType mesh_data{};
    auto& out_indices = mesh_data.indices;
    auto& out_vertices = mesh_data.vertices;

    // WARNING: These are pretty huge reserve()s. no idea if they will be worth it 
    out_vertices.reserve(MAX_VERTICES_PER_CHUNK);
    out_indices.reserve(MAX_INDICES_PER_CHUNK);

    const WorldBlockPos world_pos = toWorldOrigin(job.chunkCoord);
    auto chunk = job.blocks;
    const auto atlas= job.atlas;
    const auto& surrounding_chunks = job.surroundingChunks;

    u32 vtx_count = 0;
    for (const auto& [x, y, z] : EachBlockInChunk(meshTypePredicate<MeshDataType>(chunk))) {
        const ChunkBlockPos chunk_local_block = { x, y, z };
        // TODO: UNUSED ATM
        const glm::vec3 overlayRBG = {0,0,0}; 

        Block block = chunk[x, y, z];
        if (block.isAir()) {
            continue;
        }

        const f32 block_opacity = block.getOpacity();
        auto adjacent_blocks = get_surrounding_blocks(job,chunk_local_block);

        for (const auto& [face_idx, adjacentBlock] : enumerate(adjacent_blocks)) {
            const auto faceDir = static_cast<Direction>(face_idx);
            if constexpr (same_as_nocvref<OpaqueMeshData,MeshDataType>){
                // skip opaque blocks in the opaque meshing function
                if (adjacentBlock.isOpaque()) {
                    continue;
                }
            }else {
                // for transparent blocks, skip meshing faces which touch identical neighbours
                if (adjacentBlock.type==block.type){
                    continue;
                }
            }


            const auto& vtx_data = get_default_face_vertices(faceDir);
            const auto& uv_tex_coords = atlas->remapUVs(block.texture_id(), faceDir, vtx_data);

            for (std::size_t i = 0; i < INDICES_PER_FACE; i++) {
                i32 mapped_index = vtx_count + cube_indices[i];
                out_indices.push_back(mapped_index);
            }

            for (const auto& [vertex_idx, vtx] : enumerate(vtx_data)) {
                decltype(Vertex::pos) chunk_offsetted_vtx_pos =
                    static_cast<decltype(Vertex::pos)>(vtx.pos)
                    +
                    static_cast<decltype(Vertex::pos)>(chunk_local_block.raw());
                glm::vec2 texture_coords = uv_tex_coords[vertex_idx];
                out_vertices.emplace_back(
                    chunk_offsetted_vtx_pos,
                    texture_coords, 
                    overlayRBG, 
                    static_cast<i32>(faceDir),
                    block_opacity
                );
                vtx_count++;
            }
        }
    }
    return {out_vertices,out_indices};
}
std::array<Block, DirectionCount> 
get_surrounding_blocks(const MeshJob& job, ChunkBlockPos chunk_local_block) {
    std::array<Block, DirectionCount> res{};
    constexpr glm::ivec3 lo = glm::ivec3(0);
    constexpr glm::ivec3 hi = Chunk::Extents;
    for (const auto& dir : each_horizontal_direction){
        const i32   dir_idx = static_cast<i32>(dir);
        const auto neigh_offset = Direction_offset[dir_idx];
        ChunkBlockPos neighbour_block_pos = chunk_local_block + BlockOffset{neigh_offset};
        const bool neighbour_inside_my_chunk = LM::isVecInBounds(neighbour_block_pos, lo, hi);
        
        if (neighbour_inside_my_chunk) [[likely]]{
            AT(res,dir_idx) = AT(job.blocks,neighbour_block_pos);
        } else [[unlikely]] {
            // The adjacent chunk, if it exists, has our neighbour. else its air
            bool other_chunk_exists = job.surroundingChunks.at(dir_idx).has_value();
            if (other_chunk_exists){
                const auto& other_chunk = job.surroundingChunks[dir_idx].value();
                neighbour_block_pos = LM::euclid_mod(neighbour_block_pos, Chunk::Extents);
                AT(res,dir_idx) = AT(other_chunk,neighbour_block_pos);
            }else{
                // treat all blocks of a missing chunk as air
                AT(res,dir_idx) = Block(BlockType::AIR);
            }
        }

    }
    for (const auto& dir : each_vertical_direction){
        // if neighbour is within 0 - 256, then return that block. It exists in our chunk. It cannot be in another 
        const i32   dir_idx = static_cast<i32>(dir);
        const auto neigh_offset = Direction_offset[dir_idx];
        ChunkBlockPos neighbour_block_pos = chunk_local_block + BlockOffset{neigh_offset};
        const bool neighbour_outside_world = neighbour_block_pos.y>=WORLD_YMAX || neighbour_block_pos.y < WORLD_YMIN;
        if (neighbour_outside_world) {
            AT(res,dir_idx) = Block(BlockType::AIR);
            continue;
        }else{
            // out of bounds       V
            AT(res,dir_idx) = AT(job.blocks,neighbour_block_pos);
        }
    }

//    for (const auto& [face_dir, neighbour_block_offset] : eachDirOffset) {
//        const i32   face_dir_idx = static_cast<i32>(face_dir);
//        ChunkBlockPos neighbour_block_pos = chunk_local_block + BlockOffset{neighbour_block_offset};
//        const bool neighbour_inside_my_chunk = LM::isVecInBounds(neighbour_block_pos, lo, hi);
//        const bool neighbour_outside_world = neighbour_block_pos.y>WORLD_YMAX || neighbour_block_pos.y < WORLD_YMIN;
//        if (neighbour_outside_world) {
//            AT(res,face_dir_idx) = Block(BlockType::AIR);
//            continue;
//        }
//
//        // TODO: investigate whether attributes are helping or not here. 
//        // Likely branch should run ~95% of the time by my math
//        if (neighbour_inside_my_chunk) [[likely]]{
//            const auto& p = neighbour_block_pos;
//            AT(res,face_dir_idx) = AT(job.blocks,neighbour_block_pos);
//        } else [[unlikely]] {
//            // The adjacent chunk, if it exists, has our neighbour. else its air
//            bool other_chunk_exists = job.surroundingChunks.at(face_dir_idx).has_value();
//            if (other_chunk_exists){
//                const auto& other_chunk = job.surroundingChunks[face_dir_idx].value();
//                neighbour_block_pos = LM::euclid_mod(neighbour_block_pos, Chunk::Extents);
//                AT(res,face_dir_idx) = AT(other_chunk,neighbour_block_pos);
//            }else{
//                // treat all blocks of a missing chunk as air
//                AT(res,face_dir_idx) = Block(BlockType::AIR);
//            }
//        }
//
//    }
    return res;
}

void ChunkMesher::meshChunks
(std::stop_token stopToken, Queue<MeshJob>& input_queue, Queue<MeshResult>& output_queue){
    tracy::SetThreadName("chunk mesher");
    while (!stopToken.stop_requested()){
        
        auto job = input_queue.wait_dequeue();


        MeshResult res{job.meshRevisionID, job.chunkCoord};
        { 
            ZoneScopedN("transparent_mesh_chunks")
            res.transparent = meshChunk<TransparentMeshData>(job); // mandatory copy elision i think
        }
        { 
            ZoneScopedN("opaque_mesh_chunks")
            res.opaque = meshChunk<OpaqueMeshData>(job); // mandatory copy elision i think
        }

        { 
            ZoneScopedN("mesh_await_output")
            output_queue.wait_emplace(res);
        }
    }

}
// clang-format off
// 
static constexpr glm::vec3 NNN{0.0,0.0,0.0};
static constexpr glm::vec3 NPN{0.0, 1.0,0.0};
static constexpr glm::vec3 NNP{0.0,0.0, 1.0};
static constexpr glm::vec3 PNN{ 1.0,0.0,0.0};
static constexpr glm::vec3 PPN{ 1.0, 1.0,0.0};
static constexpr glm::vec3 PNP{ 1.0,0.0, 1.0};
static constexpr glm::vec3 NPP{0.0, 1.0, 1.0};
static constexpr glm::vec3 PPP{ 1.0, 1.0, 1.0};
using Face = std::array<Vertex,VTX_PER_FACE>;
constexpr std::array<std::array<Vertex,VTX_PER_FACE>, FACES_PER_CUBE> cube_vertices = {
    // Direction::forward
    Face{
        Vertex{PNN,glm::vec2(0,1),glm::vec3(1.0f),0},
        Vertex{NNN,glm::vec2(1,1),glm::vec3(1.0f),0},
        Vertex{NPN,glm::vec2(1,0),glm::vec3(1.0f),0},
        Vertex{PPN,glm::vec2(0,0),glm::vec3(1.0f),0},
    },
    // Direction::Backward
    Face{
        Vertex{NNP,glm::vec2(0,1),glm::vec3(1.0f),1},
        Vertex{PNP,glm::vec2(1,1),glm::vec3(1.0f),1},
        Vertex{PPP,glm::vec2(1,0),glm::vec3(1.0f),1},
        Vertex{NPP,glm::vec2(0,0),glm::vec3(1.0f),1},
    },
    // Direction:: Left
    Face{
        Vertex{NNN,glm::vec2(0,1),glm::vec3(1.0f),2},
        Vertex{NNP,glm::vec2(1,1),glm::vec3(1.0f),2},
        Vertex{NPP,glm::vec2(1,0),glm::vec3(1.0f),2},
        Vertex{NPN,glm::vec2(0,0),glm::vec3(1.0f),2},
    },

    // Direction::Right
    Face{
        Vertex{PNP,glm::vec2(0,1),glm::vec3(1.0f),3},
        Vertex{PNN,glm::vec2(1,1),glm::vec3(1.0f),3},
        Vertex{PPN,glm::vec2(1,0),glm::vec3(1.0f),3},
        Vertex{PPP,glm::vec2(0,0),glm::vec3(1.0f),3},
    },
    // Direction::Down
    Face{
        Vertex{NNN,glm::vec2(0,0),glm::vec3(1.0f),4},
        Vertex{PNN,glm::vec2(1,0),glm::vec3(1.0f),4},
        Vertex{PNP,glm::vec2(1,1),glm::vec3(1.0f),4},
        Vertex{NNP,glm::vec2(0,1),glm::vec3(1.0f),4},
    },
    // Direction::Up
    Face{
        Vertex{NPP,glm::vec2(0,0),glm::vec3(1.0f),5},
        Vertex{PPP,glm::vec2(1,0),glm::vec3(1.0f),5},
        Vertex{PPN,glm::vec2(1,1),glm::vec3(1.0f),5},
        Vertex{NPN,glm::vec2(0,1),glm::vec3(1.0f),5},
    },
};
constexpr std::array<u32,INDICES_PER_FACE> cube_indices{
    { 3, 0, 1, 1, 2, 3,},
};
static_assert(cube_vertices.size()==6);
