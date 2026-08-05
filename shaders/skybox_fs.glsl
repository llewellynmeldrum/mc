#version 330 core 

#define f32 float
in vec3 ray;
in vec3 dbg_col;
out vec4    FragColor;
uniform vec3 u_base_color;

uniform vec3 u_color_horizon;     // color of the sky at the horizon line
uniform vec3 u_color_sun;
uniform vec3 u_color_sunlight;
uniform vec3 u_color_sun_glow;
uniform vec3 u_color_lowsky;      
uniform vec3 u_color_midsky;      
uniform vec3 u_color_zenith;      // color of the sky at the highest point (top center)

uniform vec3 u_dir_moon;
uniform vec3 u_dir_sun;
uniform f32 u_sun_intensity_scale;
uniform f32 u_star_alpha;
uniform f32 u_glow_scale;


#define lerp mix
#define constrain(lo, hi, x) ((x < lo )? lo : (x>hi ? hi: x))

vec3 sun_color(vec3 facing, float t_sky){

    float t_horizon = 1.0f-t_sky;
    float d = clamp(dot(u_dir_sun,facing), 0.0,1.0f);
    float disc = pow(d, 2000.0);
    float glow = pow(d, 8.0) * 0.3;
    float contrib = (disc + (u_glow_scale * glow)) * pow(t_sky, 10.0);
    vec3 sun = u_color_sun * constrain(0.0f,1.0f, contrib);
    return sun;
}

vec3 get_sky_color(float facing_y){
    const float lomax = 0.01;
    float low= smoothstep(0.00, 0.10, facing_y);
    float mid= smoothstep(0.10, 0.15, facing_y);
    float hi= smoothstep(0.15, 0.80, facing_y);

    vec3 sky = mix(u_color_lowsky, u_color_midsky, mid);
    sky = mix(sky, u_color_zenith, hi);
    return sky;
}
void main(){
    // zenith = the highest point in the sky (directly world up)
    // nadir = the LOWEST point in the sky (directly world down)
    vec3 facing = normalize(ray);
    float t_sky = constrain(0.0f,1.0f, facing.y * 10.0f + 1.0f);
    // values between 0.4 and above should
    vec3 sky_color = get_sky_color(facing.y);

    // make the sun scale to appear larger/ more influential at the horizon
    float t_horizon = 1.0f-t_sky;
    // scale at the 0.25f
    sky_color+=sun_color(facing,t_sky) * constrain(1.0f,2.0f,t_horizon;
    sky_color = lerp(sky_color, u_color_horizon, pow(t_horizon,2));

    FragColor = vec4(sky_color,1.0f);
}
