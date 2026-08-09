#version 330 core
layout (location = 0)       in vec2 v_pos;
layout (location = 1)       in vec3 v_ray;
out vec3 ray;
void main(){
    ray = v_ray;
    gl_Position = vec4(v_pos,1.0f,1.0f);
}
