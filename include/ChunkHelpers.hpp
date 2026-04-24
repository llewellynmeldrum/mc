#include "Types.h"
#include <utility>
#include "glmWrapper.hpp"
struct Chunk;  // forward decl for using defs

// A fat chunk stores a chunks position and a ptr to it.
// Mainly used for DS's which map coord->chunk
using FatChunk = std::pair<ivec3, Chunk>;

// A ChunkView is a non owning view of a sequence of chunks, paired with their coordinates
using ChunkView = std::pair<ivec3, const Chunk*>;

struct hashChunkCoord {
    std::size_t operator()(const glm::ivec3& v) const {
        return (std::hash<i32>{}(v.x) << 1) ^  // NOLINT
               (std::hash<i32>{}(v.y) << 2) ^  // NOLINT
               (std::hash<i32>{}(v.z));
    }
};
