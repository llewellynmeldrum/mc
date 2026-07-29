#pragma once 
#include "Types.h"
#include <string_view>
#include "MetaUtils.hpp"
namespace refl{
#define SRC_LOC_CURRENT()\
    refl::source_location{\
        ._file=__FILE_NAME__,\
        ._function=__FUNCTION__,\
        ._pretty_fn=__PRETTY_FUNCTION__,\
        ._line=__LINE__,\
    }
// The type must be formatted at the callsite, 
// so then this header doesnt need to pull in the formatter overloads 
#define MAKE_VAR(val) refl::make_var(val,std::format("{}",val),pretty_type_name<decltype(val)>(),#val)

#define MAKE_VAR_NO_VAL(val)\
    refl::variable{\
        .type_str = pretty_type_name<decltype(val)>(),\
        .val_str = "",\
        .name_str= #val,\
    }

struct variable{
    std::string_view type_str;
    std::string_view  val_str;
    std::string_view  name_str;
    std::size_t size_bytes;
    const void* addr;
    const auto& val(){return val_str;}
    const auto& type(){return type_str;}
    const auto& name(){return name_str;}

    bool is_integral()const noexcept;
};

template<typename T>
concept is_addressable = requires(T&& x){
    { &x } -> std::same_as<std::remove_reference_t<T>*>;
};

template<typename T>
constexpr const void* resolve_var_addr(T&& val){
    if constexpr (is_addressable<T>){
        return &val;
    }else {
        return nullptr;
    }
}

template<typename T>
constexpr refl::variable make_var(T&& val, std::string_view s_val, std::string_view s_type, std::string_view s_name){
    return refl::variable{
        .type_str = s_type,
        .val_str = s_val,
        .name_str= s_name,
        .size_bytes = sizeof(val),
        .addr = resolve_var_addr(std::forward<T>(val)),
    };
}

enum struct ContType: i8{
    ARRAY_LIKE,
    MAP_LIKE,
};
struct source_location{
    const char* _file;
    const char* _function;
    const char* _pretty_fn;
    int _line;
    int _col{0};
    bool valid=true;
    const auto& file_name(){return _file;}
    const auto& function_name(){return _function;}
    const auto& pretty_fn(){return _pretty_fn;}
    const auto& line(){return _line;}
    const auto& column(){return _col;}
};
}//namespace refl
