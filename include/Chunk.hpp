#pragma once
#include "Types.h"
#include "Block.hpp"
#include <mdspan>
#include "glmWrapper.hpp"
#include "ChunkHelpers.hpp"
constexpr const i64 CHUNK_XWIDTH = 16;                                        // x/y/z
constexpr const i64 CHUNK_ZWIDTH = 16;                                        // x/y/z
constexpr const i64 CHUNK_HEIGHT = 16;                                        // x/y/z
constexpr const i64 CHUNK_SIZE = CHUNK_XWIDTH * CHUNK_ZWIDTH * CHUNK_HEIGHT;  // x/y/z
#define MD_ACCESS_MACRO(T)                                                                         \
    inline T& operator[](i16 x, i16 y, i16 z) {                                                    \
        return std::mdspan(data.data(), CHUNK_XWIDTH, CHUNK_HEIGHT, CHUNK_ZWIDTH)[x, y, z];        \
    }                                                                                              \
    inline const T& operator[](i16 x, i16 y, i16 z) const {                                        \
        return std::mdspan(data.data(), CHUNK_XWIDTH, CHUNK_HEIGHT, CHUNK_ZWIDTH)[x, y, z];        \
    }

// each attribute of a chunks metadata can be 3d indexed by block (operator[x,y,z])
struct ChunkMetadata {
    ChunkMetadata() = default;
    ~ChunkMetadata() = default;
    struct {
        std::array<f32, CHUNK_SIZE> data{};
        MD_ACCESS_MACRO(f32)
    } blockTemperature;

    struct {
        std::array<f32, CHUNK_SIZE> data{};
        MD_ACCESS_MACRO(f32)
    } blockHumidity;
};
struct Chunk {
    Chunk() = default;
    ~Chunk() = default;
    std::array<Block, CHUNK_SIZE> data{};  // all blocks are implicitly 0, i.e air
    static constexpr ivec3        Extents = { CHUNK_XWIDTH, CHUNK_HEIGHT, CHUNK_ZWIDTH };

    bool isDirty = true;

    MD_ACCESS_MACRO(Block)

    inline void setBlock(this auto& self, BlockType t, i16 x, i16 y, i16 z) {
        self[x, y, z] = Block{ .id = t };
    }

    inline void setBlocks(this auto& self, BlockType t, vec3 pos1, vec3 pos2) {
        vec3 min =
            vec3(std::min(pos1.x, pos2.x), std::min(pos1.y, pos2.y), std::min(pos1.z, pos2.z));
        vec3 max =
            vec3(std::max(pos1.x, pos2.x), std::max(pos1.y, pos2.y), std::max(pos1.z, pos2.z));
        for (i64 x = min.x; x < max.x; x++) {
            for (i64 y = min.y; y < max.y; y++) {
                for (i64 z = min.z; z < max.z; z++) {
                    self.setBlock(t, x, y, z);
                }
            }
        }
    }
    // @brief sets a column of blocks starting at origin of size (1xheightx1) to t
    inline void setColumn(this auto& self, BlockType t, ivec3 origin, i32 height) {
        for (i64 y = origin.y; y < origin.y + height; y++) {
            self.setBlock(t, origin.x, y, origin.z);
        }
    }
    inline void setBlock(BlockType t, vec3 pos) { return setBlock(t, pos.x, pos.y, pos.z); }

    inline Block getBlock(this auto& self, i16 x, i16 y, i16 z) {
        if (x < 0 || x >= CHUNK_XWIDTH)
            return AIR_BLOCK;
        if (y < 0 || y >= CHUNK_HEIGHT)
            return AIR_BLOCK;
        if (z < 0 || z >= CHUNK_ZWIDTH)
            return AIR_BLOCK;
        return self[x, y, z];
    }
    inline Block getBlock(this auto& self, vec3 pos) { return self.getBlock(pos.x, pos.y, pos.z); }
    inline void  fillChunk(BlockType t) {
        for (auto& block : data) {
            block = Block{ .id = t };
        }
    }
};

namespace ForEach {
enum ControlFlow {
    BREAK,
    CONTINUE,
};
template <typename Func>
void BlockInChunk(Func&& func) {
    for (i32 x = 0; x < CHUNK_XWIDTH; x++) {
        for (i32 y = 0; y < CHUNK_HEIGHT; y++) {
            for (i32 z = 0; z < CHUNK_ZWIDTH; z++) {
                switch (func(x, y, z)) {
                case BREAK:
                    goto BREAK_LOOP;
                case CONTINUE:
                    [[fallthrough]];
                default:
                    break;
                }
            }
        }
    }
BREAK_LOOP:
    return;
}

//
//
//
//
//
}  // namespace ForEach
