#version 330 core
#include "include/shaders/utils.glsl"
#include "include/SharedShaderConfig.hpp"
#include "include/shaders/noise.glsl"
#include "include/shaders/bayer.glsl"

        in vec2     tx_coord;
        in float    fake_shadow; 
        in float    block_ao; 
        in vec3     ao_dbg_col; 
        in float    face_opacity;
flat    in uint     tex_atlas_id;
        in vec3     final_light;
        in vec4     view_pos;

out vec4    FragColor;

// Global uniforms
uniform sampler2D u_texture_atlases[TEX_ATLAS_COUNT];
uniform float u_gamma;
uniform vec3 u_fog_color;
uniform float u_world_fog_start;
uniform float u_world_fog_end;

// Chunk specific uniforms
uniform float u_chunk_opacity;

uniform bool u_enable_fog;
uniform bool u_enable_cutout;
uniform bool u_enable_lighting;
uniform bool u_fade_in_chunks;
uniform bool u_enable_fake_shadows;//
uniform bool u_enable_block_ambient_occlusion;//

    uniform bool u_wireframe_mode;

void main(){
    // 1. sample the texture 
    if (u_fade_in_chunks){
        // bayer fade distant chunks, so opaque and blended blocks disappear at the same time
        if (u_chunk_opacity < bayer8(gl_FragCoord.xy)) discard;
    }
    if (u_wireframe_mode){
        FragColor = vec4(1.0f);
        return;
    }
    vec4 tex_color = vec4(1.0f);
    tex_color = texture(u_texture_atlases[tex_atlas_id],tx_coord);
    // 2. handle cutouts 
    if (u_enable_cutout){
        // cutout is sourced from the texture, simply use an alpha of <50% (preferably 0%).
        // The texure alpha, for now, is not used anywhere else. 
        // For blended blocks, set their opacity in the blocks definition.
        if (tex_color.a < 0.5f){
            discard;
        }
    }

    float dist = length(view_pos);

    float alpha = face_opacity;
    if (u_enable_fog){
        float fade_t = smoothstep(u_world_fog_start,u_world_fog_end,dist);
        float fade_alpha = 1.0f - fade_t;
        if (fade_alpha *1.5f < bayer8(gl_FragCoord.xy)) discard;
        alpha = mix(alpha,fade_alpha,0.2f);// * fade_alpha;
    }

    vec3 light = vec3(1.0f);
    if (u_enable_fake_shadows){
        light *= vec3(fake_shadow);
    }
    if (u_enable_block_ambient_occlusion){
        light *= vec3(block_ao);
    }
    if (u_enable_lighting) {
        light *= u_gamma * final_light;
    }

    vec3 color = tex_color.rgb * light;

    if (u_enable_fog){
        float fog_t = pow(smoothstep(u_world_fog_start,u_world_fog_end,dist),2);
        color = mix(color, u_fog_color, clamp(fog_t,0.0f,0.40f));
    }

    FragColor = vec4(color,face_opacity);
    //FragColor = vec4(color,alpha);
}
