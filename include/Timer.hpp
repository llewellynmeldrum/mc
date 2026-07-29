#pragma once 
#include "Types.h"
namespace timer{
    using impl_clock = std::chrono::steady_clock;
    using duration = impl_clock::duration;
    using time_point = impl_clock::time_point;

    template<typename T>
    using nano_duration = std::chrono::duration<T, std::nano>;

    using Repr_t = f64;



    using s_period = std::ratio<1>;
    using millis_period = std::milli;
    using nanos_period = std::nano;
    using micros_period = std::micro;

    [[nodiscard]]
    inline time_point now(){
        return impl_clock::now();
    }


    template<typename Period>
    [[nodiscard]]
    constexpr duration to_duration(Repr_t dur){
        return std::chrono::duration_cast<duration>(std::chrono::duration<Repr_t,Period>{dur});
    }

    template<typename Period>
    [[nodiscard]]
    constexpr Repr_t from_duration(duration dur){
        return std::chrono::duration<Repr_t, Period>(dur).count();
    }

    [[nodiscard]]
    constexpr duration milliseconds(f64 f){
        return to_duration<millis_period>(f);
    }

    [[nodiscard]]
    constexpr duration seconds(f64 f){
        return to_duration<s_period>(f);
    }

    [[nodiscard]]
    inline Repr_t get_seconds(duration dur){
        return from_duration<s_period>(dur);
    }
    [[nodiscard]]
    inline Repr_t get_milliseconds(duration dur){
        return from_duration<millis_period>(dur);
    }

}
