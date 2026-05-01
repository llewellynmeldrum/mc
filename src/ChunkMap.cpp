#include "ChunkMap.hpp"
#include "Logger.hpp"

std::span<const Chunk* const, NUM_NEIGHBOURS> ChunkMap::getNeighbours(ivec3 pos) const {
    std::span<const Chunk* const, NUM_NEIGHBOURS> res{ neighbours.at(pos) };
    return res;
}

void ChunkMap::updateNeighbourMap(ivec3 pos) {
    auto* self = &data[pos];

    // 1. Find and set any of my neighbours if they exist
    // clang-format off
    const ivec3 front = pos +   ivec3{ 0, 0, -1 };
    const ivec3 back = pos +    ivec3{ 0, 0, +1 };
    const ivec3 left = pos +    ivec3{ -1, 0, 0 };
    const ivec3 right = pos +   ivec3{ +1, 0, 0 };
    const ivec3 down = pos +    ivec3{ 0, -1, 0 };
    const ivec3 up = pos +      ivec3{ 0, +1, 0 };

    std::array<const Chunk*, NUM_NEIGHBOURS> my_neighbours{
        data.contains(front) ?   data[front].get()  : nullptr,
        data.contains(back)  ?   data[back].get()   : nullptr,
        data.contains(left)  ?   data[left].get()   : nullptr,
        data.contains(right) ?   data[right].get()  : nullptr,
        data.contains(down)  ?   data[down].get()   : nullptr,
        data.contains(up)    ?   data[up].get()     : nullptr,
        // clang-format on
    };
    neighbours.emplace(pos, my_neighbours);

    // 2. Update my neighbours neighbour list to include ME.
    if (my_neighbours[0]) {
        // front_neighbour.back_neighbour = self;
        neighbours[front][1] = self->get();
    }
    if (my_neighbours[1]) {
        // back_neighbour.front_neighbour= self;
        neighbours[back][0] = self->get();
    }
    if (my_neighbours[2]) {
        // left.right = self
        neighbours[left][3] = self->get();
    }
    if (my_neighbours[3]) {
        // right.left = self
        neighbours[right][2] = self->get();
    }
    if (my_neighbours[4]) {
        // down.up = self
        neighbours[down][5] = self->get();
    }
    if (my_neighbours[5]) {
        // up.down = self
        neighbours[up][4] = self->get();
    }
}
void ChunkMap::generate(ivec3 chunk_coord) {
    if (data.contains(chunk_coord)) {
        LOG_WARN("Chunk generation requested on chunk that already exists.");
    } else {
        data.emplace(chunk_coord, generator.gen(chunk_coord));
        updateNeighbourMap(chunk_coord);
    }
}

bool ChunkMap::isDirty(ivec3 pos) const {
    auto it = is_dirty.find(pos);
    return it != is_dirty.end() ? (*it).second : false;
}

void ChunkMap::makeDirty(ivec3 pos) {
    is_dirty.insert_or_assign(pos, true);
}
void ChunkMap::makeClean(ivec3 pos) {
    is_dirty.insert_or_assign(pos, false);
}
