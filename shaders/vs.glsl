#version 330 core
layout (location = 0)       in vec3 in_local_pos;
layout (location = 1)       in vec2 in_tx_coord; 
layout (location = 2)       in uint in_packed_0; 
#include "include/SharedShaderConfig.hpp"
flat out uint texture_id;

out vec2 texCoord;
out vec4 faceOverlayColor;
out float fakeShadowOpacity;
out float face_opacity;


uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;


uint unpack_sunlight_intensity(uint p) {
    return get_val(p, sunlight_intensity_mask,sunlight_intensity_offset);
}
uint unpack_blocklight_r(uint p) {
    return get_val(p, blocklight_r_mask,blocklight_r_offset);
}
uint unpack_blocklight_g(uint p) {
    return get_val(p, blocklight_g_mask,blocklight_g_offset);
}
uint unpack_blocklight_b(uint p) {
    return get_val(p, blocklight_b_mask,blocklight_b_offset);
}
float unpack_face_opacity(uint p) {
    return (get_val(p, face_opacity_mask,face_opacity_offset)) / 255.0f;
}
uint unpack_tex_atlas_id(uint p) {
    return get_val(p, tex_atlas_id_mask,tex_atlas_id_offset);
}
uint unpack_face_dir(uint p) {
    return get_val(p, face_dir_mask,face_dir_offset);
}

void main(){
    texCoord = in_tx_coord;

    texture_id = unpack_tex_atlas_id(in_packed_0);
    face_opacity = unpack_face_opacity(in_packed_0);

    uint face_dir = unpack_face_dir(in_packed_0);
    fakeShadowOpacity = faceShadowOpacity[face_dir];

    gl_Position = proj * view * model * vec4(in_local_pos.xyz, 1.0);
}
