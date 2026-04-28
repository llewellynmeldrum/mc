#include "ChunkMap.hpp"
#include "Logger.hpp"

const Chunk*
ChunkMap::getData(ivec3 pos) {
    if (data.contains(pos)) {
        return &data[pos];
    } else {
        return nullptr;
    }
}

std::span<const Chunk* const, NUM_NEIGHBOURS>
ChunkMap::getNeighbours(ivec3 pos) const {
    std::span<const Chunk* const, NUM_NEIGHBOURS> res{ neighbours.at(pos) };
    return res;
}

void
ChunkMap::assignNeighbours(ivec3 pos) {
    auto* self = &data[pos];

    const ivec3 neighbour_front = pos + ivec3{ 0, 0, -1 };
    const ivec3 neighbour_back = pos + ivec3{ 0, 0, +1 };
    const ivec3 neighbour_left = pos + ivec3{ -1, 0, 0 };
    const ivec3 neighbour_right = pos + ivec3{ +1, 0, 0 };
    const ivec3 neighbour_down = pos + ivec3{ 0, -1, 0 };
    const ivec3 neighbour_up = pos + ivec3{ 0, +1, 0 };

    std::array<const Chunk*, NUM_NEIGHBOURS> neighbour_arr{
        data.contains(neighbour_front) ? &data[neighbour_front] : nullptr,
        data.contains(neighbour_back) ? &data[neighbour_back] : nullptr,
        data.contains(neighbour_left) ? &data[neighbour_left] : nullptr,
        data.contains(neighbour_right) ? &data[neighbour_right] : nullptr,
        data.contains(neighbour_down) ? &data[neighbour_down] : nullptr,
        data.contains(neighbour_up) ? &data[neighbour_up] : nullptr,
    };
    neighbours.insert({ pos, neighbour_arr });

    // also, need to update neighbours to include US now.
    if (neighbour_arr[0]) {  // if we have a front neighbour
        // front_neighbour.back_neighbour = self;
        neighbours[neighbour_front][1] = self;
    }
    if (neighbour_arr[1]) {
        // back_neighbour.front_neighbour= self;
        neighbours[neighbour_back][0] = self;
    }
    if (neighbour_arr[2]) {
        // left.right = self
        neighbours[neighbour_left][3] = self;
    }
    if (neighbour_arr[3]) {
        // right.left = self
        neighbours[neighbour_right][2] = self;
    }
    if (neighbour_arr[4]) {
        // down.up = self
        neighbours[neighbour_down][5] = self;
    }
    if (neighbour_arr[5]) {
        // up.down = self
        neighbours[neighbour_up][4] = self;
    }
}
void
ChunkMap::generate(ivec3 pos) {
    if (data.contains(pos)) {
        LOG_WARN("Chunk generation requested on chunk that already exists.");
    } else {
        data.insert({ pos, generator.gen(pos) });
        assignNeighbours(pos);
    }
}

bool
ChunkMap::isDirty(ivec3 pos) {
    auto it = is_dirty.find(pos);
    if (it != is_dirty.end()) {
        return (*it).second;
    }
    return false;  // no point in meshing a chunk if it does not exist
}

void
ChunkMap::makeDirty(ivec3 pos) {
    is_dirty.insert_or_assign(pos, true);
}
void
ChunkMap::makeClean(ivec3 pos) {
    is_dirty.insert_or_assign(pos, false);
}
