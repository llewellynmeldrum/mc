#version 330 core
layout (location = 0)       in vec3 local_pos;
layout (location = 1)       in vec2 faceCoord; 
layout (location = 2)       in vec4 in_faceOverlayColor; 
layout (location = 3)       in int face_idx; 
layout (location = 4)       in float in_blockOpacity; 
layout (location = 5)       in int in_block_shape; 
flat out int block_shape;

out vec2 texCoord;
out vec4 faceOverlayColor;
out float fakeShadowOpacity;
// out float blockOpacity;


uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

#define DIR_COUNT 6

#define FORWARD  0
#define BACKWARD 1
#define LEFT     2
#define RIGHT    3 
#define DOWN     4
#define UP       5

void main(){
    const float faceShadowOpacity[DIR_COUNT] = float[DIR_COUNT](
        0.1,
        0.5,
        0.3,
        0.3,
        0.3,
        0.0
    );
    block_shape = in_block_shape;
    texCoord = faceCoord;

    faceOverlayColor = in_faceOverlayColor;

    fakeShadowOpacity = faceShadowOpacity[face_idx];

    gl_Position = proj * view * model * vec4(local_pos.xyz, 1.0);
}
