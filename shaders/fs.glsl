#version 330 core
#include "include/SharedShaderConfig.hpp"


        in vec2     tx_coord;
        in vec4     faceOverlayColor;
        in float    fake_shadow; 
        in float    face_opacity;
flat    in uint     tex_atlas_id;
        in vec3     final_light;
        in vec4     view_pos;

out vec4    FragColor;

// Global uniforms
uniform sampler2D u_texture_atlases[TEX_ATLAS_COUNT];
uniform bool u_enable_cutout;
uniform bool u_enable_lighting;
uniform float u_gamma;
uniform vec3 u_fog_color;
uniform float u_world_fog_start;
uniform float u_world_fog_end;

// Chunk specific uniforms
uniform float u_chunk_opacity;

#define RGB(r,g,b) vec3(r,g,b)
float bayer4x4(vec2 uv) {
    // Get integer pixel coordinates
    ivec2 p = ivec2(uv);
    
    // Perform bitwise operations to calculate the 4x4 Bayer pattern
    int x = p.x & 3;
    int y = p.y & 3;
    
    // 4x4 Bayer Matrix lookup table packed into an array
    int matrix[16] = int[](
         0,  8,  2, 10,
        12,  4, 14,  6,
         3, 11,  1,  9,
        15,  7, 13,  5
    );
    
    // Look up the value and normalize it to the 0.0 - 1.0 range
    return float(matrix[y * 4 + x]) / 16.0;
}

float linstep(float edge0, float edge1, float x){
    float t = clamp((x-edge0)/(edge1-edge0),0.0,1.0);
    return t;
}
float expstep(float edge0, float edge1, float x, float exponent){
    float t = clamp((x-edge0)/(edge1-edge0),0.0,1.0);
    return pow(t,exponent);
}
void main(){
    // 1. sample the texture 
    float bayer = bayer4x4(ivec2(gl_FragCoord.xy) % 4);
    if (u_chunk_opacity < bayer) discard;
    vec4 tex_color = texture(u_texture_atlases[tex_atlas_id],tx_coord);

    // 2. handle cutouts 
    if (u_enable_cutout){
        if (tex_color.a < 0.5f){
            discard;
        }
    }

    float dist = length(view_pos);

    float fog_t = pow(smoothstep(u_world_fog_start*0.5,u_world_fog_end,dist),2);
    float fade_t = smoothstep(u_world_fog_start,u_world_fog_end,dist);
    float fade_alpha = 1.0f - fade_t;
    float alpha = tex_color.a * fade_alpha;

    vec3 light = vec3(1.0f);
    if (u_enable_lighting) {
        light = u_gamma * final_light * fake_shadow;
    }
    vec3 color = tex_color.rgb * light;

    color = mix(color, u_fog_color, clamp(fog_t,0.0f,0.40f));

    FragColor = vec4(color,alpha);
}
