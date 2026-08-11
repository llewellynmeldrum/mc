#include "BlockAmbientOcclusion.hpp"
#include "ChunkNeighbourhood.hpp"
AONeighbours get_vtx_ao_neighbours(ChunkNeighbourhoodSnapshot<Block> const& neighbourhood, ChunkBlockPos pos, Direction dir, size_t vtx_idx){
    auto const& offsets = get_vtx_ao_neighbour_offsets(dir,vtx_idx);
    return AONeighbours{
        .corner = neighbourhood.get_or_default(pos + offsets.corner),
        .a = neighbourhood.get_or_default(pos + offsets.a),
        .b = neighbourhood.get_or_default(pos + offsets.b),
    };
}
