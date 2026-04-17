#version 330 core
in vec2 texCoord;
out vec4 FragColor;

uniform float blendFactor;
uniform sampler2D texture1;
uniform sampler2D texture2;

void main(){
    FragColor = texture(texture1,texCoord);
}
