#pragma once 
#include <functional>
#include <memory>
#include <optional>
#include <print>
#include <queue>
#include <type_traits>

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
    template<typename DataType>
    std::vector<GenericChunkSlice<DataType>> populate_neighbour_slices(ChunkEntry const* entry) const noexcept{
        assert(entry);
        std::vector<GenericChunkSlice<DataType>> slices;
        for (const auto& [dir_idx, neighbour_coord]: entry->each_dir_neighbour_chunk_coords()){
            ChunkBlockPos p0{}, p1{};
            constexpr auto XE = ChunkInfo::XWIDTH;
            constexpr auto YE = ChunkInfo::HEIGHT;
            constexpr auto ZE = ChunkInfo::ZWIDTH;
            SliceType slice_type = {};
            switch (static_cast<Direction>(dir_idx)){
                // -Z
                case Direction ::BACKWARD: p0 = {0,0,0}; p1 ={XE,YE,1}; slice_type = SliceType::Z; break;
                // +Z
                case Direction ::FORWARD: p0 = {0,0,ZE-1}; p1 ={XE,YE,ZE}; slice_type = SliceType::Z; break;
                // -X
                case Direction ::RIGHT: p0 = {0,0,0}; p1 ={1,YE,ZE}; slice_type = SliceType::X; break;
                // +X
                case Direction ::LEFT   : p0 = {XE-1,0,0}; p1 ={XE,YE,ZE}; slice_type = SliceType::X; break;

                default: BREAKPOINT(); break;
            }
            const auto* neighbour  = entries.try_get(neighbour_coord);
            if (neighbour){
                if constexpr (std::same_as<DataType,Block>){
                    slices.emplace_back(
                        neighbour->block_data.view(),
                        slice_type,
                        p0,
                        p1
                    );
                }else if constexpr(std::same_as<DataType,PackedLightValue>){
                    slices.emplace_back(
                        neighbour->light_data.view(),
                        slice_type,
                        p0,
                        p1
                    );
                }
            }else{
                slices.emplace_back(nullptr);
            }
        }
        assert(slices.size()==N_NEIGHBOURS);
        return slices;
}

};
