#version 330 core
layout (location = 0)       in vec3 in_local_pos;
layout (location = 1)       in vec2 in_tx_coord; 
layout (location = 2)       in uint in_packed_0; 
#include "include/SharedShaderConfig.hpp"
flat out uint tex_atlas_id;

out vec2 tx_coord;
out vec4 faceOverlayColor;
out float fake_shadow;
out float face_opacity;
out vec3 final_light;

uniform vec3 u_sunlight_rgb;
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;


uniform bool u_enable_sunlight;
uniform bool u_enable_blocklight;
uniform bool u_enable_smooth_light_falloff;
uniform float u_blocklight_smooth_falloff_factor;
uniform float u_sunlight_smooth_falloff_factor;


void main(){
    tx_coord = in_tx_coord;

    tex_atlas_id = unpack_tex_atlas_id(in_packed_0);
    face_opacity = unpack_face_opacity(in_packed_0);

    uint face_dir = unpack_face_dir(in_packed_0);
    fake_shadow = fake_shadow_multiplier_per_face[face_dir];

    
    // SECTION: LIGHTING 
    vec3 blocklight_rgb = vec3(0.0f); 
    if (u_enable_blocklight){
        blocklight_rgb = vec3(
            get_blocklight_r(in_packed_0), // [0,15]
            get_blocklight_g(in_packed_0), // [0,15]
            get_blocklight_b(in_packed_0) // [0,15]
        );
        if (u_enable_smooth_light_falloff){
            blocklight_rgb = pow(vec3(u_blocklight_smooth_falloff_factor),vec3(15u) - blocklight_rgb);
        }else{
            blocklight_rgb = blocklight_rgb / vec3(15u);
        }
    }
    vec3 sunlight_rgb = vec3(0.0f);
    if (u_enable_sunlight){
        float sunlight_intensity = unpack_sunlight_intensity(in_packed_0);
        if (u_enable_smooth_light_falloff){
            sunlight_intensity = 0.1 + pow(u_sunlight_smooth_falloff_factor,(1.0f - sunlight_intensity));
        }
        sunlight_rgb = u_sunlight_rgb * sunlight_intensity ;
    }
    final_light = max(sunlight_rgb, blocklight_rgb);
    gl_Position = u_proj * u_view * u_model * vec4(in_local_pos.xyz, 1.0);

}
