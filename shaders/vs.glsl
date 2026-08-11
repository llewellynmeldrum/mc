#version 330 core
layout (location = 0)       in vec3 in_local_pos;
layout (location = 1)       in vec2 in_tx_coord; 
layout (location = 2)       in uint in_packed_0; 

#define MASK(offset, len) ( ( (0xffffffffu) >> (32u-(len))) << (offset) )
#include "include/packed0.def"
#include "include/SharedShaderConfig.hpp"


        out vec2     tx_coord;
        out float    fake_shadow; 
        out float    block_ao; 
        out vec3     ao_dbg_col; 
        out float    face_opacity;
flat    out uint     tex_atlas_id;
        out vec3     final_light;
        out vec4     view_pos;


uniform vec3 u_sunlight_rgb;
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;


uniform float u_global_sun_intensity_scale;

uniform bool u_enable_sunlight;
uniform bool u_enable_blocklight;
uniform bool u_enable_smooth_light_falloff;
uniform float u_blocklight_smooth_falloff_factor;
uniform float u_sunlight_smooth_falloff_factor;

const float AO_STATE_LUT[] = float[](
    1.00f, // 00 (0) - no AO
    0.85f, // 01 (1) - only corner
    0.85f, // 10 (2) - one side
    0.55f // 11 (3) - both sides, corner doesnt matter
);
const vec3 ao_color_lut[] = vec3[](
    vec3(1.0,1.0,1.0),
    vec3(1.0,0.0,1.0),
    vec3(1.0,0.0,1.0),
    vec3(1.0,0.0,1.0)
);
void main(){
//    x
    tx_coord = in_tx_coord;

    tex_atlas_id = UNPACK_TEX_ATLAS_ID(in_packed_0);
    face_opacity = UNPACK_FACE_OPACITY(in_packed_0);

    uint face_dir = UNPACK_FACE_DIR(in_packed_0);
    fake_shadow = fake_shadow_multiplier_per_face[face_dir];
    block_ao = AO_STATE_LUT[UNPACK_AO_STATE(in_packed_0)];
    ao_dbg_col = ao_color_lut[UNPACK_AO_STATE(in_packed_0)];

     
    
    // SECTION: LIGHTING 
    vec3 blocklight_rgb = vec3(0.0f); 
    if (u_enable_blocklight){
        blocklight_rgb = vec3(
            GET_BLOCKLIGHT_R(in_packed_0), 
            GET_BLOCKLIGHT_G(in_packed_0), 
            GET_BLOCKLIGHT_B(in_packed_0) 
        );
        if (u_enable_smooth_light_falloff){
            blocklight_rgb = pow(vec3(u_blocklight_smooth_falloff_factor),vec3(15u) - blocklight_rgb);
        }else{
            blocklight_rgb = blocklight_rgb / vec3(15u);
        }
    }
    vec3 sunlight_rgb = vec3(0.0f);
    if (u_enable_sunlight){
        float sunlight_intensity = UNPACK_SUNLIGHT_INTENSITY(in_packed_0);
        if (u_enable_smooth_light_falloff){
            sunlight_intensity = 0.1 + pow(u_sunlight_smooth_falloff_factor,(1.0f - sunlight_intensity));
        }
        sunlight_rgb = u_sunlight_rgb * sunlight_intensity  * u_global_sun_intensity_scale;
    }
    
    final_light = max(sunlight_rgb, blocklight_rgb);
    view_pos =  u_view * u_model * vec4(in_local_pos.xyz, 1.0);
    gl_Position = u_proj *view_pos;
}
