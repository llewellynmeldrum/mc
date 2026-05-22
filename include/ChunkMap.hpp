#pragma once 
#include "Chunk.hpp"
#include "CoordTypes.hpp"
#include "Geometry.hpp"
#include "ChunkGenerator.hpp"
#include "ChunkHelpers.hpp"

#include <memory>
#include <print>
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

    inline auto getEntry(WorldChunkCoord coord){
        return chunks.at(coord).get();
    }
    inline bool hasEntry(WorldChunkCoord coord){
        return chunks.contains(coord);
    }
    ChunkGenerator generator;
    inline MeshRevisionID getLatestMeshRevisionID(WorldChunkCoord chunkCoord){
        if (!latestMeshRevisionMap.contains(chunkCoord)){
            // chunk has not yet been enqueued for generation!!
            return 0;
        } else{
            return latestMeshRevisionMap.at(chunkCoord);
        }
    }
    inline MeshRevisionID assignNewMeshRevision(WorldChunkCoord chunkCoord){
        if (!latestMeshRevisionMap.contains(chunkCoord)){
            latestMeshRevisionMap.try_emplace(chunkCoord, 0);
            return 0;
        }
        auto& res = latestMeshRevisionMap.at(chunkCoord);
        // this function should mean: 
        // i am assigning a NEW mesh revision.
        //
        // World generation should be the trigger to insert it into the map
        return ++res; 
    }
    std::unordered_map<WorldChunkCoord, MeshRevisionID> latestMeshRevisionMap;
    std::unordered_map<WorldChunkCoord, std::unique_ptr<Chunk>> chunks;
    std::unordered_map<WorldChunkCoord, ChunkGenStatus> genStatus;
    std::unordered_map<WorldChunkCoord, PendingWriteQueue> pendingWritesMap;
    std::unordered_map<WorldChunkCoord, std::unique_ptr<ChunkMetadata>> metadata;
    std::unordered_map<WorldChunkCoord, std::array<const Chunk*, NUM_NEIGHBOURS>> neighbours;
    std::unordered_map<WorldChunkCoord, bool> is_dirty;
    std::unordered_map<WorldChunkCoord, std::unique_ptr<AABB>> boundingBoxes;


    int uploadedChunkCount = 0;
    int pendingWritesAttempted= 0;
    int pendingWritesSuccessful = 0;
    // @Brief:
    // Called at the start of chunk generation
    void initChunk(WorldChunkCoord chunkCoord){
        auto [it, inserted] = latestMeshRevisionMap.try_emplace(chunkCoord);
        assert(inserted && "Entry should not yet exist.");
    }
    // @Brief:
    // Called at the END of chunk generation
    inline void uploadGeneratedChunk(GenResult gen_res){
        ChunkStore& chunkBlocks = gen_res.chunkBlocks;
        ChunkMetadata& chunkMeta = gen_res.meta;
        const auto& deferredWrites = gen_res.deferredWrites;
        const auto& chunkCoord = gen_res.chunkCoord;

        if (!chunks.contains(chunkCoord)) {
            auto [it, emplaced] = chunks.try_emplace(chunkCoord, std::make_unique<Chunk>(chunkBlocks.buffer()));
            
            metadata.emplace(chunkCoord, std::make_unique<ChunkMetadata>(chunkMeta));
            chunks[chunkCoord]->flags.finishedGeneration=true;
            chunks[chunkCoord]->flags.isDirty=true;

            handlePendingWrites(chunkCoord, static_cast<ChunkSpan>(chunkBlocks), deferredWrites);
            updateNeighbourMap(chunkCoord);
            updateBoundingBoxesMap(chunkCoord);
        } else {
            // do we insert the chunk into the map anyway?
            // We might have to decide based on request priority or something idk
        }
    }
    void handlePendingWrites(const WorldChunkCoord chunkCoord, ChunkSpan srcBlocks, const PendingWriteList& newWriteList);


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


    ChunkMetadata*                  getMetadata(WorldChunkCoord pos)const ;
    std::span<const Chunk*const>    getSurroundingChunks(glm::ivec3 pos) const ;
    const AABB*                     getBoundingBox(WorldChunkCoord chunk_offset) const;
    
    inline ChunkGenStatus queryGenStatus(WorldChunkCoord key){
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
