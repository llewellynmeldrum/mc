#version 330 core 

#include "include/shaders/noise.glsl"
#include "include/shaders/utils.glsl"
// GLSL PREFIXES
// u_   uniform variable.
// t_   a normalized (0.0->1.0) float which represents the '...ness' of something
in vec3 ray;
out vec4    FragColor;

uniform vec3 u_color_sun_disc;
uniform vec3 u_color_sunlight;
uniform vec3 u_color_sun_glow;
uniform float u_horizon_hug_intensity;
uniform vec3 u_color_outer_sun_glow;

uniform vec3 u_color_horizon;     // color of the sky at the horizon line
uniform vec3 u_color_lowsky;      
uniform vec3 u_color_midsky;      
uniform vec3 u_color_zenith;      // color of the sky at the highest point (top center)

uniform vec3    u_dir_sun;
uniform f32     u_sun_intensity_scale;

uniform f32  u_star_alpha;
uniform vec3 u_dir_moon;
uniform f32 u_belt_of_venus_intensity;


// Simplex 2D noise
void cube_uv(vec3 d, out vec2 uv, out float face){
    vec3 a = abs(d);
    if (a.x >= a.y && a.x >= a.z){ face = d.x > 0.0 ? 0.0 : 1.0; uv = d.zy / a.x; }
    else if (a.y >= a.z)         { face = d.y > 0.0 ? 2.0 : 3.0; uv = d.xz / a.y; }
    else                         { face = d.z > 0.0 ? 4.0 : 5.0; uv = d.xy / a.z; }
}

// returns the degrees of the angle between two normalized direction vectors
float deg3_between(vec3 a, vec3 b){
    float cos_theta = dot(a,b);
    float theta = acos(cos_theta);
    return degrees(theta);
}
// returns the degrees of the angle between two normalized direction vectors
float deg2_between(vec2 a, vec2 b){
    a= normalize(a);
    b= normalize(b);
    float cos_theta = dot(a,b);
    float theta = acos(cos_theta);
    return degrees(theta);
}
const f32 horizon_hug_center_deg      = -2.0; 
const f32 horizon_hug_thickness_edge  = 0.4;
const f32 horizon_hug_thickness_deg = 8.0;
const f32 sun_disc_radius_deg = 3.0;
const f32 sun_disc_edge_deg = 0.5;

const f32 sun_glow_edge_deg = 1;
const f32 sun_glow_edge_brightness = 0.1;

const f32 sun_glow_inner_edge_deg = 4.5;
const f32 sun_glow_inner_edge_brightness = 0.4;

const f32 outer_sun_glow_edge_deg = 28;
const f32 outer_sun_glow_edge_brightness = 0.1;
void apply_sun_color(inout vec3 sky_color, vec3 facing){
    f32 deg_from_sun = deg3_between(facing, u_dir_sun);
    f32 elevation = asin(facing.y);

    float t_disc = 
        1.0 - smoothstep(sun_disc_radius_deg - sun_disc_edge_deg,
                         sun_disc_radius_deg + sun_disc_edge_deg, 
                         deg_from_sun);

    // gaussian blur 
    float t_inner_edge_glow = 
        pow(sun_glow_inner_edge_brightness, sq(deg_from_sun / sun_glow_inner_edge_deg)); 

    float t_outer_edge_glow = 
        pow(sun_glow_edge_brightness, sq(deg_from_sun / sun_glow_edge_deg)); 

    float t_outer_glow = 
        pow(outer_sun_glow_edge_brightness, sq(deg_from_sun / outer_sun_glow_edge_deg));

    float t_horizon_hug_glow = 
        pow(0.1, sq(deg_from_sun / 140)); 
    float hug_thickness = horizon_hug_thickness_deg * t_horizon_hug_glow;
    f32 t_vert = pow(horizon_hug_thickness_edge,
                     sq((degrees(elevation) - horizon_hug_center_deg) / hug_thickness));
    t_horizon_hug_glow = t_horizon_hug_glow * t_vert * u_horizon_hug_intensity;

    sky_color = mix(sky_color, u_color_outer_sun_glow, t_outer_glow * 1.5);
    sky_color = mix(sky_color, u_color_outer_sun_glow, t_horizon_hug_glow * 0.5);

    vec3 disc_color = u_color_sun_disc *0.96;
    vec3 inner_glow_color = mix(u_color_sun_disc, u_color_sun_glow,0.0);

    sky_color = mix(sky_color, u_color_sun_glow, clamp(t_outer_edge_glow * 1.0,0.0,1.0));
    sky_color = mix(sky_color, disc_color, t_disc);
    sky_color = mix(sky_color, inner_glow_color, clamp(t_inner_edge_glow*1.0,0.0,1.0));
}

const f32 moon_disc_radius_deg = 1.0;
const f32 moon_disc_edge_deg = 0.05;

const f32 moon_glow_edge_deg = 1;
const f32 moon_glow_edge_brightness = 0.1;

const f32 moon_glow_inner_edge_deg = 2.5;
const f32 moon_glow_inner_edge_brightness = 0.02;

const f32 outer_moon_glow_edge_deg = 10;
const f32 outer_moon_glow_edge_brightness = 0.1;
void apply_moon_color(inout vec3 sky_color, vec3 facing){
    f32 deg_from_moon = deg3_between(facing, u_dir_moon);
    f32 elevation = asin(facing.y);

    float t_disc = 
        1.0 - smoothstep(moon_disc_radius_deg - moon_disc_edge_deg,
                         moon_disc_radius_deg + moon_disc_edge_deg, 
                         deg_from_moon);

    // gaussian blur 
    float t_inner_edge_glow = 
        pow(moon_glow_inner_edge_brightness, sq(deg_from_moon / moon_glow_inner_edge_deg)); 

    float t_outer_edge_glow = 
        pow(moon_glow_edge_brightness, sq(deg_from_moon / moon_glow_edge_deg)); 

    float t_outer_glow = 
        pow(outer_moon_glow_edge_brightness, sq(deg_from_moon / outer_moon_glow_edge_deg));

    float t_horizon_hug_glow = 
        pow(0.1, sq(deg_from_moon / 140)); 
    float hug_thickness = horizon_hug_thickness_deg * t_horizon_hug_glow;
    f32 t_vert = pow(horizon_hug_thickness_edge,
                     sq((degrees(elevation) - horizon_hug_center_deg) / hug_thickness));
    t_horizon_hug_glow = t_horizon_hug_glow * t_vert * u_horizon_hug_intensity;


    vec3 moon_disc_color = rgb(215,215,215);
    vec3 inner_moonglow_color =  rgb(225,225,225);

    vec2 uv; 
    // TODO: add craters to the mooon somehow
    sky_color = mix(sky_color, moon_disc_color, t_disc);
    sky_color = mix(sky_color, inner_moonglow_color, clamp(t_inner_edge_glow*0.2,0.0,1.0));
}
vec3 get_sky_color(float facing_y){
    float low =  smoothstep(0.00, 0.10, facing_y);
    float mid =  smoothstep(0.10, 0.35, facing_y);
    float hi  =  smoothstep(0.35, 0.80, facing_y);

    vec3 sky = mix(u_color_lowsky, u_color_midsky, mid);
    sky = mix(sky, u_color_zenith, hi);
    return sky;
}
float hash2d(vec2 p){
    p = fract(p * vec2(123.34, 456.21));
    p += dot(p, p + 45.32);
    return fract(p.x * p.y);
}
float pcg_hash(uint x) {
    uint state = x * 747796405u + 2891336453u;
    uint word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
    return float((word >> 22u) ^ word) / 4294967295.0; // Divide by uint max
}

const vec3 star_color = vec3(1.0f,1.0f,1.0f);
const float star_radius= 0.1f;
const float star_density = 40.0f;
const float star_threshold = 0.8f;
vec3 get_stars(vec3 dir){
    // stable lat/long coords
    vec2 uv; 
    float face;
    cube_uv(dir,uv,face);
    uv *= star_density;                 // e.g. 60.0 → many cells

    vec2 cell = floor(uv);
    vec2 f    = fract(uv);

    float present = hash2d(cell);
    if (present < star_threshold) return vec3(0.0);   // most cells are empty

    // random position within the cell + random brightness
    vec2  pos    = vec2(hash2d(cell + 1.3), hash2d(cell + 2.7));
    float bright = pow(hash2d(cell + 5.1),4.0);

    float d    = length(f - pos);
    float star = smoothstep(star_radius, 0.0, d);     // round, AA'd dot
    return star * bright * star_color;
}

const f32 bov_center_deg      = -2.0;   // band elevation (just above horizon)
const f32 bov_thickness_edge  = 0.4;
void main(){
    vec3 belt_of_venus_dir = -u_dir_sun;
    const vec3 bov_color = rgb(128,00,128);
    // zenith = the highest point in the sky (directly world up)
    // nadir = the LOWEST point in the sky (directly world down)
    vec3 facing = normalize(ray);
    // values between 0.4 and above should
    vec3 sky_color = get_sky_color(facing.y);

    const f32 horizon_y = -0.1;
    const f32 horizon_rad = 0.1;
    const f32 horizon_blur = 05.0;
    f32 elevation = asin(facing.y);
    // make the sun scale to appear larger/ more influential at the horizon
    float t_horizon = 1.0 - pow(smoothstep(horizon_y - horizon_rad,
                            horizon_y + horizon_rad, 
                            elevation),horizon_blur);

    f32 bov_thickness_deg   = 6.0;   // vertical half-width — SMALL = thin band

    float deg_from_bov = deg3_between(facing, belt_of_venus_dir);
    float t_bov = 
        pow(0.1, sq(deg_from_bov / 140)); 
    bov_thickness_deg *= t_bov;
    f32 t_vert = pow(bov_thickness_edge,
                     sq((degrees(elevation) - bov_center_deg) / bov_thickness_deg));
    t_bov = t_bov * t_vert * u_belt_of_venus_intensity;
    t_bov = t_bov;

    apply_sun_color(sky_color, facing);
    sky_color = mix(sky_color, bov_color, t_bov);
    sky_color = mix(sky_color, u_color_horizon, t_horizon);
    sky_color += get_stars(facing) * u_star_alpha;
    apply_moon_color(sky_color, facing);
    FragColor = vec4(sky_color, 1.0);
    //FragColor = vec4(sky_color, 1.0);

}
