#version 330 core
#include "include/SharedShaderConfig.hpp"


        in vec2     tx_coord;
        in vec4     faceOverlayColor;
        in float    fakeShadowOpacity; 
        in float    face_opacity;
flat    in uint     tex_atlas_id;
        in vec3     light_color;

out vec4    FragColor;

uniform sampler2D u_texture_atlases[TEX_ATLAS_COUNT];
uniform bool u_enable_cutout;



void main(){
    // 1. sample the texture 
    vec4 tx_color = texture(u_texture_atlases[tex_atlas_id],tx_coord);

    // 2. handle cutouts 
    if (u_enable_cutout){
        if (tx_color.a < 0.5f){
            discard;
        }
    }

    // 3. apply lighting
    vec3 shadow = vec3(1.0f - fakeShadowOpacity);
    vec3 light = light_color;
    vec3 tx_color_3 = vec3(tx_color);
    FragColor = vec4(tx_color_3 * light , tx_color.a);




    //FragColor = mix(faceOverlayColor,FragColor,0.2f);

}
