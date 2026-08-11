#pragma once 
#include "Block.hpp"
#include "CoordTypes.hpp"
#include "Direction.hpp"
#include "cppslop.hpp"
#include "AONeighbours.hpp"
// An AO neighbour (Ambient occlusion neighbour) is a set of Any given direction contains 
struct AONeighbourOffsets{
    BlockOffset corner;
    BlockOffset a;
    BlockOffset b;
};
// each direction + vertex combo has 
#define N -1
#define P +1
#define Z  0
constexpr auto direction_to_ao_neighbour_offsets = EnumMap<Direction,std::array<AONeighbourOffsets,4>>{
    {
        Direction::NORTH,
        {
            // double checked, all korrect
            AONeighbourOffsets{ {P,N,N,}, {P,Z,N,}, {Z,N,N,},  },
            AONeighbourOffsets{ {N,N,N,}, {Z,N,N,}, {N,Z,N,},  },
            AONeighbourOffsets{ {N,P,N,}, {N,Z,N,}, {Z,P,N,},  },
            AONeighbourOffsets{ {P,P,N,}, {Z,P,N,}, {P,Z,N,},  },
        }
    },
    {
        Direction::SOUTH,
        {
            // 
            AONeighbourOffsets { {N,N,P,}, {N,Z,P,}, {Z,N,P,} },
            AONeighbourOffsets { {P,N,P,}, {Z,N,P,}, {P,Z,P,} },
            AONeighbourOffsets { {P,P,P,}, {P,Z,P,}, {Z,P,P,} },
            AONeighbourOffsets { {N,P,P,}, {Z,P,P,}, {N,Z,P,} },
        }
    },
    {
        Direction::WEST,
        {
            AONeighbourOffsets { {N,N,N,}, {N,Z,N,}, {N,N,Z,} },
            AONeighbourOffsets { {N,N,P,}, {N,N,Z,}, {N,Z,P,} },
            AONeighbourOffsets { {N,P,P,}, {N,Z,P,}, {N,P,Z,} },
            AONeighbourOffsets { {N,P,N,}, {N,P,Z,}, {N,Z,N,} },
        }
    },

    {
        Direction::EAST,
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

template<typename t_DataType> 
FORWARD_DECL_STRUCT(ChunkNeighbourhoodSnapshot)

AONeighbours get_vtx_ao_neighbours(ChunkNeighbourhoodSnapshot<Block> const& neighbourhood, ChunkBlockPos pos, Direction dir, size_t vtx_idx);
// It would be nice to have block neighbourhood style semantics in the mesher. Copy on write would also enable that  a bit easier

#undef N
#undef P
#undef Z
