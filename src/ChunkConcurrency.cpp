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
        blocks(chunk_map->take_neighbourhood_snapshot<Block>(coord)),
        lights(chunk_map->take_neighbourhood_snapshot<PackedLightValue>(coord)),
        atlas_map(_atlas_list)

{}
LightingJob::LightingJob(
        ChunkBenchContext _bench,
        WorldChunkCoord _coord, 
        ChunkMap const* chunk_map,
        ChunkEntry const* entry
)
    :bench(_bench)
    ,coord(_coord)
    ,rev(entry->lighting.get_candidate_rev())
    ,blocks(chunk_map->take_neighbourhood_snapshot<Block>(coord))
    ,lights(chunk_map->take_neighbourhood_snapshot<PackedLightValue>(coord))
{}
