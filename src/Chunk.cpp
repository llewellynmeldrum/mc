#include "Chunk.hpp"
#include "ChunkHelpers.hpp"

std::atomic<u32> id_counter{0};
// Higher priority blocks are preferred to overwrite 
// lower priority blocks during PENDING BLOCK WRITES in worldgen.
// Priority is ignored during other phases of generation, i.e it is only considered
// in the case of a pending blockWrite
const std::unordered_map<BlockType, u8> BlockPriorityMap{
    { BlockType::null,              0},
    { BlockType::empty,             0},
    { BlockType::AIR,               0},
    { BlockType::DIRT_BLOCK,        1},
    { BlockType::GRASS_BLOCK,       3},
    { BlockType::STONE_BLOCK,       2},
};

PendingBlockWrite::PendingBlockWrite(const ivec3 sourceChunkwpos, const ivec3& targetWPos, BlockType bt)
    : block(bt)
    , worldPos(targetWPos)
    ,priority(BlockPriorityMap.at(bt))
    ,sourceChunkWorldPos(sourceChunkwpos)
    {}

template<
std::size_t xExtent,
std::size_t yExtent,
std::size_t zExtent>
bool BaseBlockSpan<xExtent,yExtent,zExtent>::tryWrite(PendingBlockWrite write){
    const auto& chunkLocal = worldToChunkCoord(write.worldPos);
    Block& curBlock = span[write.worldPos.x,write.worldPos.y,write.worldPos.z];
    // the higher 'priority' block wins.
    const auto& newPrio = write.priority;
    const auto& curPrio = BlockPriorityMap.at(curBlock.id);
    if (newPrio > curPrio){
        curBlock = write.block;
    }
}
