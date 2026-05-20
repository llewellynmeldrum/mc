#include "Chunk.hpp"
#include "DEBUG.hpp"
#include "DebugFormatSpecializations.hpp"
#include "ChunkMesher.hpp"
#include "Block.hpp"
#include "ChunkHelpers.hpp"

// include/Chunk
#include "CommonUtils.hpp"
#include "Logger.hpp"
#include "glmWrapper.hpp"
#include "Context.hpp"
#include "Profiler.hpp"
#include "lmath.hpp"
#include <ranges>
#include <tuple>
#include "Concurrency.hpp"

#include "Logger.hpp"

// TODO: 
// Next time i work on chunk meshing:
// Create some sort of chunk debug view, which renders on top of meshes.
// This debug view will show a different color for :
// -> A currently meshing chunk (yellow)
// -> A dirty chunk             (red)
// -> A clean chunk             (green)
// Perhaps just an opaque cube that sits on top of the chunk.

using std::views::enumerate;


extern const std::array<std::array<Vertex, VERTICES_PER_FACE>, FACES_PER_BLOCK> defaultCubeFaces;
extern const std::array<u32, INDICES_PER_BLOCK>                   defaultCubeIndices;

static const auto& getDefaultFaceVertexData(Direction dir) {
    return defaultCubeFaces[static_cast<i8>(dir)];
}

template <glm::length_t L, typename T, glm::qualifier Q>
glm::vec<L, T, Q> euclid_mod(glm::vec<L, T, Q> a, glm::vec<L, T, Q> b) {
    auto r = a % b;
    auto mask = glm::lessThan(r, glm::vec<L, T, Q>(0));
    return r + glm::mix(glm::vec<L, T, Q>(0), glm::abs(b), mask);
}


std::array<Block, DirectionCount> 
getNeighbourBlocks(MeshJob& job, ivec3 chunkLocal) {
    std::array<Block, DirectionCount> res{};

    constexpr ivec3 lo = ivec3(0);
    constexpr ivec3 hi = Chunk::Extents;
    for (const auto& [faceDirection, neighbourOffset] : eachDirOffset) {
        const i32   dir_idx = static_cast<i32>(faceDirection);
        ivec3       neighbourBlockPos = chunkLocal + neighbourOffset;
        const auto neighbourChunk = job.surroundingChunks[dir_idx];

        const bool neighbourInBounds = lmath::isVecInBounds(neighbourBlockPos, lo, hi);

        if (neighbourInBounds) [[likely]] {
            const auto& p = neighbourBlockPos;
            res[dir_idx] = job.blocks[neighbourBlockPos];
        } else [[unlikely]] {
            neighbourBlockPos = euclid_mod(neighbourBlockPos, Chunk::Extents);
            assert(lmath::isVecInBounds(neighbourBlockPos, lo, hi));
            res[dir_idx] = neighbourChunk.at(neighbourBlockPos);
        }
    }
    static_assert(res.size() == DirectionCount);
    return res;
}



std::atomic<std::size_t> thread_id;
void ChunkMesher::meshChunks
(std::stop_token stopToken, Queue<MeshJob>& input_queue, Queue<MeshResult>& output_queue){
    const std::size_t id = thread_id.fetch_add(1);
    while (!stopToken.stop_requested()){
        
       auto job = input_queue.wait_dequeue();

        MeshResult res{job.head.chunkCoord};
       // WARNING: These are pretty huge reserve()s. no idea if they will be worth it 
        res.vertices.reserve(MAX_VERTICES_PER_CHUNK);
        res.indices.reserve(MAX_INDICES_PER_CHUNK);

        const ivec3 world_pos = job.head.chunkCoord;
        auto chunk = job.blocks;
        const auto atlas= job.atlas;
        const auto& surrounding_chunks = job.surroundingChunks;



        u32 vtx_count = 0;
        for (const auto& [x, y, z] : EachBlockInChunk()) {
            const vec3 chunkLocal = { x, y, z };
            // TODO: UNUSED ATM
            const vec3 overlayColor = {0,0,0}; 

            Block block = chunk[x, y, z];
            if (block.isAir()) {
                continue;
            }
            auto neighbour_blocks = getNeighbourBlocks(job,chunkLocal);

            for (const auto& [face_idx, adjacentBlock] : enumerate(neighbour_blocks)) {
                const auto faceDir = static_cast<Direction>(face_idx);
                if (adjacentBlock.isOpaque()) {
                    continue;
                }


                const auto& vtx_data = getDefaultFaceVertexData(faceDir);
                const auto& uv_tex_coords = atlas->remapUVs(block.texture_id(), faceDir, vtx_data);

                for (int i = 0; i < 6; i++) {
                    i32 mapped_index = vtx_count + defaultCubeIndices[i];
                    res.indices.push_back(mapped_index);
                }
                for (const auto& [vertex_idx, vtx] : enumerate(vtx_data)) {
                    vec3 chunk_offsetted_vtx_pos = vtx.pos + chunkLocal;
                    vec2 texture_coords = uv_tex_coords[vertex_idx];
                    res.vertices.emplace_back(
                        chunk_offsetted_vtx_pos,
                        texture_coords, 
                        overlayColor, 
                        static_cast<i32>(faceDir)
                    );
                    vtx_count++;
                }
            }
       }
       if (vtx_count==0) continue;
       output_queue.wait_emplace(res);
    }

}
// clang-format off
// 
static constexpr glm::vec3 NNN{-0.5,-0.5,-0.5};
static constexpr glm::vec3 NPN{-0.5, 0.5,-0.5};
static constexpr glm::vec3 NNP{-0.5,-0.5, 0.5};
static constexpr glm::vec3 PNN{ 0.5,-0.5,-0.5};
static constexpr glm::vec3 PPN{ 0.5, 0.5,-0.5};
static constexpr glm::vec3 PNP{ 0.5,-0.5, 0.5};
static constexpr glm::vec3 NPP{-0.5, 0.5, 0.5};
static constexpr glm::vec3 PPP{ 0.5, 0.5, 0.5};
constexpr std::array<std::array<Vertex,4>,6> defaultCubeFaces = {
    // Direction::forward
    std::array<Vertex,4>{
        Vertex{PNN,vec2(0,0),vec3(1.0f),0},
        Vertex{NNN,vec2(1,0),vec3(1.0f),0},
        Vertex{NPN,vec2(1,1),vec3(1.0f),0},
        Vertex{PPN,vec2(0,1),vec3(1.0f),0},
    },
    // Direction::backward
    std::array<Vertex,4>{
        Vertex{NNP,vec2(0,0),vec3(1.0f),1},
        Vertex{PNP,vec2(1,0),vec3(1.0f),1},
        Vertex{PPP,vec2(1,1),vec3(1.0f),1},
        Vertex{NPP,vec2(0,1),vec3(1.0f),1},
    },
    // Direction:: Left
    std::array<Vertex,4>{
        Vertex{NNN,vec2(0,0),vec3(1.0f),2},
        Vertex{NNP,vec2(1,0),vec3(1.0f),2},
        Vertex{NPP,vec2(1,1),vec3(1.0f),2},
        Vertex{NPN,vec2(0,1),vec3(1.0f),2},
    },

    // Direction::Right
    std::array<Vertex,4>{
        Vertex{PNP,vec2(0,0),vec3(1.0f),3},
        Vertex{PNN,vec2(1,0),vec3(1.0f),3},
        Vertex{PPN,vec2(1,1),vec3(1.0f),3},
        Vertex{PPP,vec2(0,1),vec3(1.0f),3},
    },
    // Direction::Down
    std::array<Vertex,4>{
        Vertex{NNN,vec2(0,0),vec3(1.0f),4},
        Vertex{PNN,vec2(1,0),vec3(1.0f),4},
        Vertex{PNP,vec2(1,1),vec3(1.0f),4},
        Vertex{NNP,vec2(0,1),vec3(1.0f),4},
    },
    // Direction::Up
    std::array<Vertex,4>{
        Vertex{NPP,vec2(0,0),vec3(1.0f),5},
        Vertex{PPP,vec2(1,0),vec3(1.0f),5},
        Vertex{PPN,vec2(1,1),vec3(1.0f),5},
        Vertex{NPN,vec2(0,1),vec3(1.0f),5},
    },
};
constexpr std::array<u32,INDICES_PER_FACE> defaultCubeIndices{
    { 3, 0, 1, 1, 2, 3,},
};

static_assert(defaultCubeFaces.size()==6);
[[maybe_unused]] vec3 getBlockOverlayColor(ivec3 local, const ChunkMetadata* meta) {
    auto temp = meta->blockTemperature[local.x, local.y, local.z];
    auto humidity = meta->blockHumidity[local.x, local.y, local.z];
    return { temp, 0, humidity };
    // TODO: Make a better mapping function?
    // I feel like red = temp and blue = humidity is kinda stupid
};
