#version 330 core
in vec2 texCoord;
out vec4 FragColor;

uniform float blendFactor;
uniform sampler2D texture1;
uniform sampler2D texture2;

vec2 t1coord;
vec2 t2coord;
void main(){
    t1coord = texCoord;
    t2coord = texCoord;

    FragColor = mix(texture(texture1, t1coord), texture(texture2, t2coord), blendFactor);
}
