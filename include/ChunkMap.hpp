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


    inline void mark_neighbours_dirty(WorldChunkCoord key, std::string_view reason="N/A"){
        auto lo = key.raw() + glm::ivec2{-1};
        auto hi = key.raw() + glm::ivec2{2};
        ForEachInRangeEx(lo,hi,[&](i32 x, i32 z){
            entries.if_contains(
                WorldChunkCoord{x,z},
                [&](ChunkEntry& neighbour_entry){
                    neighbour_entry.mark_mesh_dirty(reason);
                }
            );
        });
    }

    // NOTE: ENTRY MADE: Either on GenData upload, or when a chunk tries to write to it
    // NOTE: ENTRY DELETED: When the queue for a chunk is empty. Not sure how i feel about this.
    HashMap<WorldChunkCoord, PendingWriteQueue> pending_writes;


    // NOTE: ENTRY MADE: on enqueue into GenJobs (before generation)
    // NOTE: ENTRY DELETED: World regen?
    HashMap<WorldChunkCoord, ChunkEntry> entries;



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

    void upload_generated_chunk(GenResult gen_res);
    void handlePendingWrites(const WorldChunkCoord chunkCoord, ChunkSpan srcBlocks, const PendingWriteList& newWriteList);



    const AABB* getBoundingBox(WorldChunkCoord chunk_offset) const;
    

private:
    void           update_neighbour_map(WorldChunkCoord chunkCoord);
    void           update_bounding_boxes_map(WorldChunkCoord chunkCoord);


};
