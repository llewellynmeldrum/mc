#ifdef __cplusplus 
    #pragma once 
    #define cpp
    #include "Assertion.hpp"
    #include "Types.h"
#else 
    #define glsl
#endif

#if defined(glsl) 
#define DECL_CONST const
    #define START_DECL_ARRAY(T, N, name) const T name[] = T[](
    #define END_DECL_ARRAY() );
    #define DECL_CONST const
    #define DECL_FN
    #define u32 uint
    #define i32 int

    #define f32 float
    #define f64 double
#elif defined(cpp)
    #define START_DECL_ARRAY(T, N, name) inline constexpr std::array<T,N> name{
    #define END_DECL_ARRAY() };
    #define DECL_CONST constexpr inline
    #define DECL_FN constexpr inline
#endif 

#define TEX_ATLAS_COUNT 4u
#define ENABLE_FAKE_SHADOWS 
#define DIR_COUNT 6u

START_DECL_ARRAY(float, DIR_COUNT, fake_shadow_multiplier_per_face)
    0.80f, // FORWARD,
    0.80f, // BACKWARD
    0.60f, // LEFT,   
    0.60f, // RIGHT,//
    0.50f, // DOWN, //
    1.00f // UP,     
END_DECL_ARRAY()

// SECTION: SHARED (glsl and c++)

#ifdef __cplusplus
DECL_CONST uint SUNLIGHT_INTENSITY_OFFSET        =  0u;
DECL_CONST uint BLOCKLIGHT_R_OFFSET              =  4u;
DECL_CONST uint BLOCKLIGHT_G_OFFSET              =  8u;
DECL_CONST uint BLOCKLIGHT_B_OFFSET              = 12u;
DECL_CONST uint FACE_DIR_OFFSET                  = 16u;
DECL_CONST uint TEX_ATLAS_ID_OFFSET              = 19u;
DECL_CONST uint FACE_OPACITY_OFFSET              = 24u;
DECL_CONST uint AO_STATE_OFFSET                  = 30u;  //11000000000000000000000000000000; // set in mesher

DECL_CONST uint SUNLIGHT_INTENSITY_MASK = 0x0000000FU;  //00000000000000000000000000001111; // set in mesher
DECL_CONST uint BLOCKLIGHT_R_MASK       = 0x000000F0U;  //00000000000000000000000011110000; // set in mesher
DECL_CONST uint BLOCKLIGHT_G_MASK       = 0x00000F00U;  //00000000000000000000111100000000; // set in mesher
DECL_CONST uint BLOCKLIGHT_B_MASK       = 0x0000F000U;  //00000000000000001111000000000000; // set in mesher
DECL_CONST uint FACE_DIR_MASK           = 0x00070000U;  //00000000000001110000000000000000; // set in ctor
DECL_CONST uint TEX_ATLAS_ID_MASK       = 0x00380000U;  //00000000001110000000000000000000; // set in ctor
DECL_CONST uint FACE_OPACITY_MASK       = 0x3F000000U;  //00111111000000000000000000000000; // set in mesher
DECL_CONST uint AO_STATE_MASK           = 0xC0000000U;  //11000000000000000000000000000000; // set in mesher

DECL_CONST uint BLOCKLIGHT_OFFSET              =  BLOCKLIGHT_R_OFFSET;
DECL_CONST uint BLOCKLIGHT_MASK       = BLOCKLIGHT_R_MASK | BLOCKLIGHT_G_MASK | BLOCKLIGHT_B_MASK;  //00000000000000000000000011110000; // set in mesher

#define POW2(x) (2u<<(x-1u))
#define UINT_RANGE(bitcount) (POW2(bitcount)-1u)
DECL_CONST uint SUNLIGHT_INTENSITY_MAX = UINT_RANGE(4u);
DECL_CONST uint BLOCKLIGHT_R_MAX       = UINT_RANGE(4u);
DECL_CONST uint BLOCKLIGHT_G_MAX       = UINT_RANGE(4u);
DECL_CONST uint BLOCKLIGHT_B_MAX       = UINT_RANGE(4u);
DECL_CONST uint FACE_OPACITY_MAX       = UINT_RANGE(6u);
DECL_CONST uint TEX_ATLAS_ID_MAX       = UINT_RANGE(3u);
DECL_CONST uint FACE_DIR_MAX           = DIR_COUNT;
DECL_CONST uint AO_STATE_MAX           = UINT_RANGE(2u);

//constexpr inline void set32_sunlight_intensity(u32& p, u8 val) {
//    SET_BITFIELD_MEMBER_VAL(p, SUNLIGHT_INTENSITY_MASK,SUNLIGHT_INTENSITY_OFFSET,val);
//}
//constexpr inline void set32_blocklight_r(u32& p, u8 val) {
//    SET_BITFIELD_MEMBER_VAL(p, BLOCKLIGHT_R_MASK,BLOCKLIGHT_R_OFFSET,val);
//}
//constexpr inline void set32_blocklight_g(u32& p, u8 val) {
//    SET_BITFIELD_MEMBER_VAL(p, BLOCKLIGHT_G_MASK,BLOCKLIGHT_G_OFFSET,val);
//}
//constexpr inline void set32_blocklight_b(u32& p, u8 val) {
//    SET_BITFIELD_MEMBER_VAL(p, BLOCKLIGHT_B_MASK,BLOCKLIGHT_B_OFFSET,val);
//}
//constexpr inline void set32_face_opacity(u32& p, u8 val) {
//    assert_leq(val, FACE_OPACITY_MAX);
//    SET_BITFIELD_MEMBER_VAL(p, FACE_OPACITY_MASK,FACE_OPACITY_OFFSET,val);
//}
//constexpr inline void set32_tex_atlas_id(u32& p, u8 val) {
//    assert_leq(val, TEX_ATLAS_ID_MAX);
//    SET_BITFIELD_MEMBER_VAL(p, TEX_ATLAS_ID_MASK,TEX_ATLAS_ID_OFFSET,val);
//}
//constexpr inline void set32_face_dir(u32& p, u8 val) {
//    assert_leq(val, FACE_DIR_MAX);
//    SET_BITFIELD_MEMBER_VAL(p, FACE_DIR_MASK,FACE_DIR_OFFSET,val);
//}


#define LIGHTING_MASK (SUNLIGHT_INTENSITY_MASK|BLOCKLIGHT_R_MASK|BLOCKLIGHT_G_MASK|BLOCKLIGHT_B_MASK)

#define COPY_LIGHTING_BITS(src_bits, dst_bits) \
    (  dst_bits = ( CLR_BITS(dst_bits,LIGHTING_MASK) | GET_BITS(src_bits, LIGHTING_MASK) )  )

#endif
