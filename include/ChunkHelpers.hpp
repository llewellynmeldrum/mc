#pragma once
#include "Types.h"
#include <utility>
#include "glmWrapper.hpp"
struct Chunk;  // forward decl for using defs

// A ChunkView is a non owning view of a sequence of chunks, paired with their coordinates
using ChunkView = std::pair<ivec3, const Chunk*>;

namespace std{

template<>
struct hash<glm::ivec3>{
    std::size_t operator()(const glm::ivec3& v) const {
        return (std::hash<i32>{}(v.x) << 1) ^  // NOLINT
               (std::hash<i32>{}(v.y) << 2) ^  // NOLINT
               (std::hash<i32>{}(v.z));
    }
};

}
