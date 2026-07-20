#pragma once 
#include "Types.h"
// compile time optons  (default)
#define ENABLE_CPPTRACE 


#undef ENABLE_CPPTRACE



namespace DebugOption{
    constexpr inline bool ON = true;
    constexpr inline bool OFF = false;

    inline bool enable_cutout_optimisation            = OFF;
    inline bool enable_drone_cam                = OFF;
    inline bool enable_3d_debug_visuals         = OFF;

    inline bool gen_state_mode                  = OFF; // OFF = mesh state mode
    inline bool outline_neighbour_boundaries    = OFF;
    inline bool fill_neighbour_boundaries       = OFF;
    inline bool fill_all_boundaries             = OFF;
    inline bool outline_all_boundaries          = OFF;
    inline bool show_debug_ui                   = ON;
    inline bool HIDE_AIR_CHUNKS                 = OFF;
    inline bool HIDE_CLEAN_CHUNKS               = OFF;
    inline bool enable_opaque_sorting           = OFF;

    inline bool show_noise_debug                = OFF;

    inline u8   ChunkDebugFillOpacity           = 38;
    inline f32  ChunkDebugOutlineOpacity        = 0.9f;
    inline i32  compiler_optimisation_level     = 0; 
    inline f32  player_cam_vfov = 70.0f;
} // namesapce DebugOption
