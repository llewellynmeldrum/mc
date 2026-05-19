#include "Chunk.hpp"
#include "Geometry.hpp"
#include "ChunkGenerator.hpp"
#include "ChunkHelpers.hpp"

#include <memory>
#include <queue>

// should the mesh queue be a prio queue?
struct ChunkGenStatus{
    u32 hasEntry:               1 = 0;
    u32 finishedHeightmap:      1 = 0;
    u32 finishedTrees:          1 = 0;
    // ...

    u32 inProgress:             1 = 0; // currently enqueued for generation
    u32 finishedAll:            1 = 0;
};
struct ChunkMap {
std::array<ChunkStore, NUM_NEIGHBOURS> copySurroundingChunks(ivec3 pos) const;
    ChunkMap() = default;
    ~ChunkMap() = default;

    // MOVE ONLY!
    ChunkMap(ChunkMap const&) = delete;
    ChunkMap& operator=(ChunkMap const&) = delete;
    ChunkMap(ChunkMap&&) = delete;
    ChunkMap& operator=(ChunkMap&&) = delete;
    inline void setupChunkMap(){
        generator.setupChunkGenerator();
    }

    ChunkGenerator generator;
    // chunk map
    std::unordered_map<ivec3, std::unique_ptr<Chunk>> chunks;
    std::unordered_map<ivec3, ChunkGenStatus> genStatus;
    std::unordered_map<ivec3, PendingWriteQueue> pendingWritesMap;
    std::unordered_map<ivec3, std::unique_ptr<ChunkMetadata>> metadata;
    std::unordered_map<ivec3, std::array<const Chunk*, NUM_NEIGHBOURS>> neighbours;
    std::unordered_map<ivec3, bool> is_dirty;

    inline Chunk& operator[](const ivec3& chunkLocal) {
        return *chunks.at(chunkLocal).get();
    }

    inline void uploadGeneratedChunk(GenResult gen_res){
        auto&& chunkBlocks = std::move(gen_res.chunkBlocks);
        auto&& chunkMeta = std::move(gen_res.meta);
        const auto& chunkPos = gen_res.head.worldOffset;

        if (!chunks.contains(chunkPos)) {
            chunks.emplace(chunkPos, std::make_unique<Chunk>(chunkBlocks.buffer()));
            metadata.emplace(chunkPos, std::make_unique<ChunkMetadata>(chunkMeta));
            chunks[chunkPos]->flags.finishedGeneration=true;

            handlePendingWrites(chunkPos, static_cast<ChunkSpan>(chunkBlocks), gen_res.deferredWrites);
            updateNeighbourMap(chunkPos);
            updateBoundingBoxesMap(chunkPos);
            markGenerated(chunkPos);
        } else {
            // do we insert the chunk into the map anyway?
            // We might have to decide based on request priority or something idk
        }
    }
    void handlePendingWrites(
        const ivec3 src_wpos, 
        ChunkSpan srcBlocks, 
        const PendingWriteList& newWriteList)
    {
        // 1. apply any pending writes TO CURRENT chunk which exist on the map.
        if (pendingWritesMap.contains(src_wpos)){
            PendingWriteQueue& writesForSrc = pendingWritesMap.at(src_wpos);
            while (!writesForSrc.empty()){
                const auto write = writesForSrc.top();
                writesForSrc.pop();
                srcBlocks.tryWrite(write);
            }
        }
        
        // 2. Apply any pending writes TO OTHER chunks from pwl
        for (const auto& write: newWriteList){
            // a.) if the OTHER chunk exists, apply the write
            const auto& targetChunkCoord = worldToChunkCoord(write.worldPos);
            if (chunks.contains(targetChunkCoord)){
                const auto& chunkLocal = worldToChunkCoord(write.worldPos);
                srcBlocks.tryWrite(write);
            }else{
                // b.) if not, add to the map, so it can be applied later,
                // in the worst case upon its generation.
                bool mapEntryExists = pendingWritesMap.contains(write.worldPos);
                if (!mapEntryExists){
                    pendingWritesMap.emplace(write.worldPos, std::priority_queue<PendingBlockWrite>{});
                }
                pendingWritesMap.at(write.worldPos).push(write);
            }
        }
    }


    bool isDirty(ivec3 pos) const;
    bool isClean(ivec3 pos) const;
    bool isMeshing(ivec3 pos) const;
    bool isGenerated(ivec3 pos) const ;
    bool isMeshed(ivec3 pos) const ;
    void markDirty(ivec3 pos);
    void markClean(ivec3 pos);
    void markMeshing(ivec3 pos);
    void markGenerated(ivec3 pos);
    void markMeshed(ivec3 pos);

    // world space bounding boxes for chunks (used in frustum culling)
    std::unordered_map<ivec3, std::unique_ptr<AABB>> boundingBoxes;
    const AABB*   getBoundingBox(ivec3 chunk_offset) const;

    ChunkMetadata* getMetadata(ivec3 pos)const ;
    std::array<const Chunk*, NUM_NEIGHBOURS> getSurroundingChunks(ivec3 pos) const;
    
    void           generate(ivec3 pos);
    const Chunk*   getNeighbourByOffset(ivec3 chunk_offset, ivec3 local_offset) const;
    inline ChunkGenStatus queryGenStatus(ivec3 key){
            auto it = genStatus.find(key);
            if (it == genStatus.end()){
                // not found, definitely needs to be generated
                return {};
            } else{
                return it->second;
            }
    }
private:
    void           updateNeighbourMap(ivec3 pos);
    void           updateBoundingBoxesMap(ivec3 chunkCoord);


};
