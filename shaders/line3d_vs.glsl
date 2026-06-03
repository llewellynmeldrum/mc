#version 330 core
layout (location = 0)       in vec2 corner;
layout (location = 1)       in vec3 w_startPos; 
layout (location = 2)       in vec3 w_endPos; 
layout (location = 3)       in vec4 color; 
layout (location = 4)       in float thickness;  // world space thickness

out vec4 frag_color;
out float t;
out float perp;

uniform mat4 view;
uniform mat4 proj;


void main(){

    // corner.x encodes t 
    // corner.y encodes 

    t = corner.x; // how far along the line we are, from 0 (start) -> 1 (end)
    perp = corner.y; // the perpendicular to line offset scale each quad takes on.
    float half_thick = thickness * 0.5;


    // vertex we are referring to 
    vec3 w_pointPos = mix(w_startPos, w_endPos, t);
    vec3 v_pointPos = (view * vec4(w_pointPos, 1.0)).xyz;

    // line dir from start to end
    vec3 w_lineDir = w_endPos-w_startPos; 
    vec3 v_lineDir = (view * vec4(w_lineDir, 0.0)).xyz; // w=0, no translation (i think?)


    // dir from point->cam (cam = origin in viewspace)
    vec3 v_dirTowardsCamera = normalize(-v_pointPos);
    vec3 v_perpDir = normalize(cross(v_dirTowardsCamera, v_lineDir));


    // the final pos is the perpDir * half thickness * perp from vtx data
    vec3 v_finalOffset = v_perpDir * half_thick * perp;
    vec3 v_finalPos = v_pointPos + v_finalOffset;

    // project 
    gl_Position = proj * vec4(v_finalPos, 1.0);

    frag_color = color;
}


