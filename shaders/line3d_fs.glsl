#version 330 core

in vec4 frag_color;
out vec4 FragColor;


void main(){
    FragColor = vec4(frag_color);
    // interpolate based on distance to edge of line later
}
