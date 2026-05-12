
#include "DebugFormatSpecializations.hpp"
#include "ChunkMap.hpp"
#include "CommonUtils.hpp"
#include "Logger.hpp"
#include <memory>

std::array<const Chunk*, NUM_NEIGHBOURS> ChunkMap::getSurroundingChunks(ivec3 pos) const {
    std::array<const Chunk*, NUM_NEIGHBOURS> res{ neighbours.at(pos) };
    return res;
}
const ChunkMetadata* ChunkMap::getMetadata(ivec3 pos)const {
    return metadata.at(pos).get();
}
const AABB*   ChunkMap::getBoundingBox(ivec3 chunk_offset) const{
    return boundingBoxes.at(chunk_offset).get();
}

ChunkStatus ChunkMap::getChunkState(ivec3 chunk_offset) const{
    return chunkState.at(chunk_offset);
}
// assign our neighbours if they exist in the chunkmap,
// also add ourselves  to our neighbours neighbourlist.
void ChunkMap::updateNeighbourMap(ivec3 pos) {
    auto* self_ptr = data[pos].get();

    std::array<const Chunk*, NUM_NEIGHBOURS> my_neighbours{};
    for (std::size_t dir_idx = 0; dir_idx < my_neighbours.size(); dir_idx++) {
        const ivec3 neighbour_chunk_pos = pos + Direction_offset[dir_idx];
        if (data.contains(neighbour_chunk_pos)) {
            // assign NEIGHBOUR to ourNeighbours.dir
            const auto* neighbour_chunk = data[neighbour_chunk_pos].get();
            if (!neighbour_chunk)
                continue;
            my_neighbours[dir_idx] = neighbour_chunk;

            // assign SELF to ourNeighbours.dir.inverseDir
            const auto inverseDir_idx = inverseDirection_n.at(dir_idx);
            neighbours[neighbour_chunk_pos][inverseDir_idx] = self_ptr;
        }
    }
    neighbours.emplace(pos, std::move(my_neighbours));
}

void ChunkMap::updateBoundingBoxesMap(ivec3 chunk_coord) {
    const ivec3 min = chunk_coord * Chunk::Extents;
    const ivec3 max = min + Chunk::Extents;
    boundingBoxes.emplace(chunk_coord, std::make_unique<AABB>(min, max));
}

void ChunkMap::generate(ivec3 chunk_coord) {
    //    LOG_DEBUG("Generating chunk for {}", dbg_fmt(chunk_coord));
    if (data.contains(chunk_coord)) {
        LOG_WARN("Chunk generation requested on chunk that already exists.");
    } else {
        auto [chunk_data, chunk_metadata] = generator.gen(chunk_coord);
        data.emplace(chunk_coord, std::make_unique<Chunk>(chunk_data));
        metadata.emplace(chunk_coord, std::make_unique<ChunkMetadata>(chunk_metadata));

        updateNeighbourMap(chunk_coord);
        updateBoundingBoxesMap(chunk_coord);
        chunkState.emplace(chunk_coord, ChunkStatus::DIRTY);
    }
}

bool ChunkMap::isDirty(ivec3 pos) const {
    return chunkState.at(pos)==ChunkStatus::DIRTY;
}
bool ChunkMap::isClean(ivec3 pos) const {
    return chunkState.at(pos)==ChunkStatus::CLEAN;
}
bool ChunkMap::isMeshing(ivec3 pos) const {
    return chunkState.at(pos)==ChunkStatus::CURRENTLY_MESHING;
}

void ChunkMap::markDirty(ivec3 pos) {
    chunkState.insert_or_assign(pos,ChunkStatus::DIRTY);
}
void ChunkMap::markClean(ivec3 pos) {
    chunkState.insert_or_assign(pos,ChunkStatus::CLEAN);
}
void ChunkMap::markMeshing(ivec3 pos) {
    chunkState.insert_or_assign(pos,ChunkStatus::CURRENTLY_MESHING);
}
