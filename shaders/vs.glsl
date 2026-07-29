#version 330 core
layout (location = 0)       in vec3 in_local_pos;
layout (location = 1)       in vec2 in_tx_coord; 
layout (location = 2)       in uint in_packed_0; 
#include "include/SharedShaderConfig.hpp"
flat out uint tex_atlas_id;

out vec2 tx_coord;
out vec4 faceOverlayColor;
out float fakeShadowOpacity;
out float face_opacity;
out vec3 light_color;

uniform vec3 u_sunlight_color;
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

uniform bool u_enable_smooth_light_falloff;
uniform float u_smooth_light_falloff_base;


void main(){
    tx_coord = in_tx_coord;

    tex_atlas_id = unpack_tex_atlas_id(in_packed_0);
    face_opacity = unpack_face_opacity(in_packed_0);

    
    uint blocklight_r = get_blocklight_r(in_packed_0);
    uint blocklight_g = get_blocklight_g(in_packed_0);
    uint blocklight_b = get_blocklight_b(in_packed_0);
    vec3 block_light_color = vec3(
        blocklight_r,
        blocklight_g,
        blocklight_b
    );
    if (u_enable_smooth_light_falloff){
        block_light_color = pow(vec3(u_smooth_light_falloff_base),vec3(15u)-block_light_color);
    }else{
        block_light_color = block_light_color / vec3(15u);
    }
   // vec3 block_light_color = vec3(0.9f,0.0f,0.0f);

    float sunlight_intensity = unpack_sunlight_intensity(in_packed_0);
    light_color = block_light_color; //block_light_color;

    uint face_dir = unpack_face_dir(in_packed_0);
    fakeShadowOpacity = faceShadowOpacity[face_dir];

    gl_Position = u_proj * u_view * u_model * vec4(in_local_pos.xyz, 1.0);
}
