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



    inline void clear(){
        pending_writes.clear();
        entries.clear();
    }


    // NOTE: ENTRY MADE: Either on GenData upload, or when a chunk tries to write to it
    // NOTE: ENTRY DELETED: When the queue for a chunk is empty. Not sure how i feel about this.
    HashMap<WorldChunkCoord, PendingWriteList> pending_writes;


    // NOTE: ENTRY MADE: on enqueue into GenJobs (before generation)
    // NOTE: ENTRY DELETED: World regen?
    HashMap<WorldChunkCoord, ChunkEntry> entries;



    auto* get_or_emplace_pwq(WorldChunkCoord coord){
        return pending_writes.get_or_insert(coord,{});
    }


    // temporary debugging 
    int uploadedChunkCount = 0;
    int pendingWritesAttempted= 0;
    int pendingWritesSuccessful = 0;

    const AABB* getBoundingBox(WorldChunkCoord chunk_offset) const;
    

private:


};
