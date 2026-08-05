#include "SkyboxState.hpp"
#include <numbers>


constexpr glm::vec3 calc_sun_direction_vec(f32 t){
    static constexpr auto PI = std::numbers::pi;
    static constexpr auto _1_5PI= 1.5 * PI;
    static constexpr auto _2PI = 2.0 * PI;
    return glm::normalize(glm::vec3{
            sin((_2PI * t) + PI),
            sin((_2PI * t) + _1_5PI),
            0.0f,
    });
}
SkyboxState SkyboxConfig::make_skybox()const noexcept{
    static constexpr auto MIDNIGHT0= 0.0f;
    static constexpr auto SUNRISE = 0.25f;
    static constexpr auto MIDDAY= 0.50f;
    static constexpr auto SUNSET= 0.75f;
    static constexpr auto MIDNIGHT1 = 1.0f;

    static constexpr auto PRE_SUNRISE = SUNRISE-0.05f;
    static constexpr auto POST_SUNRISE = SUNRISE+0.05f;
    GenericRemapTable<glm::vec3> remap_t_sun_color{
            {MIDNIGHT0,   rgb(10 ,10,5  )},
            {SUNRISE,     rgb(255,215,80 )}, 
            {MIDDAY,      rgb(255,255,240)}, 
            {SUNSET,      rgb(255,195,170)}, 
            {MIDNIGHT1,   rgb(10 ,10,5  )},
    };
    GenericRemapTable<glm::vec3> remap_t_zenith_color{
            {MIDNIGHT0,     rgb(10 ,10 ,5  )},
            {SUNRISE,       rgb(150,90,  0)},
            {POST_SUNRISE,  rgb(130,120, 60)},
            {MIDDAY,        rgb(135,206,235)},
            {SUNSET,        rgb(130, 90, 60)},
            {MIDNIGHT1,     rgb(10 ,10 ,5  )},
    };

    GenericRemapTable<f32> remap_t_glow_scale{
            {MIDNIGHT0,    0.0f},
            {SUNRISE,      1.5f},
            {MIDDAY,       1.0f},
            {SUNSET,       1.5f},
            {MIDNIGHT1,    0.0f},
    };
    using glm::vec3;
    f32 t = tod01();

    // midnight = 0
    // midday = 0.5
    // midnight = 1.0
    return SkyboxState {
        .color_horizon = rgb(10 ,10 ,5  ),
        .color_sun = remap_t_sun_color.remap(t),
        .color_lowsky = rgb(135,49,105),
        .color_midsky = rgb(190,110,0),
        .color_zenith= rgb(135,206,235),
//        .color_zenith= remap_t_zenith_color.remap(t),
        .dir_sun = calc_sun_direction_vec(t),
        .glow_scale = remap_t_glow_scale.remap(t)
    };
}
