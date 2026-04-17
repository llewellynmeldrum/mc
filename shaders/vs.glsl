#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 vs_texCoord;
layout (location = 2) in vec3 vs_vertColor;

out vec2 texCoord;

uniform mat4 scale;
uniform mat4 rotation;
void main(){
    gl_Position = scale * rotation * vec4(pos.xyz, 1.0);
    texCoord = vs_texCoord;
}
