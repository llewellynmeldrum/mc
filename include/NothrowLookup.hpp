#pragma once 
#include "Assertion.hpp"
#include "Breakpoints.hpp"
#include "refl.hpp"
#include "CommonConcepts.hpp"
#include "CommonUtils.hpp"
#include <type_traits>
#include <utility>
#include <print>
// BRIEF:
// Exception safe `.at()` replacement for most STL containers.
// These macros capture type info, caller source location, container name, etc,
// and present them in a console message, in the case of an out-of-bounds access 
//      (or entry doesnt exist in the case of an associative container)
// Overhead is around 2-3x .at() or operator[] from my benchmarks. Worth it for the time being,
//  (and its a macro so very easy to reroute to regular .at() in real builds.)
#define AT(c, i) at_safe_noexcept(c,i, #c, #i, SRC_LOC_CURRENT())
#define AT_EXCEPT(c, i) at_safe(c,i, #c, #i, SRC_LOC_CURRENT())

template<typename C, typename K> 
[[gnu::always_inline]] inline bool inbounds(C& cont, K key){
    if constexpr(array_like<C>){
        static_assert(std::is_integral_v<K>);
        return (key>=0 && key<static_cast<decltype(key)>(cont.size()));
    } else if constexpr(map_like<C>){
        return cont.contains(key);
    } else{
        // ERROR: 
        // add a specialization for my hashmap
        std::println(stderr, "Container type {} is neither MapLike nor ArrayLike. Check CommonConcepts.hpp",pretty_type_name(cont));
        BREAKPOINT();
        return false;
    }
    return false;
}
// replace all instances of .at() with bounds checked/exception handling AT().

template<typename T>
[[gnu::always_inline]]inline auto make_variable(const T& var, std::string_view name){
    std::string var_str{"?"};
    if constexpr(std::formattable<T,char>){ 
        var_str = std::format("{}",var);
    }
    return refl::variable{
        .type_str = pretty_type_name<T>(),
        .val_str =  var_str,
        .name_str= name,
    };
}
// NOTE:
// we make this cold and noinline, to keep the actual checking function smaller (since its always inline.)
// That way, there is less cost in the cold path.
template<typename C, typename K>
[[gnu::cold, gnu::noinline,noreturn]]
inline void report_oor_intermediate(C& cont, K key,std::string_view cont_name, std::string_view key_name, refl::source_location loc){
        i64 sz = cont.size();
        if constexpr(map_like<C>){ 
            sz=-1;
        }
        report_OOR(
            sz, "Index out of bounds!", 
            make_variable(cont,cont_name),
            make_variable(key,key_name),
            loc
        );
}
template<typename C, typename K>
[[gnu::always_inline]] inline decltype(auto) at_safe(C& cont, K key,std::string_view cont_name, std::string_view key_name, refl::source_location loc){
    try {
        return cont.at(key);
    } catch(std::out_of_range&){
        report_oor_intermediate(cont,key,cont_name,key_name,loc);
        throw;
    };
}

template<typename C, typename K>
requires std::is_const_v<C>
[[gnu::always_inline]] inline decltype(auto) at_safe_noexcept(C& cont, K key, std::string_view cont_name, std::string_view key_name, refl::source_location loc){
    if (inbounds(cont,key)){
        return cont.at(key);
    }else[[unlikely]]{
        report_oor_intermediate(cont,key,cont_name,key_name, loc);
        return cont.at(key);
    }
}
template<typename C, typename K>
requires (!std::is_const_v<C>)
[[gnu::always_inline]] inline decltype(auto) at_safe_noexcept(C& cont, K key, std::string_view cont_name, std::string_view key_name, refl::source_location loc){
    if (inbounds(cont,key)){
        return cont[key];
    }else[[unlikely]]{
        report_oor_intermediate(cont,key,cont_name,key_name, loc);
        return cont[key];
    }
}





