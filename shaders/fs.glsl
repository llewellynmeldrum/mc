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
uniform bool u_enable_fog;
uniform bool u_fade_in_chunks;

#define RGB(r,g,b) vec3(r,g,b)
#define ROW_MAJOR_IDX2D(p, width, height) (((p.y) * (width)) + (p.x))
// 4x4 Bayer Matrix lookup table packed into an array
// credit: [wikipedia - Ordered Dithering](https://en.wikipedia.org/wiki/Ordered_dithering)

const float bayer_matrix_8x8[8*8] = float[](
	0, 0.75, 0.1875, 0.9375, 0.046875, 0.796875, 0.234375, 0.984375,
	0.5, 0.25, 0.6875, 0.4375, 0.546875, 0.296875, 0.734375, 0.484375,
	0.125, 0.875, 0.0625, 0.8125, 0.171875, 0.921875, 0.109375, 0.859375,
	0.625, 0.375, 0.5625, 0.3125, 0.671875, 0.421875, 0.609375, 0.359375,
	0.03125, 0.78125, 0.21875, 0.96875, 0.015625, 0.765625, 0.203125, 0.953125,
	0.53125, 0.28125, 0.71875, 0.46875, 0.515625, 0.265625, 0.703125, 0.453125,
	0.15625, 0.90625, 0.09375, 0.84375, 0.140625, 0.890625, 0.078125, 0.828125,
	0.65625, 0.40625, 0.59375, 0.34375, 0.640625, 0.390625, 0.578125, 0.328125
);
const float bayer_matrix_4x4[4*4] = float[](
	0.0000, 0.5000, 0.1250, 0.6250,
	0.7500, 0.2500, 0.8750, 0.3750,
	0.1875, 0.6875, 0.0625, 0.5625,
	0.9375, 0.4375, 0.8125, 0.3125
);
const float bayer_matrix_2x2[2*2] = float[](
	0, 0.5,
	0.75, 0.25
);

float bayer8(vec2 uv) { return float(bayer_matrix_8x8[ROW_MAJOR_IDX2D((ivec2(uv)%8),8,8)]); }
float bayer4(vec2 uv) { return float(bayer_matrix_4x4[ROW_MAJOR_IDX2D((ivec2(uv)%4),4,4)]); }
float bayer2(vec2 uv) { return float(bayer_matrix_2x2[ROW_MAJOR_IDX2D((ivec2(uv)%2),2,2)]); }

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
    if (u_fade_in_chunks){
        if (u_chunk_opacity < bayer8(gl_FragCoord.xy)) discard;
    }
    vec4 tex_color = texture(u_texture_atlases[tex_atlas_id],tx_coord);

    // 2. handle cutouts 
    if (u_enable_cutout){
        if (tex_color.a < 0.5f){
            discard;
        }
    }

    float dist = length(view_pos);

    float fog_t = pow(smoothstep(u_world_fog_start,u_world_fog_end,dist),2);
    float fade_t = smoothstep(u_world_fog_start,u_world_fog_end,dist);
    float fade_alpha = 1.0f - fade_t;
    float alpha = tex_color.a;
    if (u_enable_fog){
        if (fade_alpha *1.5f < bayer8(gl_FragCoord.xy)) discard;
        alpha = mix(alpha,fade_alpha,0.2f);// * fade_alpha;
    }

    vec3 light = vec3(fake_shadow);
    if (u_enable_lighting) {
        light *= u_gamma * final_light;
    }
    vec3 color = tex_color.rgb * light;

    if (u_enable_fog){
        color = mix(color, u_fog_color, clamp(fog_t,0.0f,0.40f));
    }

    FragColor = vec4(color,alpha);
}
