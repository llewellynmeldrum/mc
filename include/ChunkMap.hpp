#include "Chunk.hpp"
#include "Geometry.hpp"
#include "ChunkGenerator.hpp"
#include "ChunkHelpers.hpp"

#include <memory>
#include <queue>

// should the mesh queue be a prio queue? 
// idk bro what makes you ask
struct ChunkGenStatus{
    u32 hasEntry:               1 = 0;
    u32 finishedHeightmap:      1 = 0;
    u32 finishedTrees:          1 = 0;
    // ...

    u32 inProgress:             1 = 0; // currently enqueued for generation
    u32 finishedAll:            1 = 0;
};
struct ChunkMap {
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
    std::unordered_map<WorldChunkCoord, std::unique_ptr<Chunk>> chunks;
    std::unordered_map<WorldChunkCoord, ChunkGenStatus> genStatus;
    std::unordered_map<WorldChunkCoord, PendingWriteQueue> pendingWritesMap;
    std::unordered_map<WorldChunkCoord, std::unique_ptr<ChunkMetadata>> metadata;
    std::unordered_map<WorldChunkCoord, std::array<const Chunk*, NUM_NEIGHBOURS>> neighbours;
    std::unordered_map<WorldChunkCoord, bool> is_dirty;
    std::unordered_map<WorldChunkCoord, std::unique_ptr<AABB>> boundingBoxes;
    const AABB*   getBoundingBox(WorldChunkCoord chunk_offset) const;

    inline Chunk& operator[](const ChunkBlockPos& chunkLocal) {
        return *chunks.at(chunkLocal).get();
    }

    inline void uploadGeneratedChunk(GenResult gen_res){
        ChunkStore& chunkBlocks = gen_res.chunkBlocks;
        ChunkMetadata& chunkMeta = gen_res.meta;
        const auto& deferredWrites = gen_res.deferredWrites;
        const auto& chunkCoord = gen_res.head.chunkCoord;

        if (!chunks.contains(chunkCoord)) {
            chunks.emplace(chunkCoord, std::make_unique<Chunk>(chunkBlocks.buffer()));
            metadata.emplace(chunkCoord, std::make_unique<ChunkMetadata>(chunkMeta));
            chunks[chunkCoord]->flags.finishedGeneration=true;

            handlePendingWrites(chunkCoord, static_cast<ChunkSpan>(chunkBlocks), deferredWrites);
            updateNeighbourMap(chunkCoord);
            updateBoundingBoxesMap(chunkCoord);
            markGenerated(chunkCoord);
        } else {
            // do we insert the chunk into the map anyway?
            // We might have to decide based on request priority or something idk
        }
    }
    void handlePendingWrites(
        const WorldChunkCoord chunkCoord, 
        ChunkSpan srcBlocks, 
        const PendingWriteList& newWriteList)
    {
        // 1. apply any pending writes TO CURRENT chunk which exist on the map.
        if (pendingWritesMap.contains(chunkCoord)){
            PendingWriteQueue& writesForMe = pendingWritesMap.at(chunkCoord);
            while (!writesForMe.empty()){
                const auto write = writesForMe.top(); writesForMe.pop();
                srcBlocks.tryWrite(write);
            }
        }
        
        // 2. Apply any NEW pending writes TO OTHER chunks from pwl
        for (const auto& write: newWriteList){
            // a.) if the TARGET chunk exists, apply the write IMMEDIATELY to the TARGET chunk
            const auto& targetChunkCoord = toWorldChunkCoord(write.blockPos);
            if (chunks.contains(targetChunkCoord)){
                chunks.at(targetChunkCoord)->tryWrite(write);
            }else{
                // b.) if the TARGET chunk DOESNT exist, create an entry in the pending writes map,
                // and then enqueue the write.
                bool mapEntryExists = pendingWritesMap.contains(write.blockPos);
                if (!mapEntryExists){
                    pendingWritesMap.emplace(write.blockPos, std::priority_queue<PendingBlockWrite>{});
                }
                pendingWritesMap.at(write.blockPos).push(write);
            }
        }
    }


    std::array<ChunkStore, NUM_NEIGHBOURS> 
        copySurroundingChunks(WorldChunkCoord pos) const;

    bool isDirty(WorldChunkCoord pos) const;
    bool isClean(WorldChunkCoord pos) const;
    bool isMeshing(WorldChunkCoord pos) const;
    bool isGenerated(WorldChunkCoord pos) const ;
    bool isMeshed(WorldChunkCoord pos) const ;
    void markDirty(WorldChunkCoord pos);
    void markClean(WorldChunkCoord pos);
    void markMeshing(WorldChunkCoord pos);
    void markGenerated(WorldChunkCoord pos);
    void markMeshed(WorldChunkCoord pos);


    ChunkMetadata* getMetadata(WorldChunkCoord pos)const ;
    std::array<const Chunk*, NUM_NEIGHBOURS> getSurroundingChunks(WorldChunkCoord pos) const;
    
    void           generate(WorldChunkCoord pos);
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
    void           updateNeighbourMap(WorldChunkCoord chunkCoord);
    void           updateBoundingBoxesMap(WorldChunkCoord chunkCoord);


};
