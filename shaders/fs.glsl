#version 330 core

in vec2 texCoord;
in vec4 faceOverlayColor;
in float fakeShadowOpacity; 


out vec4 FragColor;

uniform float overlayOpacity;
uniform sampler2D texture1;



const vec4 BLACK = vec4(0.0,0.0,0.0, 1.0f);

void main(){
    FragColor = texture(texture1,texCoord);
    FragColor = mix(FragColor, faceOverlayColor, overlayOpacity);
    FragColor = mix(FragColor, BLACK, fakeShadowOpacity);
}
