#version 330 core
layout (location = 0) in ivec3 cube_local_pos; 
layout (location = 1) in ivec3 instance_origin_pos;
layout (location = 2) in vec4 instance_color;

out vec4 frag_color;

uniform mat4 view;
uniform mat4 proj;


void main(){
    vec3 world_pos = vec3(cube_local_pos + instance_origin_pos);
    frag_color = instance_color;
    gl_Position = proj * view * vec4(world_pos, 1.0);
}
