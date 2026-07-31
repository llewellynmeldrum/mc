#version 330 core
#include "include/SharedShaderConfig.hpp"


        in vec2     tx_coord;
        in vec4     faceOverlayColor;
        in float    fake_shadow; 
        in float    face_opacity;
flat    in uint     tex_atlas_id;
        in vec3     final_light;

out vec4    FragColor;

uniform sampler2D u_texture_atlases[TEX_ATLAS_COUNT];
uniform bool u_enable_cutout;
uniform bool u_enable_lighting;
uniform float u_gamma;



void main(){
    // 1. sample the texture 
    vec4 tex_color = texture(u_texture_atlases[tex_atlas_id],tx_coord);

    // 2. handle cutouts 
    if (u_enable_cutout){
        if (tex_color.a < 0.5f){
            discard;
        }
    }

    vec3 light = vec3(1.0f);
    if (u_enable_lighting) {
        light = u_gamma * final_light * fake_shadow;
    }
    vec3 color = tex_color.xyz *  light;
    FragColor = vec4(color,face_opacity);
    

}
