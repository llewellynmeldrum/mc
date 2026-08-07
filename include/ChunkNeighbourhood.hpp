#pragma once 

#include <print>

#include "ChunkEntryEnums.hpp"
#include "ChunkHelpers.hpp"
#include "ChunkStorage.hpp"
#include "DebugChunkLog.hpp"
#include "FormatSpecs.hpp"
#include "CopyOnWrite.hpp"

// @BRIEF:
//  A ChunkNeighbourHood can be used to seamlessly read neighbour data* of any 'block' within a center chunk,
//  *Data here can be any of: (Block, PackedLightData)
//
//  Specifically, a chunk neighbourhood is a wrapper around:
//  -> const handles to snapshots of the center chunk and each of its cardinal neighbour chunks
//  NOTE:
//  PRECONDITIONS:
//  -> Entries for the center chunk, and each of the four cardinal neighbours MUST exist.
template<typename t_DataType>
struct ChunkNeighbourhoodSnapshot{
public:
    using DataType = t_DataType;
    using Snapshot = std::shared_ptr<const GenericChunkStore<DataType>>;

    ChunkNeighbourhoodSnapshot(const ChunkEntry*            center_entry,
                               std::span<const ChunkEntry*> neighbour_entries)
        : center_coord(center_entry->coord)
        , store(center_entry->get_snapshot<DataType>())
        ,
        neighbour_stores(std::from_range, neighbour_entries | ranges::views::transform(
                [](const ChunkEntry* e){
            return e->get_snapshot<DataType>();
        }))

    { }
    ChunkNeighbourhoodSnapshot(const ChunkNeighbourhoodSnapshot&) = default;
    ChunkNeighbourhoodSnapshot(ChunkNeighbourhoodSnapshot&&) = default;
    ChunkNeighbourhoodSnapshot& operator=(const ChunkNeighbourhoodSnapshot&) = default;
    ChunkNeighbourhoodSnapshot& operator=(ChunkNeighbourhoodSnapshot&&) = default;

    WorldChunkCoord center_coord;
    Snapshot store;
    std::vector<Snapshot> neighbour_stores;

    DataType get(ChunkBlockPos pos) const{
        if (in_center(pos)){
            return store->at(pos);
        }else{
            assert(in_neighbourhood(pos)); 
            const auto& neighbour = which_neighbour_chunk_store(pos);
            auto corrected_pos = LM::euclid_mod(pos, ChunkInfo::Extents3D);
            return neighbour.at(corrected_pos);
        }
    }

    static constexpr bool in_center(ChunkBlockPos p)noexcept{
        return LM::isVecInBounds(p, ChunkBlockPos{0,0,0}, ChunkBlockPos{ChunkInfo::Extents3D});
    }

    static constexpr bool in_neighbourhood(ChunkBlockPos p) noexcept{
        // Currently, a neighbourhood represents:
        // [ ][D][ ]
        // [D][C][D]
        // [ ][D][ ]
        // Where C = center chunk, and D neighours in each cardinal direction.
        ChunkOffset neighbour_chunk_offset = toChunkOffset(p);
        if (neighbour_chunk_offset.x != 0 && neighbour_chunk_offset.x == neighbour_chunk_offset.z){
            // corner chunk, we dont store those
            return false;
        }

        if (neighbour_chunk_offset.x < -1 || neighbour_chunk_offset.x > 1){
            return false;
        }
        if (neighbour_chunk_offset.z < -1 || neighbour_chunk_offset.z > 1){
            return false;
        }
        return p.y >=0 && p.y < ChunkInfo::HEIGHT;
    }
    static_assert(in_neighbourhood(ChunkBlockPos{0,0,0}));
    static_assert(!in_neighbourhood(ChunkBlockPos{0,-1,0}));
private:
    const GenericChunkStore<DataType>& which_neighbour_chunk_store(ChunkBlockPos p) const noexcept{
        assert(!in_center(p)); 
        assert(in_neighbourhood(p)); 
        auto const neighbour_dir = get_cpos_overflow_direction(p);
        auto const neighbour_dir_idx = std::to_underlying(neighbour_dir);
        return *(neighbour_stores[neighbour_dir_idx]);
    }
};

using LightNeighbourhoodSnapshot = ChunkNeighbourhoodSnapshot<PackedLightValue>;
using BlockNeighbourhoodSnapshot = ChunkNeighbourhoodSnapshot<Block>;

