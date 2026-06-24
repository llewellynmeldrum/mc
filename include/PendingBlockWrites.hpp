#pragma once


#include "Block.hpp"
#include "CoordIteration.hpp"
#include "CoordTypes.hpp"
#include "glmWrapper.hpp"
#include <queue>
#include <type_traits>

#define OVERWRITE_POLICY_LIST   \
    X(OnlyAir)                  \
    X(OnlyGrass)
enum class OverwritePolicy: u8{
#define X(var) var,
    OVERWRITE_POLICY_LIST
#undef X
};

struct PendingBlockWrite{
    OverwritePolicy overwritePolicy;
    WorldChunkCoord sourceChunkCoord;
    WorldBlockPos targetWorldBlockPos;
    BlockType source_block;
};
inline auto operator<=>(PendingBlockWrite lhs, PendingBlockWrite rhs){
    return lhs.sourceChunkCoord<=>rhs.sourceChunkCoord;
}
inline auto operator==(PendingBlockWrite lhs, PendingBlockWrite rhs){
    return lhs.sourceChunkCoord==rhs.sourceChunkCoord;
}

inline PendingBlockWrite makePendingWrite(OverwritePolicy pol, WorldChunkCoord src_coord, BlockType src_type, WorldBlockPos dst_pos){
    return PendingBlockWrite{
        .overwritePolicy = pol,
        .sourceChunkCoord = src_coord,
        .targetWorldBlockPos = dst_pos,
        .source_block = src_type,

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

        default:
        break;
    }
    return false;
}
inline bool canMakeWrite(const PendingBlockWrite& pending, const Block& target){
    return canMakeWrite(pending.overwritePolicy,target);
}


// chunkBlockPos->      worldChunkCoord = deleted
// worldChunkCoord->    chunkBlockPos = deleted

// max heap (prio queue) containing block writes
// since we are placing based on priority anyway, im not sure this being a prio queue is even necessary
using PendingWriteQueue = std::priority_queue<PendingBlockWrite>;

// unordered, heap allocated list of block writes
using PendingWriteList = std::vector<PendingBlockWrite>;

//
//
