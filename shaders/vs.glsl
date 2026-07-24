#version 330 core
layout (location = 0)       in vec3 local_pos;
layout (location = 1)       in vec2 faceCoord; 
layout (location = 2)       in vec4 in_faceOverlayColor; 
layout (location = 3)       in int face_idx; 
layout (location = 4)       in float in_blockOpacity; 
layout (location = 5)       in int in_texture_id; 
#include "include/SharedShaderConfig.hpp"
flat out int texture_id;

out vec2 texCoord;
out vec4 faceOverlayColor;
out float fakeShadowOpacity;
// out float blockOpacity;


uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;



void main(){
    texture_id = in_texture_id;
    texCoord = faceCoord;

    faceOverlayColor = in_faceOverlayColor;

    fakeShadowOpacity = faceShadowOpacity[face_idx];

    gl_Position = proj * view * model * vec4(local_pos.xyz, 1.0);
}
