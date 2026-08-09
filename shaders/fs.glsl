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
	0.0078125, 0.7578125, 0.1953125, 0.9453125, 0.0546875, 0.8046875, 0.2421875, 0.9921875,
	0.5078125, 0.2578125, 0.6953125, 0.4453125, 0.5546875, 0.3046875, 0.7421875, 0.4921875,
	0.1328125, 0.8828125, 0.0703125, 0.8203125, 0.1796875, 0.9296875, 0.1171875, 0.8671875,
	0.6328125, 0.3828125, 0.5703125, 0.3203125, 0.6796875, 0.4296875, 0.6171875, 0.3671875,
	0.0390625, 0.7890625, 0.2265625, 0.9765625, 0.0234375, 0.7734375, 0.2109375, 0.9609375,
	0.5390625, 0.2890625, 0.7265625, 0.4765625, 0.5234375, 0.2734375, 0.7109375, 0.4609375,
	0.1640625, 0.9140625, 0.1015625, 0.8515625, 0.1484375, 0.8984375, 0.0859375, 0.8359375,
	0.6640625, 0.4140625, 0.6015625, 0.3515625, 0.6484375, 0.3984375, 0.5859375, 0.3359375
);
const float bayer_matrix_4x4[4*4] = float[](
	0.03125, 0.53125, 0.15625, 0.65625,
	0.78125, 0.28125, 0.90625, 0.40625,
	0.21875, 0.71875, 0.09375, 0.59375,
	0.96875, 0.46875, 0.84375, 0.34375
);
const float bayer_matrix_2x2[2*2] = float[](
	0.125, 0.625,
	0.875, 0.375
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
