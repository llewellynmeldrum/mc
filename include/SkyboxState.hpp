#pragma once 
#include "Types.h"
#include "Types.hpp"
#include "glm_math_extensions.hpp"
struct SkyboxState{
    using vec3 = glm::vec3;

    vec3 color_horizon;     // color of the sky at the horizon line (~y=0)
    vec3 color_sun;
    vec3 color_sun_glow;
    vec3 color_outer_sun_glow;
    vec3 color_lowsky;      // color of the sky ~y=0.3
    vec3 color_midsky;      // color of the sky ~y=0.5
    vec3 color_zenith;      // color of the sky ~y=1.0

    vec3 dir_moon;
    vec3 dir_sun;
    f32 sun_intensity_scale;
    f32 belt_of_venus_intensity;
    f32 horizon_hug_intensity;
    f32 star_alpha;
    f32 glow_scale;
    glm::vec3 get_casted_sun_color(){
        vec3 sun_color = color_sun*color_sun_glow;
        vec3 sky_color = color_lowsky * color_midsky*color_zenith;
        return glm::mix(sky_color, sun_color,sun_intensity_scale);
    }
};

struct SkyboxConfig{
    TickCount ticks_per_day{};
    TickCount tick_count{};

    static constexpr auto MIDNIGHT0= 0.0f;
    static constexpr auto SUNRISE = 0.25f;
    static constexpr auto MIDDAY= 0.50f;
    static constexpr auto SUNSET= 0.75f;
    static constexpr auto MIDNIGHT1 = 1.0f;

    static constexpr auto PRE_SUNRISE = SUNRISE-0.05f;
    static constexpr auto POST_SUNRISE = SUNRISE+0.05f;
    GenericRemapTable<glm::vec3> remap_t_sun_color{
            {MIDNIGHT0,   rgb(10 ,10,5  )},
            {SUNRISE,     rgb(255,215,170)}, 
            {MIDDAY,      rgb(255,255,240)}, 
            {SUNSET,      rgb(255,195,170)}, 
            {MIDNIGHT1,   rgb(10 ,10,5  )},
    };
    GenericRemapTable<glm::vec3> remap_t_zenith_color{
            {MIDNIGHT0,     rgb(10 ,10 ,5  )},
            {SUNRISE,       rgb(150,60,  0)},
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



    SkyboxState make_skybox() const noexcept;
    consteval static inline glm::vec3 rgb(auto r, auto g, auto b) {
        return glm::vec3{r,g,b}/255.0f;
    };
    constexpr auto ticks_to_tod01(TickCount p_tick_count)const noexcept{
        return static_cast<f32>(p_tick_count % ticks_per_day) / ticks_per_day;
    }
    constexpr auto tod01()const noexcept{
        return ticks_to_tod01(tick_count);
    }
    constexpr auto tod01_to_tick_count(f32 p_tod01){
        return p_tod01 * ticks_per_day;
    }
};
