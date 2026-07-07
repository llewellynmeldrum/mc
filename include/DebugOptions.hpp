#pragma once 
#include "Types.h"
namespace DebugOption{
    constexpr inline bool ON = true;
    constexpr inline bool OFF = false;

    inline bool gen_state_mode                  = ON; // OFF = mesh state mode
    inline bool outline_neighbour_boundaries    = ON;
    inline bool fill_neighbour_boundaries       = ON;
    inline bool fill_all_boundaries             = OFF;
    inline bool outline_all_boundaries          = OFF;
    inline bool show_debug_ui                     = ON;
    inline bool HIDE_AIR_CHUNKS                 = OFF;
    inline bool HIDE_CLEAN_CHUNKS               = OFF;

    inline u8   ChunkDebugFillOpacity           = 38;
    inline f32  ChunkDebugOutlineOpacity        = 0.9f;
    inline i32  compiler_optimisation_level     = 0; 
} // namesapce DebugOption
