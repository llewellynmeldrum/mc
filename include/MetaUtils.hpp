#pragma once 
template <typename T>
static inline constexpr std::string_view pretty_type_name() {
#if !defined(__cplusplus)
#error "non c++ context, tf compiler are you using"
#else
    #if __cplusplus == 202603L
        #warning "pretty_type_name does not work as intended on gcc16 with c++26 mode enabled."
        // Actually a very convinient time for this to break, as this is the update with reflection
        // So the entire need for this fuckery is erased by that update. Nice timing.
    #endif
#endif

#if defined(__clang__)
    std::string_view p = __PRETTY_FUNCTION__;
    auto             start = p.find("T = ");
    start += 4;
    auto end = p.rfind(']');
    return p.substr(start, end - start);

#elif defined(__GNUC__)
    std::string_view p = __PRETTY_FUNCTION__;
    auto             start = p.find("with T = ");
    start += 9;
    auto end = p.find(';', start);
    return p.substr(start, end - start);

#else
    return "unsupported compiler";
#endif
}
