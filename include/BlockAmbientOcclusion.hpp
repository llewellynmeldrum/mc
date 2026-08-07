#pragma once 
#include "Direction.hpp"
#include "ChunkMesher_RawData.hpp"
#include "ChunkNeighbourhood.hpp"
// An AO neighbour (Ambient occlusion neighbour) is a set of Any given direction contains 
struct AONeighbourOffsets{
    BlockOffset corner;
    BlockOffset a;
    BlockOffset b;
};
struct AONeighbours{
    Block corner;
    Block a;
    Block b;
};
// each direction + vertex combo has 
#define N -1
#define P +1
#define Z  0
constexpr auto direction_to_ao_neighbour_offsets = EnumMap<Direction,std::array<AONeighbourOffsets,4>>{
    {
        Direction::FORWARD,
        {
            AONeighbourOffsets{ {P,N,N,}, {P,Z,N,}, {Z,N,N,},  },
            AONeighbourOffsets{ {N,P,N,}, {Z,N,N,}, {N,Z,N,},  },
            AONeighbourOffsets{ {N,P,N,}, {N,Z,N,}, {Z,P,N,},  },
            AONeighbourOffsets{ {P,P,N,}, {Z,P,N,}, {P,Z,N,},  },
        }
    },
    {
        Direction::BACKWARD,
        {
            AONeighbourOffsets { {N,N,P,}, {N,Z,P,}, {Z,N,P,} },
            AONeighbourOffsets { {P,N,P,}, {Z,N,P,}, {P,Z,P,} },
            AONeighbourOffsets { {P,P,P,}, {P,Z,P,}, {Z,P,P,} },
            AONeighbourOffsets { {N,P,P,}, {Z,P,P,}, {N,Z,P,} },
        }
    },
    {
        Direction::LEFT,
        {
            AONeighbourOffsets { {N,N,N,}, {P,Z,N,}, {P,N,Z,} },
            AONeighbourOffsets { {N,N,P,}, {P,N,Z,}, {P,Z,P,} },
            AONeighbourOffsets { {N,P,P,}, {P,Z,P,}, {P,P,Z,} },
            AONeighbourOffsets { {N,P,N,}, {P,P,Z,}, {P,Z,N,} },
        }
    },

    {
        Direction::RIGHT,
        {
            AONeighbourOffsets { {P,N,P,}, {P,Z,P,}, {P,N,Z,} },
            AONeighbourOffsets { {P,N,N,}, {P,N,Z,}, {P,Z,N,} },
            AONeighbourOffsets { {P,P,N,}, {P,Z,N,}, {P,P,Z,} },
            AONeighbourOffsets { {P,P,P,}, {P,P,Z,}, {P,Z,P,} },
        }
    },
    {
        Direction::DOWN,
    {
            AONeighbourOffsets { {N,N,N,}, {N,N,Z,}, {Z,N,N,} },
            AONeighbourOffsets { {P,N,N,}, {Z,N,N,}, {P,N,Z,} },
            AONeighbourOffsets { {P,N,P,}, {P,N,Z,}, {Z,N,P,} },
            AONeighbourOffsets { {N,N,P,}, {Z,N,P,}, {N,N,Z,} },
        }
    },
    {
        Direction::UP,
        {
            AONeighbourOffsets { {N,P,P,}, {N,P,Z,}, {Z,P,P,} },
            AONeighbourOffsets { {P,P,P,}, {Z,P,P,}, {P,P,Z,} },
            AONeighbourOffsets { {P,P,N,}, {P,P,Z,}, {Z,P,N,} },
            AONeighbourOffsets { {N,P,N,}, {Z,P,N,}, {N,P,Z,} },
        },
    },
};
inline constexpr AONeighbourOffsets get_vtx_ao_neighbour_offsets(Direction dir, size_t vtx_idx){
    return direction_to_ao_neighbour_offsets.at(dir).at(vtx_idx);
}
// TODO: add chunk neigbhourhood shit to vtx and implement AO :w
// // TODO: add chunk neigbhourhood shit to vtx and implement AO :w
inline constexpr AONeighbours get_vtx_ao_neighbours(ChunkNeighbourhood const& neighbourhood, ChunkBlockPos pos, Direction dir, size_t vtx_idx){
    auto const& offsets = get_vtx_ao_neighbour_offsets(dir,vtx_idx);
    return AONeighbours{
        .corner = neighbourhood.block_at(pos + offsets.corner),
        .a = neighbourhood.block_at(pos + offsets.a),
        .b = neighbourhood.block_at(pos + offsets.b),
    };
}
// It would be nice to have block neighbourhood style semantics in the mesher. Copy on write would also enable that  a bit easier

#undef N
#undef P
#undef Z
