#pragma once 
#include "Chunk.hpp"
#include "CoordTypes.hpp"
#include "Geometry.hpp"
#include "ChunkGenerator.hpp"
#include "ChunkHelpers.hpp"
#include "ChunkEntry.hpp"
#include "DEBUG.hpp"
#include "cppslop.hpp"

#include <functional>
#include <memory>
#include <optional>
#include <print>
#include <queue>

// should the mesh queue be a prio queue? 
// idk bro what makes you ask

struct ChunkMap {
    ChunkMap() = default;
    ~ChunkMap() = default;
    ChunkMap(ChunkMap const&) = delete;
    ChunkMap& operator=(ChunkMap const&) = delete;
    ChunkMap(ChunkMap&&) = delete;
    ChunkMap& operator=(ChunkMap&&) = delete;


    ChunkGenerator generator;
    // NOTE: A chunk entry is made right before generation enqueue.
    std::unordered_map<WorldChunkCoord, std::unique_ptr<ChunkEntry>> entries;

    // NOTE: This map should be separate to the main chunkEntries map, as neighbouring chunks 
    // can produce `PendingWrite`s for chunks that havent generated (and thus been added as an entry) yet
    std::unordered_map<WorldChunkCoord, PendingWriteQueue> pendingWritesMap;

    // NOTE: An entry is made into this map right before generation enqueue, 
    // and is **NEVER UNLOADED OR REMOVED**. It persists through mesh unloading
    std::unordered_map<WorldChunkCoord, ChunkState> states;

    inline std::optional<ChunkState*> try_get_state(WorldChunkCoord coord){
        if (states.contains(coord)){
            return std::make_optional(&states.at(coord));
        }else{
            return std::nullopt;
        }
    }
    inline ChunkState* get_state(WorldChunkCoord coord){
        assert(states.contains(coord));
        return &states.at(coord);
    }
    inline bool has_state_entry(WorldChunkCoord coord){
        return states.contains(coord);
    }
    inline bool make_state_entry(WorldChunkCoord coord){
        return states.try_emplace(coord).second;
    }

    inline void launchGenerator(){
        
        generator.launchGenThreads();
    }

    inline ChunkEntry* get_entry_safe(WorldChunkCoord coord){
        if (entries.contains(coord)){
            return entries.at(coord).get();
        }else{
            DEBUG_BREAKPOINT();
            return {};
        }
    }

    inline std::optional<ChunkEntry*> try_get_entry(WorldChunkCoord coord){
        if (entries.contains(coord)){
            return std::make_optional(entries.at(coord).get());
        }else{
            return std::nullopt;
        }
    }
    inline ChunkEntry* get_entry(WorldChunkCoord coord){
        return entries.at(coord).get();
    }
    inline const ChunkEntry* get_entry(WorldChunkCoord coord) const{
        return entries.at(coord).get();
    }
    inline bool has_entry(WorldChunkCoord coord){
        return entries.contains(coord);
    }
    inline ChunkEntry* make_entry(WorldChunkCoord chunkCoord){
        auto [it, inserted] = 
            entries.try_emplace(chunkCoord, 
                std::make_unique<ChunkEntry>(std::move(chunkCoord))
            );
        assert(inserted); // Entry was double created 
        return (*it).second.get();
    }


    // temporary debugging 
    int uploadedChunkCount = 0;
    int pendingWritesAttempted= 0;
    int pendingWritesSuccessful = 0;
    inline void uploadGeneratedChunk(GenResult gen_res){
        ChunkStore& generatedBlocks = gen_res.chunkBlocks;
        ChunkMetadata& generatedMeta = gen_res.meta;
        const auto& deferredWrites = gen_res.deferredWrites;
        const auto& chunkCoord = gen_res.chunkCoord;

        auto* entry = make_entry(chunkCoord);
        auto& state = states.at(chunkCoord);
        // update entry to reflect generation data
        handlePendingWrites(chunkCoord, static_cast<ChunkSpan>(generatedBlocks), deferredWrites);
        
        // why the fuck did they make it (src,dst) fucking AT&T propaganda
        std::ranges::copy(generatedBlocks, entry->block_data.begin());
        entry->metadata=generatedMeta;
        updateNeighbourMap(chunkCoord);
        updateBoundingBoxesMap(chunkCoord);
    }
    void handlePendingWrites(const WorldChunkCoord chunkCoord, ChunkSpan srcBlocks, const PendingWriteList& newWriteList);


    inline MeshRevisionID getLatestMeshRevisionID(WorldChunkCoord chunkCoord){
        if (!entries.contains(chunkCoord)){
            // chunk has not yet been enqueued for generation!!
            return 0; //err? optional?
        } else{
            return entries.at(chunkCoord)->latest_mesh_revision;
        }
    }

    inline MeshRevisionID assignNewMeshRevision(WorldChunkCoord chunkCoord){
        if (!entries.contains(chunkCoord)){
            // chunk has not yet been enqueued for generation!!
            return 0; //err? optional?
        } 
        auto& res = entries.at(chunkCoord)->latest_mesh_revision;
        return ++res; 
    }

    const AABB* getBoundingBox(WorldChunkCoord chunk_offset) const;
    

private:
    void           updateNeighbourMap(WorldChunkCoord chunkCoord);
    void           updateBoundingBoxesMap(WorldChunkCoord chunkCoord);


};
