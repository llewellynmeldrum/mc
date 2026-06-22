#pragma once 
#include "Types.h"
#include <string_view>
namespace refl{
#define SRC_LOC_CURRENT()\
    refl::source_location{\
        ._file=__FILE_NAME__,\
        ._function=__FUNCTION__,\
        ._pretty_fn=__PRETTY_FUNCTION__,\
        ._line=__LINE__,\
    }
#define MAKE_VAR(val)\
    refl::variable{\
        .type_str = pretty_type_name<decltype(val)>(),\
        .val_str = std::format("{}",val),\
        .name_str= #val,\
    }
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
    const auto& val(){return val_str;}
    const auto& type(){return type_str;}
    const auto& name(){return name_str;}
};
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
