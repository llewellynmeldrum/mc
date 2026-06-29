#pragma once 
#include "Chunk.hpp"
#include "ChunkConcurrency.hpp"
#include "CommonUtils.hpp"
#include "CoordTypes.hpp"
#include "HashMap.hpp"
#include "glmWrapper.hpp"
#include <algorithm>
#include <memory>
#include <print>

#include "Camera.hpp"

struct World {
public:
    World() = default;
    ~World() = default;
    World(World const&) = delete;
    World& operator=(World const&) = delete;
    World(World&&) = delete;
    World& operator=(World&&) = delete;
    void setup(){
    }

    static constexpr u64 seed = 1237;

    // NOTE: ENTRY MADE: on enqueue into MeshJobs (before meshing)
    HashMap<WorldChunkCoord, MeshRevisionID> current_mesh_revision;



    // NOTE: ENTRY MADE: Either on GenData upload, or when a chunk tries to write to it
    // NOTE: ENTRY DELETED: When the queue for a chunk is empty. Not sure how i feel about this.
    HashMap<WorldChunkCoord, PendingWriteQueue> pending_writes;


    // NOTE: ENTRY MADE: on enqueue into GenJobs (before generation)
    // NOTE: ENTRY DELETED: World regen?
    HashMap<WorldChunkCoord, ChunkEntry> entries;

    // NOTE: Rather than rechecking every frame, we keep a list of candidates.
    // Before upload as a job, each candidate is rechecked to make sure it still qualifies.
    std::unordered_set<WorldChunkCoord> persistent_mesh_candidates;

    // Also adds coord to `mesh_candidates` set.
    template<typename Fn>
        requires return_type_is<bool, Fn, ChunkEntry&>
    inline void mark_mesh_dirty(WorldChunkCoord coord, Fn pred){
        entries.if_contains(
            coord,
            [&](ChunkEntry& e){
                if (pred(e)){
                    assert_eq(e.state.gen, GenState::done);
                    e.target_mesh_revision++;
                    persistent_mesh_candidates.emplace(coord);
                }
            }
        );
    }

    inline void mark_mesh_dirty(WorldChunkCoord coord){
        entries.if_contains(
            coord,
            [&](ChunkEntry& e){
                if(e.state.gen != GenState::done){
                    std::println("Rejected mesh endirtying of chunk @{}, its gen state is not done!", coord);
                }else{
                    e.target_mesh_revision++;
                    persistent_mesh_candidates.emplace(coord);
                }
            }
        );
    }
    void mark_mesh_clean(WorldChunkCoord coord){
        entries.if_contains(
            coord,
            [&](ChunkEntry& e){
                assert(e.target_mesh_revision==e.loaded_mesh_revision);
                persistent_mesh_candidates.erase(coord);
            }
        );
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
    



    GenConfig genConfig;
    void make_chunk_entry(WorldChunkCoord key);
    decltype(auto) try_get_chunk_entry(WorldChunkCoord key){
        return entries.try_get(key);
    }


    inline std::vector<std::pair<bool, WorldChunkCoord>> chunksStatesInRadius(WorldChunkCoord chunkCoord, i32 dist) {
        const size_t nChunksInRadius = std::pow(2*dist+1,3);
        std::vector<std::pair<bool,WorldChunkCoord>> candidates;
        candidates.reserve(nChunksInRadius);

        auto add = [this, &candidates](i32 x, i32 y, i32 z){
            const auto key = WorldChunkCoord{x,y,z}; // dont you have to 
            const auto state = entries.try_get(key);
            candidates.emplace_back(static_cast<bool>(state),key);
        };

        const i32& oy = chunkCoord.y;

        i32 minY = oy-dist;
        i32 maxY = oy+dist;
        for (i32 y = maxY; y>=minY; y--){
            i32 x{chunkCoord.x}, z{chunkCoord.z};
            add(x,y,z); // center point
            for (i32 r = 1; r<= dist; r++){
                const i32 r2 = 2*r;
                add(--x,y,z); // move out of the centre point
                for (int j = 0; j<r2 - 1;j++)    add(x,y,++z); // traverse the remaining (-X) edge
                for (int j = 0; j<r2 ; j++)     add(++x,y,z);  // traverse the whole     (Z+) edge
                for (int j = 0; j<r2 ; j++)     add(x,y,--z);  // traverse the whole     (+X) edge
                for (int j = 0; j<r2 ; j++)     add(--x,y,z);  // traverse the whole     (+X) edge
            }
        }
        return candidates;
    }


    std::vector<std::pair<Block, Direction>> getNeighbourBlocks(WorldBlockPos world_pos) const;

private:
    void           updateNeighbourMap(WorldChunkCoord chunkCoord);
    void           updateBoundingBoxesMap(WorldChunkCoord chunkCoord);
};
