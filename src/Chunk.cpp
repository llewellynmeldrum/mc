#include "Chunk.hpp"
#include "ChunkHelpers.hpp"
#include "CoordTypes.hpp"

std::atomic<u32> id_counter{0};
// Higher priority blocks are preferred to overwrite 
// lower priority blocks during PENDING BLOCK WRITES in worldgen.
// Priority is ignored during other phases of generation, i.e it is only considered
// in the case of a pending blockWrite
// NOTE:
// In order for this model to work without some other deterministic tiebreaker,
// isnt it required that every block has a distinct priority?
// Because elsewise, 2 chunks could write to the same spot with a distinct blocks A and B which have identical priority, in which case the order of loading the chunks would determine which would win.
// In other words, if every block has a different priority, its deterministic, since load order doesnt matter if it ends up being the same block anyway.
// WARNING: 
// ALL BLOCKS MUST HAVE DIFFERENT PRIORITY!! SEE ABOVE FOR WHY
const std::unordered_map<BlockType, u8> BlockPriorityMap{
    { BlockType::null,              0},
    { BlockType::empty,             0},
    { BlockType::AIR,               0},
    { BlockType::DIRT_BLOCK,        1},
    { BlockType::GRASS_BLOCK,       3},
    { BlockType::STONE_BLOCK,       2},
};
constexpr inline u8 BlockPriority(BlockType bt){
    return BlockPriorityMap.at(bt);
}
constexpr inline u8 BlockPriority(Block b){
    return BlockPriorityMap.at(b.type);
}

PendingBlockWrite::PendingBlockWrite(const WorldChunkCoord _sourceChunkCoord, const WorldBlockPos& _blockPos, BlockType bt) :
    block(bt),
    targetWorldBlockPos(_blockPos),
    priority(BlockPriority(bt)),
    sourceChunkCoord(_sourceChunkCoord){}

bool Chunk::tryWrite(PendingBlockWrite write){
    auto targetChunkLocal = toChunkBlockPos(write.targetWorldBlockPos);
    Block& curBlock = this->at(targetChunkLocal);
    // the higher 'priority' block wins.
    const auto& newPrio = write.priority;
    const auto& curPrio = BlockPriority(curBlock);
    if (newPrio > curPrio){
        //BUG:  Tiebreaker not being used
        curBlock = write.block;
        return true;
    }
    return false;
}
bool ChunkSpan::tryWrite(PendingBlockWrite write){
    // turn the targetChunkCoord and write.WorldBlockPos to targetChunkBlockPos
    auto targetChunkLocal = toChunkBlockPos(write.targetWorldBlockPos);
    Block& curBlock = this->at(targetChunkLocal);
    // the higher 'priority' block wins.
    const auto& newPrio = write.priority;
    const auto& curPrio = BlockPriority(curBlock);
    if (newPrio > curPrio){
        //BUG:  Tiebreaker not being used, because currently existing blocks dont know where they came from
        curBlock = write.block;
        return true;
    }
    return false;
}
