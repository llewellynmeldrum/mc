#version 330 core
#define BLOCK_SHAPE_COUNT 2
#define ENABLE_FAKE_SHADOWS 

// TODO: implement #include on the c++ side

in vec2 texCoord;
in vec4 faceOverlayColor;
in float fakeShadowOpacity; 

in float blockOpacity;
flat in int block_shape;

out vec4 FragColor;

//uniform bool u_cutout;
uniform float overlayOpacity;
uniform sampler2D textures[BLOCK_SHAPE_COUNT];


const vec4 BLACK = vec4(0.0,0.0,0.0, 1.0f);

void main(){
    FragColor = texture(textures[block_shape],texCoord);
    if (FragColor.a < 0.1f){
        discard;
    }
    //FragColor = mix(faceOverlayColor,FragColor,0.2f);
    #ifdef ENABLE_FAKE_SHADOWS
        FragColor = mix(FragColor, BLACK, fakeShadowOpacity);
    #endif 

}
