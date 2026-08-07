#pragma once 
#include <functional>
#include <memory>
#include <optional>
#include <print>
#include <queue>
#include <type_traits>

#include "ChunkEntryEnums.hpp"
#include "Types.h"
#include "ChunkNeighbourhood.hpp"
#include "HashMap.hpp"
#include "Chunk.hpp"
#include "CoordTypes.hpp"
#include "Geometry.hpp"
#include "ChunkHelpers.hpp"
#include "ChunkEntry.hpp"
#include "Breakpoints.hpp"
#include "cppslop.hpp"
#include "CommonConcepts.hpp"
#include "Assertion.hpp"

struct ChunkMap {
public:
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
    template<typename T>
    requires chunk_store_compatible<T>
    inline ChunkNeighbourhoodSnapshot<T> take_neighbourhood_snapshot(WorldChunkCoord center) const {
        const auto* entry = entries.try_get(center);
        std::vector<const ChunkEntry*> neighbour_entries;
        neighbour_entries.reserve(N_CARDINAL_DIRECTIONS);
        assert(entry);
        for (const auto neighbour_coord: entry->cardinal_neighbour_coords()){
            auto* neighbour_entry = entries.try_get(neighbour_coord);
            assert(neighbour_entry);
            neighbour_entries.push_back(neighbour_entry);
        }
        return ChunkNeighbourhoodSnapshot<T>(entry,neighbour_entries);
    }

};
