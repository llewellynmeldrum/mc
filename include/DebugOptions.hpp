#pragma once 
#include "CommonUtils.hpp"
#include "KeyCodes.hpp"
#include "Types.h"
// compile time optons  (default)
#define ENABLE_CPPTRACE 


//#undef ENABLE_CPPTRACE



template<typename T>
struct DebugVal : Bounded<T>{
    using value_type = T;
    const char* name;
    const char* desc;
};
struct DebugOpt {
    using value_type = bool;
    bool val;
    decltype(auto) operator=(const bool& b){this->val = b;}
    const char* name;
    const char* desc;
    constexpr operator bool()const noexcept{return val;}
};
#define DECL_OPTION(T, name, desc, ...) inline DebugVal<T> name { __VA_ARGS__ __VA_OPT__(,) #name, desc}
#define DECL_TOGGLE(T, name, desc, val) inline DebugOpt name { val, #name, desc} 
namespace DebugOption{
    constexpr inline bool ON = true;
    constexpr inline bool OFF = false;
    enum struct DebugRenderStateTarget{
        MESH,
        GEN,
        LIGHTING,
    };
    inline const std::vector<char const*> DebugRenderStateTarget_names
    {
        "MESH",
        "GEN",
        "LIGHTING",
	};


    inline DebugRenderStateTarget render_state_mode                  = DebugRenderStateTarget::GEN; // OFF = mesh state mode
    DECL_TOGGLE(bool,show_lighting_system,"enable the lighting system in the frag shader",ON) ;
    DECL_TOGGLE(bool,draw_blocklight,"enable the blocklight in frag shader",ON) ;
    DECL_TOGGLE(bool,draw_sunlight,"enable sunlight in the frag shader",ON) ;
    DECL_TOGGLE(bool,enable_cutout_optimisation,"dynamically disables cutout rendering for far away chunks",OFF) ;
    DECL_TOGGLE(bool,enable_drone_cam,"Enable the renderering of the drone cam overhead",OFF) ;
    DECL_TOGGLE(bool,enable_3d_debug_visuals,"Global toggle for all 3d debug rendering",ON) ;
    DECL_TOGGLE(bool,outline_neighbour_boundaries,"Toggle the outline for chunk boundaries of the 4 neighbour chunks",OFF) ;
    DECL_TOGGLE(bool,fill_neighbour_boundaries,"Toggle the filling of chunk boundaries of the 4 neighbour chunks",OFF) ;
    DECL_TOGGLE(bool,outline_all_boundaries,"Toggle the outline for all chunk boundaries",OFF) ;
    DECL_TOGGLE(bool,fill_all_boundaries,"Toggle the filling of all chunk boundaries", OFF);
    DECL_TOGGLE(bool,show_debug_ui, "Toggle this ui", ON) ;
    DECL_TOGGLE(bool,skybox_ui_override, "Override the default skybox params.", ON) ;
    DECL_TOGGLE(bool,HIDE_AIR_CHUNKS,"Exclude chunks consisting only of air from 3d debug rendering",OFF) ;
    DECL_TOGGLE(bool,HIDE_CLEAN_CHUNKS,"Exclude chunks at the 'clean' pipeline stage, of the selected pipeline stage from 3d debug rendering",OFF) ;
    DECL_TOGGLE(bool,enable_opaque_sorting,"Enable furthest first sorting of opaque chunks",OFF) ;


    inline u8   ChunkDebugFillOpacity           = 38;
    inline f32  ChunkDebugOutlineOpacity        = 0.9f;
    inline i32  compiler_optimisation_level     = 0; 
    DECL_OPTION(f32, player_cam_vfov, "Player vertical fov",  {70.0f, 0.0f, 180.0f});
    DECL_TOGGLE(bool, _name, "Player vertical fov", ON);
    
} // namesapce DebugOption
