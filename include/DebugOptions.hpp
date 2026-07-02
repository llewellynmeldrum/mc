#pragma once 
#include "Types.h"
namespace DebugOption{
    constexpr inline bool ON = true;
    constexpr inline bool OFF = false;

    inline bool showGenState                = ON;
    inline bool showNeighbours              = OFF;
    inline bool fill_chunk_boundaries       = ON;
    inline bool showDebugUI                 = ON;
    inline bool HIDE_AIR_CHUNKS             = OFF;
    inline bool HIDE_CLEAN_CHUNKS           = OFF;
    inline u8   ChunkDebugFillOpacity       = 8;
    inline f32  ChunkDebugOutlineOpacity    = 0.9f;
    inline i32  compiler_optimisation_level = 0; 
} // namesapce DebugOption
