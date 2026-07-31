#include "BlockLight.hpp"
#include "PackedLightValue.hpp"
UnpackedLightValue unpack(PackedLightValue const& v){
    return UnpackedLightValue{
         (v.get_blocklight_r()),
         (v.get_blocklight_g()),
         (v.get_blocklight_b()),
         (v.get_sunlight()),
    };
}
BlockLight unpack_blocklight(PackedLightValue const& v){
    return BlockLight{
         v.get_blocklight_r(),
         v.get_blocklight_g(),
         v.get_blocklight_b(),
    };
}
SunLight unpack_sunlight(PackedLightValue const& v){
    return v.get_sunlight();
}
