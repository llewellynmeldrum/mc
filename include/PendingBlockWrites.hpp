#pragma once


#include "Block.hpp"
#include "ChunkView.hpp"
#include "CoordIteration.hpp"
#include "CoordTypes.hpp"
#include "glmWrapper.hpp"
#include <queue>
#include <type_traits>

#define OVERWRITE_POLICY_LIST   \
    X(OnlyAir)                  \
    X(Everything)                  \
    X(OnlyLeaves)                  \
    X(OnlyGrass)
enum class OverwritePolicy: u8{
#define X(var) var,
    OVERWRITE_POLICY_LIST
#undef X
};

struct PendingBlockWrite{
    OverwritePolicy policy;
    WorldChunkCoord source_chunk;
    WorldBlockPos target_world;
    BlockType new_block;
};
inline auto operator<=>(PendingBlockWrite lhs, PendingBlockWrite rhs){
    return lhs.source_chunk<=>rhs.source_chunk;
}
inline auto operator==(PendingBlockWrite lhs, PendingBlockWrite rhs){
    return lhs.source_chunk==rhs.source_chunk;
}

inline PendingBlockWrite makePendingWrite(OverwritePolicy pol, WorldChunkCoord src_coord, BlockType src_type, WorldBlockPos dst_pos){
    return PendingBlockWrite{
        .policy = pol,
        .source_chunk = src_coord,
        .target_world = dst_pos,
        .new_block = src_type,

    };
}

inline bool canMakeWrite(const OverwritePolicy& policy, const Block& target){
    switch(policy){
        case OverwritePolicy::OnlyAir:
            return target.type==BlockType::AIR; 
        break;

        case OverwritePolicy::OnlyGrass:
            return target.type==BlockType::GRASS_BLOCK;
        break;
        case OverwritePolicy::OnlyLeaves:
            return target.type==BlockType::OAK_LEAF;
        break;
        case OverwritePolicy::Everything:
            return true;
        break;

        default:
        break;
    }
    return false;
}
inline bool canMakeWrite(const PendingBlockWrite& pending, const Block& target){
    return canMakeWrite(pending.policy,target);
}


// chunkBlockPos->      worldChunkCoord = deleted
// worldChunkCoord->    chunkBlockPos = deleted

// max heap (prio queue) containing block writes
// since we are placing based on priority anyway, im not sure this being a prio queue is even necessary

// unordered, heap allocated list of block writes
using PendingWriteList = std::vector<PendingBlockWrite>;

bool tryWrite(PendingBlockWrite write, ChunkView view);
//
//
