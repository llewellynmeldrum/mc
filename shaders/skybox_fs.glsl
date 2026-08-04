#version 330 core 

in vec3 ray;
in vec3 dbg_col;
out vec4    FragColor;
uniform vec3 u_base_color;
void main(){
    vec3 norm_ray = normalize(ray);

    FragColor = vec4(dbg_col,1.0f);
//    FragColor = vec4(u_base_color,1.0f);
}
