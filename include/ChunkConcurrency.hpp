#pragma once 
#include "Block.hpp"
#include "CoordTypes.hpp"
#include "glmWrapper.hpp"

struct PendingBlockWrite{
    BlockType block;
    WorldBlockPos blockPos;
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
