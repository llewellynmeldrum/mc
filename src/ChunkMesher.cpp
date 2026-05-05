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
#include <ranges>
#include <tuple>
extern const std::vector<std::vector<Vertex>> defaultCubeFaces;

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
// BUG: issue is probably in getNeighbourBlocks. Need to ensure blocks that dont exist count
// as air
// BUG: ALSO, are you sure neighbour blocks contains an ordered, 6 long list where idx =
// direction?
std::vector<Block> getNeighbourBlocks(const Chunk* chunk, const ivec3& chunk_local,
                                      std::span<const Chunk*, DirectionCount> surrounding_chunks) {
    std::vector<Block> res;
    res.reserve(DirectionCount);

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
            res.push_back(chunk->getBlock(neighbourBlockPos));
        } else [[unlikely]] {
            neighbourBlockPos = euclid_mod(neighbourBlockPos, Chunk::Extents);
            assert(lmath::isVecInBounds(neighbourBlockPos, lo, hi));
            if (neighbourChunk) {
                res.push_back(neighbourChunk->getBlock(neighbourBlockPos));
            } else {
                res.emplace_back();
            }
        }
    }
    assert(res.size() == 6);
    return res;
};

std::size_t ChunkMesher::emit_chunk_vertices(std::vector<Vertex>& out_vertices,
                                             const World* world_ptr, const Chunk* chunk,
                                             const ChunkMetadata* chunk_meta,
                                             const ivec3 chunk_offset, const TextureAtlas& atlas) {
    const auto& world = *world_ptr;
    auto        surrounding_chunks = world.chunks.getSurroundingChunks(chunk_offset);
    std::size_t vtx_count = 0;

    ForEach::BlockInChunk([&](i32 x, i32 y, i32 z) {
        const vec3 chunk_local = { x, y, z };
        const auto overlayColor = getBlockOverlayColor(chunk_local, chunk_meta);

        Block block = (*chunk)[x, y, z];
        if (block.isAir()) {
            return ForEach::CONTINUE;
        }
        std::vector<Block> neighbour_blocks =
            getNeighbourBlocks(chunk, chunk_local, surrounding_chunks);
        assert(neighbour_blocks.size() == DirectionCount);

        // clang-format off
        std::size_t face_idx{0};
        for (const auto adjacentBlock : neighbour_blocks) {
            const auto faceDir = static_cast<Direction>(face_idx);
            if (adjacentBlock.isOpaque()){ 
                face_idx++;
//                LOG_DEBUG("not Emitting face for L:{}, as {} neighbour is opaque.",dbg_fmt(chunk_local), dbg_fmt(faceDir));
                continue; 
            }

//            ivec3 self_pos = chunk_local+ World::chunkToWorldPos(chunk_offset);
//            LOG_DEBUG("Emitting face for L:{}, as {} neighbour is NOT opaque.",dbg_fmt(chunk_local), dbg_fmt(faceDir));
            // no point in emitting a face that is directly facing an opaque block

            const auto& vtx_data = getDefaultFaceVertexData(faceDir);
            const auto& uv_tex_coords =
            atlas.remapUVs(block.texture_id(), faceDir, vtx_data);

            // TODO: use std::enumerate here
            std::size_t vtx_idx{0};
            for  (const auto& vtx : vtx_data) {
                ivec3 chunk_offsetted_vtx_pos = vtx.pos+chunk_local;
                vec2 texture_coords = uv_tex_coords[vtx_idx];
                // heap buffer overflow V, perhaps in uv_tex_coords, vtx_idx could be wrong
                out_vertices.emplace_back(
                    chunk_offsetted_vtx_pos,
                    texture_coords,
                    overlayColor,
                    static_cast<i32>(faceDir)
                );
                vtx_count++;
                vtx_idx++;
            }
            face_idx++;
        }

        return ForEach::CONTINUE;
    });
    // TODO:
    return vtx_count;
}
// clang-format off
// 
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
