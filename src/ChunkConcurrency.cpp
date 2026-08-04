#include "ChunkConcurrency.hpp"
#include "ChunkMap.hpp"
#include "TextureAtlas.hpp"
#include "ChunkNoiseDebug.hpp"
#include "ChunkMap.hpp"

MeshJob::MeshJob(
        ChunkBenchContext _bench,
        WorldChunkCoord key, 
        const_span<TextureAtlas*> _atlas_list, 
        const ChunkMap* chunk_map,
        const ChunkEntry* entry
    ):
        bench(_bench), 
        coord(key),
        rev(entry->mesh.get_candidate_rev()),
        blocks(entry->block_data.clone()),
        light_data(entry->light_data.clone()),
        atlas_map(_atlas_list)

{
    surrounding_chunks_block_slices = chunk_map->populate_neighbour_slices<Block>(entry);
    surrounding_chunks_light_slices = chunk_map->populate_neighbour_slices<PackedLightValue>(entry);
}
LightingJob::LightingJob(
        ChunkBenchContext _bench,
        WorldChunkCoord _coord, 
        ChunkMap const* chunk_map,
        ChunkEntry const* entry
)
    :bench(_bench)
    ,coord(_coord)
    ,rev(entry->lighting.get_candidate_rev())
    ,light_data(entry->light_data.clone())
    ,block_data(entry->block_data.clone())
{
    neighbour_light_slices = chunk_map->populate_neighbour_slices<PackedLightValue>(entry);
    neighbour_block_slices = chunk_map->populate_neighbour_slices<Block>(entry);
}
