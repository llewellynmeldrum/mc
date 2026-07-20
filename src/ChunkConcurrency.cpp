#include "ChunkConcurrency.hpp"
#include "ChunkMap.hpp"
#include "TextureAtlas.hpp"
#include "ChunkNoiseDebug.hpp"
#include <ranges>

MeshJob::MeshJob(
        WorldChunkCoord key, 
        const_span<TextureAtlas*> _atlas_list, 
        ChunkMap* chunk_map,
        const ChunkEntry* entry
    ):
        meshRevisionID(entry->target_mesh_revision),
        chunkCoord(key),
        blocks(entry->block_data.clone()),
        atlas_map(_atlas_list)
{
#ifdef CHUNK_NOISE_DEBUG
    std::ranges::copy(entry->noise.buffer(), this->noise.buffer().begin());
#endif 
    for (const auto& [dir, dir_idx]: eachDirIndex2D){
        const auto& neighbour_coord = entry->neighbours[dir_idx];
        if (!neighbour_coord){
            surroundingChunks.emplace_back(std::nullopt);
        }else{
            ChunkBlockPos p0{}, p1{};
            constexpr auto XE = ChunkInfo::XWIDTH;
            constexpr auto YE = ChunkInfo::HEIGHT;
            constexpr auto ZE = ChunkInfo::ZWIDTH;
            SliceType slice_type = {};
            switch (dir){
                // -Z
                case Direction ::BACKWARD: p0 = {0,0,0}; p1 ={XE,YE,1}; slice_type = SliceType::Z; break;
                // +Z
                case Direction ::FORWARD: p0 = {0,0,ZE-1}; p1 ={XE,YE,ZE}; slice_type = SliceType::Z; break;

                // -X
                case Direction ::RIGHT: p0 = {0,0,0}; p1 ={1,YE,ZE}; slice_type = SliceType::X; break;
                // +X
                case Direction ::LEFT   : p0 = {XE-1,0,0}; p1 ={XE,YE,ZE}; slice_type = SliceType::X; break;

                default:
                    break;
            }
            chunk_map->entries.if_contains_else(
                neighbour_coord.value(),
                [&](ChunkEntry& neighbour){
                    surroundingChunks.emplace_back(
                        std::in_place,
                        neighbour.block_data.view(),
                        slice_type,
                        p0,p1);
                },
                [&](){
                    surroundingChunks.emplace_back(std::nullopt);
                }
            );
        }
    }
    assert(surroundingChunks.size()==N_NEIGHBOURS);
}
