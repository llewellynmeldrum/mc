#ifdef __cplusplus // SECTION: c++ specific versions of stuff
    #pragma once 
#endif 

#define TEXTURE_ID_COUNT 4
#define ENABLE_FAKE_SHADOWS 
#define DIR_COUNT 6

#ifdef __cplusplus 
#define LAYOUT_CONST constexpr
#define INLINE(T) inline T
// SECTION: c++ specific versions of stuff

#define START_DECL_ARRAY(T, N, name) inline constexpr std::array<T,N> name{                   
#define END_DECL_ARRAY() };
#else 

// SECTION: glsl specific versions of stuff
#define LAYOUT_CONST const 
#define INLINE(T) T
#define START_DECL_ARRAY(T, N, name)    T name[N] = T[N](
#define END_DECL_ARRAY() );
#endif


START_DECL_ARRAY(float, DIR_COUNT, faceShadowOpacity)
    0.15, // FORWARD,
    0.45, // BACKWARD
    0.35, // LEFT,   
    0.35, // RIGHT,//
    0.7, // DOWN, //
    0.0 // UP,     
END_DECL_ARRAY()

LAYOUT_CONST INLINE(uint) sunlight_intensity_offset        =  0u;
LAYOUT_CONST INLINE(uint) blocklight_r_offset              =  4u;
LAYOUT_CONST INLINE(uint) blocklight_g_offset              =  8u;
LAYOUT_CONST INLINE(uint) blocklight_b_offset              = 12u;
LAYOUT_CONST INLINE(uint) face_dir_offset                  = 16u;
LAYOUT_CONST INLINE(uint) tex_atlas_id_offset              = 19u;
LAYOUT_CONST INLINE(uint) face_opacity_offset              = 24u;

LAYOUT_CONST INLINE(uint) sunlight_intensity_mask = 0x0000000Fu;  //00000000000000000000000000001111; // set in mesher
LAYOUT_CONST INLINE(uint) blocklight_r_mask       = 0x000000F0u;  //00000000000000000000000011110000; // set in mesher
LAYOUT_CONST INLINE(uint) blocklight_g_mask       = 0x00000F00u;  //00000000000000000000111100000000; // set in mesher
LAYOUT_CONST INLINE(uint) blocklight_b_mask       = 0x0000F000u;  //00000000000000001111000000000000; // set in mesher
LAYOUT_CONST INLINE(uint) face_dir_mask           = 0x00070000u;  //00000000000001110000000000000000; // set in ctor
LAYOUT_CONST INLINE(uint) tex_atlas_id_mask       = 0x00380000u;  //00000000001110000000000000000000; // set in ctor
LAYOUT_CONST INLINE(uint) face_opacity_mask       = 0xFF000000u;  //11111111000000000000000000000000; // set in mesher



#define get_val(target, mask, offset) ((target & mask) >> offset)
#define set_val(target, mask, offset, val) target = ((target & ~mask)|((val<<offset) & mask))
