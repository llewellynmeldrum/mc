#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 vs_texCoord;
layout (location = 2) in vec3 vs_vertColor;

out vec2 texCoord;

uniform int triangle_idx;

uniform mat4 scale0;
uniform mat4 scale1;
uniform mat4 rotation;
uniform mat4 mouse_transform;
void main(){
    switch (triangle_idx){
        case 0:
            gl_Position = scale0 * vec4(pos.xyz, 1.0);
        break;
        case 1: 
            gl_Position = scale1 * rotation * vec4(pos.xyz, 1.0);
        break;
        case 2: 
            gl_Position = mouse_transform * vec4(pos.xyz, 1.0);
        break;
    }
    texCoord = vs_texCoord;
}
