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

#include "Assertion.hpp"

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
    // NOTE: ENTRY MADE: on enqueue into MeshJobs (before meshing)
    std::unordered_map<WorldChunkCoord, MeshRevisionID> currentMeshRevision;
    // NOTE: ENTRY DELETED: 

    // NOTE: ENTRY MADE: on enqueue into GenJobs (before generation)
    // NOTE: ENTRY DELETED: World regen?
    std::unordered_map<WorldChunkCoord, std::unique_ptr<ChunkEntry>> entries;

    // NOTE: A chunk MeshEntry contains information about the currently loaded mesh.
    // NOTE: ENTRY MADE: on deqeueue from MeshResults (before mesh upload)
    // NOTE: ENTRY DELETED: Idk
    std::unordered_map<WorldChunkCoord, std::unique_ptr<MeshEntry>> meshEntries;

    // NOTE: ENTRY MADE: Either on GenData upload, or when a chunk tries to write to it
    // NOTE: ENTRY DELETED: When the queue for a chunk is empty. Not sure how i feel about this.
    std::unordered_map<WorldChunkCoord, PendingWriteQueue> pendingWritesMap;

    // NOTE: An entry is made into this map right before generation enqueue, 
    // and is **NEVER UNLOADED OR REMOVED**. It persists through mesh unloading
    // NOTE: ENTRY MADE: on enqueue into GenJobs (before generation)
    // NOTE: ENTRY DELETED: NEVER
    std::unordered_map<WorldChunkCoord, ChunkState> states;


    inline void launchGenerator(){
        generator.launchGenThreads();
    }

    inline MeshRevisionID& get_current_mesh_revision(WorldChunkCoord coord){
        auto [it, inserted] = currentMeshRevision.try_emplace(coord,0);
        return it->second;
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



    inline std::optional<MeshEntry*> try_get_meshEntry(WorldChunkCoord coord){
        if (meshEntries.contains(coord)){
            return std::make_optional(meshEntries.at(coord).get());
        }else{
            return std::nullopt;
        }
    }

    inline std::optional<ChunkState*> try_get_state(WorldChunkCoord coord){
        if (states.contains(coord)){
            return std::make_optional(&states.at(coord));
        }else{
            return std::nullopt;
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

    inline MeshEntry* make_mesh_entry(WorldChunkCoord chunkCoord, std::size_t meshRevisionID){
        auto [it, inserted] = 
            meshEntries.try_emplace(chunkCoord, 
                std::make_unique<MeshEntry>(meshRevisionID)
            );
        assert(inserted); // Entry was double created 
        return (*it).second.get();
    }
    inline MeshEntry* assign_mesh_entry(WorldChunkCoord chunkCoord, std::size_t meshRevisionID){
        auto uptr = std::make_unique<MeshEntry>(meshRevisionID);
        auto [it,inserted] = meshEntries.insert_or_assign(chunkCoord,std::move(uptr));
        return it->second.get();
    }

    // temporary debugging 
    int uploadedChunkCount = 0;
    int pendingWritesAttempted= 0;
    int pendingWritesSuccessful = 0;

    void uploadGeneratedChunk(GenResult gen_res);
    void handlePendingWrites(const WorldChunkCoord chunkCoord, ChunkSpan srcBlocks, const PendingWriteList& newWriteList);



    const AABB* getBoundingBox(WorldChunkCoord chunk_offset) const;
    

private:
    void           updateNeighbourMap(WorldChunkCoord chunkCoord);
    void           updateBoundingBoxesMap(WorldChunkCoord chunkCoord);


};
