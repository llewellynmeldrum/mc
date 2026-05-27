#pragma once


#include "Block.hpp"
#include "CoordTypes.hpp"
#include "glmWrapper.hpp"
#include <queue>

struct PendingBlockWrite{
    BlockType block;
    WorldBlockPos targetWorldBlockPos;
    u8 priority;
    WorldChunkCoord sourceChunkCoord;
    PendingBlockWrite(const WorldChunkCoord _sourceChunkCoord, const WorldBlockPos& _blockPos, BlockType bt);

    PendingBlockWrite() = delete;
    ~PendingBlockWrite() = default;

    inline bool operator==(this const PendingBlockWrite& lhs, const PendingBlockWrite& rhs){
        if (lhs.priority==rhs.priority) return true;
        else return lhs.sourceChunkCoord == rhs.sourceChunkCoord;
    }
    inline auto operator<=>(this const PendingBlockWrite& lhs, const PendingBlockWrite& rhs){
        auto cmp1 = lhs.priority<=>rhs.priority;
        if ( cmp1 != 0) return cmp1; //NOLINT
        // TIE BREAK DETERMINISTICALLY BASED ON SOURCE CHUNK
        else return lhs.sourceChunkCoord <=> rhs.sourceChunkCoord;
    }
};

// chunkBlockPos->      worldChunkCoord = deleted
// worldChunkCoord->    chunkBlockPos = deleted

// max heap (prio queue) containing block writes
// since we are placing based on priority anyway, im not sure this being a prio queue is even necessary
using PendingWriteQueue = std::priority_queue<PendingBlockWrite>;

// unordered, heap allocated list of block writes
using PendingWriteList = std::vector<PendingBlockWrite>;

//
//
