#version 330 core

in vec4 frag_color;
in float t;
in float perp;
out vec4 FragColor;


void main(){
    float a = frag_color.a-0.1;
    //a = mix(frag_color.a,0,(t-0.9) * 10);
    FragColor = vec4(frag_color.rgb,a);
    // interpolate based on distance to edge of line later
}
