#pragma once 
#include "HashMap.hpp"
#include "Chunk.hpp"
#include "CoordTypes.hpp"
#include "Geometry.hpp"
#include "ChunkGenerator.hpp"
#include "ChunkHelpers.hpp"
#include "ChunkEntry.hpp"
#include "Breakpoints.hpp"
#include "cppslop.hpp"
#include "CommonConcepts.hpp"

#include <functional>
#include <memory>
#include <optional>
#include <print>
#include <queue>
#include <type_traits>

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

    inline void launchGenerator(){
        generator.launchGenThreads();
    }

    ChunkGenerator generator;
    // NOTE: ENTRY MADE: on enqueue into MeshJobs (before meshing)
    HashMap<WorldChunkCoord, MeshRevisionID> current_mesh_revision;
    // NOTE: ENTRY DELETED: 

    // NOTE: ENTRY MADE: on enqueue into GenJobs (before generation)
    // NOTE: ENTRY DELETED: World regen?
    HashMap<WorldChunkCoord, std::unique_ptr<ChunkEntry>> chunk_entries;

    // NOTE: A chunk MeshEntry contains information about the currently loaded mesh.
    // NOTE: ENTRY MADE: on deqeueue from MeshResults (before mesh upload)
    // NOTE: ENTRY DELETED: Idk
    HashMap<WorldChunkCoord, std::unique_ptr<MeshEntry>> mesh_entries;

    // NOTE: ENTRY MADE: Either on GenData upload, or when a chunk tries to write to it
    // NOTE: ENTRY DELETED: When the queue for a chunk is empty. Not sure how i feel about this.
    HashMap<WorldChunkCoord, PendingWriteQueue> pending_writes;

    // NOTE: An entry is made into this map right before generation enqueue, 
    // and is **NEVER UNLOADED OR REMOVED**. It persists through mesh unloading
    // NOTE: ENTRY MADE: on enqueue into GenJobs (before generation)
    // NOTE: ENTRY DELETED: NEVER
    HashMap<WorldChunkCoord, ChunkState> states;


    inline MeshRevisionID get_current_mesh_revision(WorldChunkCoord coord){
        auto current_revision = *current_mesh_revision.get_or_insert(coord,0);
        return current_revision;
    }

    inline bool has_pending_writes(WorldChunkCoord coord){
        return pending_writes.if_contains_else(
            coord,
            [](PendingWriteQueue& pwq){
                return !pwq.empty();
            },
            [](){
                return false;
            }
        );
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
