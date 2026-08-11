#pragma once 
#include "CommonUtils.hpp"
#include "KeyCodes.hpp"
#include "Types.h"
#include <string>
#include <string_view>
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
    std::string name;
    std::string desc;
    constexpr operator bool()const noexcept{return val;}
    constexpr std::string get_uniform_name()const noexcept{
        return "u_" + name;
    }
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
    DECL_TOGGLE(bool,enable_lighting,"enable the lighting system in the frag shader",ON) ;
    DECL_TOGGLE(bool,enable_blocklight,"enable the blocklight in frag shader",ON) ;
    DECL_TOGGLE(bool,enable_fog,"enable depth fog",ON) ;
    DECL_TOGGLE(bool,fade_in_chunks,"enable depth fog",ON) ;
    DECL_TOGGLE(bool,enable_sunlight,"enable sunlight in the frag shader",ON) ;
    DECL_TOGGLE(bool,enable_skybox,"enable skybox",OFF) ;
    DECL_TOGGLE(bool,enable_cutout_optimisation,"dynamically disables cutout rendering for far away chunks",OFF) ;
    DECL_TOGGLE(bool,enable_drone_cam,"Enable the renderering of the drone cam overhead",OFF) ;
    DECL_TOGGLE(bool,enable_3d_debug_visuals,"Global toggle for all 3d debug rendering",ON) ;
    DECL_TOGGLE(bool,outline_neighbour_boundaries,"Toggle the outline for chunk boundaries of the 4 neighbour chunks",OFF) ;
    DECL_TOGGLE(bool,fill_neighbour_boundaries,"Toggle the filling of chunk boundaries of the 4 neighbour chunks",OFF) ;
    DECL_TOGGLE(bool,outline_all_boundaries,"Toggle the outline for all chunk boundaries",OFF) ;
    DECL_TOGGLE(bool,fill_all_boundaries,"Toggle the filling of all chunk boundaries", OFF);
    DECL_TOGGLE(bool,show_debug_ui, "Toggle this ui", ON) ;
    DECL_TOGGLE(bool,skybox_ui_override, "Override the default skybox params.", ON) ;
    DECL_TOGGLE(bool,hide_air_chunks,"Exclude chunks consisting only of air from 3d debug rendering",OFF) ;
    DECL_TOGGLE(bool,hide_clean_chunks,"Exclude chunks at the 'clean' pipeline stage, of the selected pipeline stage from 3d debug rendering",OFF) ;
    DECL_TOGGLE(bool,enable_opaque_sorting,"Enable furthest first sorting of opaque chunks",OFF) ;
    DECL_TOGGLE(bool,enable_block_ambient_occlusion,"Enable occlusion estimated by vertex neighbour blocks",ON) ;
    DECL_TOGGLE(bool,enable_fake_shadows,"Darken the north, south, and bottom faces of every block.",ON) ;


    inline u8   ChunkDebugFillOpacity           = 38;
    inline f32  ChunkDebugOutlineOpacity        = 0.9f;
    inline i32  compiler_optimisation_level     = 0; 
    DECL_OPTION(f32, player_cam_vfov, "Player fov (vertical)",  {70.0f, 0.0f, 180.0f});
    DECL_OPTION(f32, player_cam_vfov_zoom, "Player zoom fov (vertical)",  {30.0f, 0.0f, 180.0f});
    DECL_TOGGLE(bool, _name, "Player vertical fov", ON);
    
} // namesapce DebugOption
