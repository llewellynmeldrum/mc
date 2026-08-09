#include "SkyboxState.hpp"
#include <numbers>


constexpr glm::vec3 calc_sun_direction_vec(f32 t){
    static constexpr auto PI = std::numbers::pi;
    static constexpr auto _1_5PI= 1.5 * PI;
    static constexpr auto _2PI = 2.0 * PI;

    // t    dir.x       dir.y
    // 0    -1(east)    -1 (middle of horizon)
    return glm::normalize(glm::vec3{
            sin((_2PI * t) + PI),
            sin((_2PI * t) + _1_5PI),
            0.0f,
    });
}
constexpr float operator ""_am(long double d){
    return d/24.0f;
}
constexpr float operator ""_pm(long double d){
    return (d+12)/24.0f;
}
SkyboxState SkyboxConfig::make_skybox()const noexcept{


    static constexpr auto MIDNIGHT0      = 0.0_am;
    static constexpr auto LATE_MIDNIGHT0 = 4.5_am;  // 0.187
    static constexpr auto DAWN           = 5.3_am;  // 0.22
    static constexpr auto LATE_DAWN      = 5.8_am;  // 0.241
    static constexpr auto PRE_SUNRISE    = 6.0_am;  // 0.25
    static constexpr auto SUNRISE        = 6.5_am;  // 0.27
    static constexpr auto LATE_SUNRISE   = SUNRISE+0.03f;
    static constexpr auto POST_SUNRISE   = SUNRISE+0.07f;
    static constexpr auto TWILIGHT= 0.8;


    static constexpr auto MIDDAY= 0.50f;
    static constexpr auto SUNSET= 0.75f;
    static constexpr auto LATE_AFTERNOON = SUNSET-0.10f;
    static constexpr auto PRE_SUNSET = SUNSET-0.05f;
    static constexpr auto LATE_SUNSET = SUNSET+0.03f;
    static constexpr auto POST_SUNSET = SUNSET+0.07f;
    static constexpr auto EARLY_MIDNIGHT1= 0.843;
    static constexpr auto MIDNIGHT1 = 1.0f;


    constexpr static auto MIDNIGHT_BLACK = rgb(00,00,15);
    constexpr static auto MIDNIGHT_BLURPLE = rgb(40,10,50);
    constexpr static auto MIDNIGHT_ORPLE = rgb(190,125,98);
    constexpr static auto MIDNIGHT_REDPLE = rgb(140,65,98);
    constexpr static auto OZONE =       rgb(110,130,195);

    constexpr static auto MIDNIGHT_BLURPLE1 = rgb(190,70,105);
    constexpr static auto MIDNIGHT_ORPLE1 = rgb(190,105,98);

    GenericRemapTable<glm::vec3> remap_t_sun_glow_color{
            {MIDNIGHT0,     rgb(255,205,110 )}, 
            {PRE_SUNRISE,   rgb(250,165,110)}, 
            {POST_SUNRISE,  rgb(255,235,190)}, 
            {MIDDAY,        rgb(255,245,225)}, 
            {PRE_SUNSET,   rgb(255,235,190)}, 
            {PRE_SUNSET,    rgb(250,205,110)}, 
            {MIDNIGHT1,     rgb(255,205,110 )}, 

    };
    GenericRemapTable<glm::vec3> remap_t_outer_sun_glow_color{
            {MIDNIGHT0,     rgb(255,205,110 )}, 
            {PRE_SUNRISE,   rgb(250,125,40)}, 
            {LATE_SUNRISE,  rgb(255,225,185)}, 
            {MIDDAY,        rgb(255,245,225)}, 
            {LATE_AFTERNOON,rgb(255,225,185)}, 
            {POST_SUNSET,   rgb(255,105,40)}, 
            {MIDNIGHT1,     rgb(255,205,110 )}, 

    };
    GenericRemapTable<glm::vec3> remap_t_sun_color{
            {MIDNIGHT0,     rgb(255,205,110 )}, 
            {SUNRISE,       rgb(255,255,210)}, 
            {POST_SUNRISE,  rgb(255,255,250)}, 
            {MIDDAY        ,rgb(255,255,250)}, 
            {LATE_AFTERNOON,rgb(255,245,230)}, 
            {SUNSET,        rgb(255,255,210)}, 
            {MIDNIGHT1,     rgb(255,205,110 )}, 
    };
    GenericRemapTable<glm::vec3> remap_t_horizon_color{
            {MIDNIGHT0     ,    0.5f * MIDNIGHT_BLACK},
            {LATE_MIDNIGHT0,    0.5f * MIDNIGHT_BLURPLE},
            {SUNRISE,           0.5f * MIDNIGHT_ORPLE},
            {POST_SUNRISE,      0.5f * rgb(115,176,235)},
            {MIDDAY        ,    0.5f * rgb(135,206,235)},
            {PRE_SUNSET,        0.5f * rgb(115,176,235)},
            {SUNSET,            0.5f * MIDNIGHT_ORPLE},
            {EARLY_MIDNIGHT1,   0.5f * MIDNIGHT_BLURPLE},
            {MIDNIGHT1     ,    0.5f * MIDNIGHT_BLACK},
    };

    GenericRemapTable<glm::vec3> remap_t_lowsky_color{
            {MIDNIGHT0     ,    MIDNIGHT_BLACK},
            {LATE_MIDNIGHT0,    MIDNIGHT_BLURPLE},
            {SUNRISE,           MIDNIGHT_ORPLE},
            {POST_SUNRISE,      rgb(115,176,235)},
            {MIDDAY        ,    rgb(135,186,235)},
            {PRE_SUNSET,        rgb(115,176,235)},
            {SUNSET,            MIDNIGHT_ORPLE},
            {EARLY_MIDNIGHT1,   MIDNIGHT_BLURPLE},
            {MIDNIGHT1     ,    MIDNIGHT_BLACK},
    };
    GenericRemapTable<glm::vec3> remap_t_zenith_color{
            {MIDNIGHT0     ,    MIDNIGHT_BLACK},
            {PRE_SUNRISE,       OZONE},
            {POST_SUNRISE,      rgb(115,176,235)},
            {MIDDAY        ,    rgb(135,186,235)},
            {PRE_SUNSET,        rgb(115,176,235)},
            {TWILIGHT,          MIDNIGHT_BLACK},
            {MIDNIGHT1     ,    MIDNIGHT_BLACK},
    };
    GenericRemapTable<glm::vec3> remap_t_midsky_color{
            {MIDNIGHT0     ,    MIDNIGHT_BLACK},
            {LATE_MIDNIGHT0,    MIDNIGHT_BLACK},
            {PRE_SUNRISE,       MIDNIGHT_BLURPLE1},
            {SUNRISE,           MIDNIGHT_BLURPLE1},
            {POST_SUNRISE,      rgb(115,176,235)},
            {MIDDAY        ,    rgb(135,186,235)},
            {PRE_SUNSET,        rgb(115,176,235)},
            {TWILIGHT,          MIDNIGHT_BLACK},
            {MIDNIGHT1     ,    MIDNIGHT_BLACK},
    };

    GenericRemapTable<f32> remap_t_belt_of_venus_intensity{
            {MIDNIGHT0     ,    0.0f},
            {LATE_MIDNIGHT0,    0.0f},
            {DAWN          ,    0.90f},
            {SUNRISE       ,    0.00f},
            {POST_SUNRISE  ,    0.0f},
            {PRE_SUNSET,        0.0f},
            {SUNSET,            0.15f},
            {TWILIGHT,          0.90f},
            {EARLY_MIDNIGHT1,    0.0f},
    };
    GenericRemapTable<f32> remap_t_horizon_hug_intensity{

            {MIDNIGHT0     ,        0.0f},
            {LATE_MIDNIGHT0,        0.0f},
            {DAWN      ,            0.70f},
            {LATE_SUNRISE       ,   0.80f},
            {POST_SUNRISE+0.5_am,   0.0f},
            {PRE_SUNSET,            0.0f},
            {LATE_SUNSET,           0.80f},
            {TWILIGHT,              0.70f},
            {MIDNIGHT1     ,        0.0f},
    };
    GenericRemapTable<f32> remap_t_glow_scale{

            {MIDNIGHT0,      0.0f},
            {LATE_MIDNIGHT0, 0.0f},
            {DAWN,           0.10f},
            {LATE_DAWN,      0.15f},
            {PRE_SUNRISE,    0.35f},
            {SUNRISE,        0.55f},
            {POST_SUNRISE,   0.55f},
            {MIDDAY,         0.45f},
            {PRE_SUNSET,     0.55f},
            {SUNSET,         0.55f},
            {POST_SUNSET,    0.35f},
            {TWILIGHT,       0.15f},
            {MIDNIGHT1,      0.0f},
    };
    GenericRemapTable<f32> remap_t_sun_intensity{

            {MIDNIGHT0,      0.0f},
            {LATE_MIDNIGHT0, 0.1f},
            {LATE_DAWN,      0.50f},
            {PRE_SUNRISE,    0.5f},
            {SUNRISE,   0.8f},
            {POST_SUNRISE,   1.0f},
            {MIDDAY,         1.0f},
            {SUNSET,         0.6f},
            {TWILIGHT,       0.20f},
            {MIDNIGHT1,      0.0f},
    };
    GenericRemapTable<f32> remap_t_star_alpha{
            {MIDNIGHT0,      1.0f},
            {LATE_MIDNIGHT0, 0.3f},
            {PRE_SUNRISE,    0.0f},
            {MIDDAY,         0.0f},
            {POST_SUNSET,    0.0f},
            {TWILIGHT,       0.3f},
            {MIDNIGHT1,      1.0f},
    };
    // modify blocks sunlight color based on 
    using glm::vec3;
    f32 t = tod01();

    // midnight = 0
    // midday = 0.5
    // midnight = 1.0
    return SkyboxState {
        .color_horizon = remap_t_horizon_color.remap(t),
        .color_sun = remap_t_sun_color.remap(t),
        .color_sun_glow = remap_t_sun_glow_color.remap(t),
        .color_outer_sun_glow = remap_t_outer_sun_glow_color.remap(t),
        .color_lowsky = remap_t_lowsky_color.remap(t),
        .color_midsky = remap_t_midsky_color.remap(t),
        .color_zenith=  remap_t_zenith_color.remap(t),
        .dir_moon = -calc_sun_direction_vec(t),
        .dir_sun = calc_sun_direction_vec(t),
        .sun_intensity_scale = remap_t_sun_intensity.remap(t),
        .belt_of_venus_intensity = remap_t_belt_of_venus_intensity.remap(t),
        .horizon_hug_intensity = remap_t_horizon_hug_intensity.remap(t),
        .star_alpha= remap_t_star_alpha.remap(t),
        .glow_scale = remap_t_glow_scale.remap(t),
    };
}
