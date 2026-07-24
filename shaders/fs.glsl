#version 330 core
#include "include/SharedShaderConfig.hpp"


in vec2 texCoord;
in vec4 faceOverlayColor;
in float fakeShadowOpacity; 

in float blockOpacity;
flat in int texture_id;

out vec4 FragColor;

//uniform bool u_cutout;
uniform float overlayOpacity;
uniform sampler2D textures[TEXTURE_ID_COUNT];
uniform bool u_enable_cutout;


const vec4 BLACK = vec4(0.0,0.0,0.0, 1.0f);

void main(){
    FragColor = texture(textures[texture_id],texCoord);
    if (u_enable_cutout){
        if (FragColor.a < 0.5f){
            discard;
        }
    }
    //FragColor = mix(faceOverlayColor,FragColor,0.2f);
    #ifdef ENABLE_FAKE_SHADOWS
        FragColor = mix(FragColor, BLACK, fakeShadowOpacity);
    #endif 

}
