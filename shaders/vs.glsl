#version 330 core
layout (location = 0) in vec3 local_pos;
layout (location = 1) in vec2 vs_texCoord;

out vec2 texCoord;


uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main(){
    texCoord = vs_texCoord;
    gl_Position = proj * view * model * vec4(local_pos.xyz, 1.0);
}
