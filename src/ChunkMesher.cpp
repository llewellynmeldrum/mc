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

using std::views::enumerate;

extern const std::array<std::array<Vertex, 4>, 6> defaultCubeFaces;
extern const std::array<u32, 6>                   defaultCubeIndices;

static const auto& getDefaultFaceIndices(Direction dir) {
    return defaultCubeIndices[0];
}
static const auto& getDefaultFaceVertexData(Direction dir) {
    return defaultCubeFaces[static_cast<i8>(dir)];
}

template <glm::length_t L, typename T, glm::qualifier Q>
glm::vec<L, T, Q> euclid_mod(glm::vec<L, T, Q> a, glm::vec<L, T, Q> b) {
    auto r = a % b;
    auto mask = glm::lessThan(r, glm::vec<L, T, Q>(0));
    return r + glm::mix(glm::vec<L, T, Q>(0), glm::abs(b), mask);
}
vec3 getBlockOverlayColor(ivec3 local, const ChunkMetadata* meta) {
    auto temp = meta->blockTemperature[local.x, local.y, local.z];
    auto humidity = meta->blockHumidity[local.x, local.y, local.z];
    return { temp, 0, humidity };
    // TODO: Make a better mapping function?
    // I feel like red = temp and blue = humidity is kinda stupid
};
std::array<Block, DirectionCount>
getNeighbourBlocks(const Chunk* chunk, const ivec3& chunk_local,
                   std::span<const Chunk*, DirectionCount> surrounding_chunks) {
    std::array<Block, DirectionCount> res{};

    if (!chunk) {
        LOG_ERROR("Own chunk is null, crashing");
        DEBUG_BREAKPOINT();
    }
    constexpr ivec3 lo = ivec3(0);
    constexpr ivec3 hi = Chunk::Extents;
    for (const auto& [dir, offset] : eachDirOffset) {
        const i32   dir_idx = static_cast<i32>(dir);
        ivec3       neighbourBlockPos = chunk_local + offset;
        const auto* neighbourChunk = surrounding_chunks[dir_idx];

        const bool neighbourInBounds = lmath::isVecInBounds(neighbourBlockPos, lo, hi);

        // TODO: fix refactored chunk meshing, do `make run` and figure it out
        if (neighbourInBounds) [[likely]] {
            const auto& p = neighbourBlockPos;
            assert(chunk);
            res[dir_idx] = (chunk->getBlock(neighbourBlockPos));
        } else [[unlikely]] {
            neighbourBlockPos = euclid_mod(neighbourBlockPos, Chunk::Extents);
            assert(lmath::isVecInBounds(neighbourBlockPos, lo, hi));
            if (neighbourChunk) {
                res[dir_idx] = (neighbourChunk->getBlock(neighbourBlockPos));
            }
        }
    }
    assert(res.size() == 6);
    return res;
};

std::size_t ChunkMesher::emit_chunk_vertex_data(std::vector<Vertex>& out_vertices,
                                                std::vector<u32>&    out_indices,
                                                const World* world_ptr, const Chunk* chunk,
                                                const ChunkMetadata* chunk_meta,
                                                const ivec3          chunk_offset,
                                                const TextureAtlas&  atlas) {
    const auto& world = *world_ptr;
    auto        surrounding_chunks = world.chunkMap.getSurroundingChunks(chunk_offset);
    u32         vtx_count = 0;

    for (const auto& [x, y, z] : EachBlockInChunk()) {
        const vec3 chunk_local = { x, y, z };
        const auto overlayColor = getBlockOverlayColor(chunk_local, chunk_meta);

        Block block = (*chunk)[x, y, z];
        if (block.isAir()) {
            continue;
        }
        auto neighbour_blocks = getNeighbourBlocks(chunk, chunk_local, surrounding_chunks);
        assert(neighbour_blocks.size() == DirectionCount);

        for (const auto& [face_idx, adjacentBlock] : enumerate(neighbour_blocks)) {
            const auto faceDir = static_cast<Direction>(face_idx);
            if (adjacentBlock.isOpaque()) {
                continue;
            }

            ivec3 self_pos = chunk_local + World::chunkToWorldPos(chunk_offset);
            LOG_DEBUG("Emitting face for L:{}, as {} neighbour is NOT opaque.",
                      dbg_fmt(chunk_local), dbg_fmt(faceDir));

            const auto& vtx_data = getDefaultFaceVertexData(faceDir);
            const auto& uv_tex_coords = atlas.remapUVs(block.texture_id(), faceDir, vtx_data);

            for (int i = 0; i < 6; i++) {
                out_indices.push_back(vtx_count + defaultCubeIndices[i]);
                LOG_DEBUG("{},{}->{}", vtx_count, defaultCubeIndices[i], out_indices.back());
            }
            for (const auto& [vertex_idx, vtx] : enumerate(vtx_data)) {
                vec3 chunk_offsetted_vtx_pos = vtx.pos + chunk_local;
                vec2 texture_coords = uv_tex_coords[vertex_idx];
                out_vertices.emplace_back(chunk_offsetted_vtx_pos, texture_coords, overlayColor,
                                          static_cast<i32>(faceDir));
                vtx_count++;
            }

            // 4 vertices emitted per face.
            // ebo should emit
        }
    }
    // TODO:
    return vtx_count;
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
constexpr std::array<u32,6> defaultCubeIndices{
    { 3, 0, 1, 1, 2, 3,},
};
    /*
constexpr std::array<std::array<u32,6>, 6> defaultCubeIndices{
    std::array<u32,6>{ 3, 0, 1, 1, 2, 3,},
    std::array<u32,6>{ 7, 4, 5, 5, 6, 7,},
    std::array<u32,6>{11, 8, 9, 9,10,11,},
    std::array<u32,6>{15,12,13,13,14,15,},
    std::array<u32,6>{19,16,17,17,18,19,},
    std::array<u32,6>{23,20,21,21,22,23,},
};
*/

static_assert(defaultCubeFaces.size()==6);
/*
const std::vector<std::vector<Vertex>> defaultCubeFaces = {
        // Direction::FORWARD,
        std::vector<Vertex>{
            Vertex{.pos={   -0.5f, -0.5f, -0.5f,   }, .txCoords = {0.0f, 1.0f}, .face_direction = 0},
            Vertex{.pos={    0.5f, -0.5f, -0.5f,   }, .txCoords = {1.0f, 1.0f}, .face_direction = 0},
            Vertex{.pos={    0.5f,  0.5f, -0.5f,   }, .txCoords = {1.0f, 0.0f}, .face_direction = 0},
            Vertex{.pos={    0.5f,  0.5f, -0.5f,   }, .txCoords = {1.0f, 0.0f}, .face_direction = 0},
            Vertex{.pos={   -0.5f,  0.5f, -0.5f,   }, .txCoords = {0.0f, 0.0f}, .face_direction = 0},
            Vertex{.pos={   -0.5f, -0.5f, -0.5f,   }, .txCoords = {0.0f, 1.0f}, .face_direction = 0},
        },
        // Direction::BACKWARD,
        std::vector<Vertex>{
            Vertex{.pos={    -0.5f, -0.5f,  0.5f,  }, .txCoords = {0.0f, 1.0f}, .face_direction = 1},
            Vertex{.pos={     0.5f, -0.5f,  0.5f,  }, .txCoords = {1.0f, 1.0f}, .face_direction = 1},
            Vertex{.pos={     0.5f,  0.5f,  0.5f,  }, .txCoords = {1.0f, 0.0f}, .face_direction = 1},
            Vertex{.pos={     0.5f,  0.5f,  0.5f,  }, .txCoords = {1.0f, 0.0f}, .face_direction = 1},
            Vertex{.pos={    -0.5f,  0.5f,  0.5f,  }, .txCoords = {0.0f, 0.0f}, .face_direction = 1},
            Vertex{.pos={    -0.5f, -0.5f,  0.5f,  }, .txCoords = {0.0f, 1.0f}, .face_direction = 1},

        },
        // Direction::LEFT,
        std::vector<Vertex>{
            Vertex{.pos={    -0.5f,  0.5f,  0.5f,  }, .txCoords = {1.0f, 0.0f}, .face_direction = 2},
            Vertex{.pos={    -0.5f,  0.5f, -0.5f,  }, .txCoords = {0.0f, 0.0f}, .face_direction = 2},
            Vertex{.pos={    -0.5f, -0.5f, -0.5f,  }, .txCoords = {0.0f, 1.0f}, .face_direction = 2},
            Vertex{.pos={    -0.5f, -0.5f, -0.5f,  }, .txCoords = {0.0f, 1.0f}, .face_direction = 2},
            Vertex{.pos={    -0.5f, -0.5f,  0.5f,  }, .txCoords = {1.0f, 1.0f}, .face_direction = 2},
            Vertex{.pos={    -0.5f,  0.5f,  0.5f,  }, .txCoords = {1.0f, 0.0f}, .face_direction = 2},
        },
        // Direction::RIGHT,
        std::vector<Vertex>{
            Vertex{.pos={     0.5f,  0.5f,  0.5f,  }, .txCoords = {0.0f, 0.0f}, .face_direction = 3},
            Vertex{.pos={     0.5f,  0.5f, -0.5f,  }, .txCoords = {1.0f, 0.0f}, .face_direction = 3},
            Vertex{.pos={     0.5f, -0.5f, -0.5f,  }, .txCoords = {1.0f, 1.0f}, .face_direction = 3},
            Vertex{.pos={     0.5f, -0.5f, -0.5f,  }, .txCoords = {1.0f, 1.0f}, .face_direction = 3},
            Vertex{.pos={     0.5f, -0.5f,  0.5f,  }, .txCoords = {0.0f, 1.0f}, .face_direction = 3},
            Vertex{.pos={     0.5f,  0.5f,  0.5f,  }, .txCoords = {0.0f, 0.0f}, .face_direction = 3},
        },
        // Direction::DOWN,
        std::vector<Vertex>{
            Vertex{.pos={    -0.5f, -0.5f, -0.5f,  }, .txCoords = {1.0f, 0.0f}, .face_direction = 4},
            Vertex{.pos={     0.5f, -0.5f, -0.5f,  }, .txCoords = {0.0f, 0.0f}, .face_direction = 4},
            Vertex{.pos={     0.5f, -0.5f,  0.5f,  }, .txCoords = {0.0f, 1.0f}, .face_direction = 4},
            Vertex{.pos={     0.5f, -0.5f,  0.5f,  }, .txCoords = {0.0f, 1.0f}, .face_direction = 4},
            Vertex{.pos={    -0.5f, -0.5f,  0.5f,  }, .txCoords = {1.0f, 1.0f}, .face_direction = 4},
            Vertex{.pos={    -0.5f, -0.5f, -0.5f,  }, .txCoords = {1.0f, 0.0f}, .face_direction = 4},
        },
        // Direction::UP,
        std::vector<Vertex>{
            Vertex{.pos={    -0.5f,  0.5f, -0.5f,  }, .txCoords = {0.0f, 1.0f},.face_direction = 5},
            Vertex{.pos={     0.5f,  0.5f, -0.5f,  }, .txCoords = {1.0f, 1.0f},.face_direction = 5},
            Vertex{.pos={     0.5f,  0.5f,  0.5f,  }, .txCoords = {1.0f, 0.0f},.face_direction = 5},
            Vertex{.pos={     0.5f,  0.5f,  0.5f,  }, .txCoords = {1.0f, 0.0f},.face_direction = 5},
            Vertex{.pos={    -0.5f,  0.5f,  0.5f,  }, .txCoords = {0.0f, 0.0f},.face_direction = 5},
            Vertex{.pos={    -0.5f,  0.5f, -0.5f,  }, .txCoords = {0.0f, 1.0f},.face_direction = 5},
        },
};
// clang-format on
*/
