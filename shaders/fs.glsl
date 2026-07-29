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
uniform bool u_enable_lighting;



void main(){
    // 1. sample the texture 
    vec4 obj_color = texture(u_texture_atlases[tex_atlas_id],tx_coord);

    // 2. handle cutouts 
    if (u_enable_cutout){
        if (obj_color.a < 0.5f){
            discard;
        }
    }

    if (!u_enable_lighting) {
        FragColor = obj_color;
        return;
    }
    const f32 ambient_strength = 0.5f;
    // 3. apply lighting
    //vec3 shadow = vec3(1.0f - fakeShadowOpacity);
    vec3 ambient = ambient_strength * light_color;
    vec3 light = ambient;
    vec3 color = vec3(obj_color) * light;
    FragColor = vec4(color, obj_color.a);

}
