#include "BlockLight.hpp"
inline auto unpack(PackedLightValue const& v){
    return UnpackedLightValue{
         (v.get_blocklight_r()),
         (v.get_blocklight_g()),
         (v.get_blocklight_b()),
         (v.get_sunlight()),
    };
}
inline auto unpack_blocklight(PackedLightValue const& v){
    return BlockLight{
         .r = v.get_blocklight_r(),
         .g = v.get_blocklight_g(),
         .b = v.get_blocklight_b(),
    };
}
inline auto unpack_sunlight(PackedLightValue const& v){
    return v.get_sunlight();
}
inline auto pack(UnpackedLightValue const& v){
    return PackedLightValue{
        v.r,v.g,v.b,v.s
    };
}
