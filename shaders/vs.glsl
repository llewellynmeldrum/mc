#version 330 core
layout (location = 0) in vec3 local_pos;
layout (location = 1) in vec2 faceCoord; 
// per vertex, what is the offset IN the texture

out vec2 texCoord;


uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform float texOffset;
void main(){
    // each face has 
    texCoord = faceCoord;
    texCoord.x+=texOffset;
    gl_Position = proj * view * model * vec4(local_pos.xyz, 1.0);
}
