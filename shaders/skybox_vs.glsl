#version 330 core
layout (location = 0)       in vec2 v_pos;
layout (location = 1)       in vec3 v_ray;
out vec3 ray;
out vec3 dbg_col;
void main(){
    dbg_col = vec3(v_pos.x, v_pos.y, 0.0f);
    ray = v_ray;
    gl_Position = vec4(ray,1.0f);
}
