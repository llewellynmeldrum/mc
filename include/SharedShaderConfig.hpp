#ifdef __cplusplus 
    #pragma once 
    #define cpp
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
#elif defined(cpp)
    #define START_DECL_ARRAY(T, N, name) inline constexpr std::array<T,N> name{
    #define END_DECL_ARRAY() };
    #define DECL_CONST constexpr inline
    #define DECL_FN constexpr inline
#endif 

#define TEX_ATLAS_COUNT 4
#define ENABLE_FAKE_SHADOWS 
#define DIR_COUNT 6

START_DECL_ARRAY(float, DIR_COUNT, faceShadowOpacity)
    0.15, // FORWARD,
    0.45, // BACKWARD
    0.35, // LEFT,   
    0.35, // RIGHT,//
    0.7, // DOWN, //
    0.0 // UP,     
END_DECL_ARRAY()
// SECTION: SHARED (glsl and c++)

#define CLR_BITS(src_bits, mask) ( (src_bits) & (~(mask)) )
#define GET_BITS(src_bits, mask) ( (src_bits) & (mask) )

#define UNPACK_BITFIELD_MEMBER(dst_bits, FIELD_MASK, FIELD_OFFSET) \
    ( GET_BITS(dst_bits, FIELD_MASK) >> FIELD_OFFSET )

#define SET_BITFIELD_MEMBER_VAL(dst_bits, mask, offset, src_val) \
    ( dst_bits = (CLR_BITS(dst_bits,mask) | GET_BITS((src_val << offset), mask)) )

DECL_CONST uint sunlight_intensity_offset        =  0u;
DECL_CONST uint blocklight_r_offset              =  4u;
DECL_CONST uint blocklight_g_offset              =  8u;
DECL_CONST uint blocklight_b_offset              = 12u;
DECL_CONST uint face_dir_offset                  = 16u;
DECL_CONST uint tex_atlas_id_offset              = 19u;
DECL_CONST uint face_opacity_offset              = 24u;

DECL_CONST uint sunlight_intensity_mask = 0x0000000FU;  //00000000000000000000000000001111; // set in mesher
DECL_CONST uint blocklight_r_mask       = 0x000000F0U;  //00000000000000000000000011110000; // set in mesher
DECL_CONST uint blocklight_g_mask       = 0x00000F00U;  //00000000000000000000111100000000; // set in mesher
DECL_CONST uint blocklight_b_mask       = 0x0000F000U;  //00000000000000001111000000000000; // set in mesher
DECL_CONST uint face_dir_mask           = 0x00070000U;  //00000000000001110000000000000000; // set in ctor
DECL_CONST uint tex_atlas_id_mask       = 0x00380000U;  //00000000001110000000000000000000; // set in ctor
DECL_CONST uint face_opacity_mask       = 0xFF000000U;  //11111111000000000000000000000000; // set in mesher


DECL_CONST uint SUNLIGHT_INTENSITY_MAX = 15u;
DECL_CONST uint BLOCKLIGHT_R_MAX       = 15u;
DECL_CONST uint BLOCKLIGHT_G_MAX       = 15u;
DECL_CONST uint BLOCKLIGHT_B_MAX       = 15u;
DECL_CONST uint FACE_OPACITY_MAX       = 255u;

DECL_FN float unpack_sunlight_intensity(uint p) {
    return UNPACK_BITFIELD_MEMBER(p, sunlight_intensity_mask,sunlight_intensity_offset) / float(SUNLIGHT_INTENSITY_MAX);
}
DECL_FN float unpack_blocklight_r(uint p) {
    return (UNPACK_BITFIELD_MEMBER(p, blocklight_r_mask,blocklight_r_offset)) / float(BLOCKLIGHT_R_MAX);
}
DECL_FN float unpack_blocklight_g(uint p) {
    return (UNPACK_BITFIELD_MEMBER(p, blocklight_g_mask,blocklight_g_offset)) / float(BLOCKLIGHT_G_MAX);
}
DECL_FN float unpack_blocklight_b(uint p) {
    return (UNPACK_BITFIELD_MEMBER(p, blocklight_b_mask,blocklight_b_offset)) / float(BLOCKLIGHT_B_MAX);
}
DECL_FN float unpack_face_opacity(uint p) {
    return (UNPACK_BITFIELD_MEMBER(p, face_opacity_mask,face_opacity_offset)) / float(FACE_OPACITY_MAX);
}
DECL_FN uint unpack_tex_atlas_id(uint p) {
    return UNPACK_BITFIELD_MEMBER(p, tex_atlas_id_mask,tex_atlas_id_offset);
}
DECL_FN uint unpack_face_dir(uint p) {
    return UNPACK_BITFIELD_MEMBER(p, face_dir_mask,face_dir_offset);
}

#if defined(cpp)
constexpr inline void set_sunlight_intensity(u16& p, u8 val) {
    (p = (((p) & (~(sunlight_intensity_mask))) |
          (((val << sunlight_intensity_offset)) & (sunlight_intensity_mask))));
}
constexpr inline void set_blocklight_r(u16& p, u8 val) {
    SET_BITFIELD_MEMBER_VAL(p, blocklight_r_mask,blocklight_r_offset,val);
}
constexpr inline void set_blocklight_g(u16& p, u8 val) {
    SET_BITFIELD_MEMBER_VAL(p, blocklight_g_mask,blocklight_g_offset,val);
}
constexpr inline void set_blocklight_b(u16& p, u8 val) {
    SET_BITFIELD_MEMBER_VAL(p, blocklight_b_mask,blocklight_b_offset,val);
}
constexpr inline void set_face_opacity(u16& p, u8 val) {
    SET_BITFIELD_MEMBER_VAL(p, face_opacity_mask,face_opacity_offset,val);
}
constexpr inline void set_tex_atlas_id(u16& p, u8 val) {
    SET_BITFIELD_MEMBER_VAL(p, tex_atlas_id_mask,tex_atlas_id_offset,val);
}
constexpr inline void set_face_dir(u16& p, u8 val) {
    SET_BITFIELD_MEMBER_VAL(p, face_dir_mask,face_dir_offset,val);
}
#endif 


#define LIGHTING_MASK (sunlight_intensity_mask|blocklight_r_mask|blocklight_g_mask|blocklight_b_mask)

#define COPY_LIGHTING_BITS(src_bits, dst_bits) \
    (  dst_bits = ( CLR_BITS(dst_bits,mask) | GET_BITS(src_bits, LIGHTING_MASK) )  )

