#include "BlockLight.hpp"
#include "PackedLightValue.hpp"
UnpackedLightValue unpack(PackedLightValue const& v){
    return UnpackedLightValue{
         static_cast<u8>((v.get_blocklight_r())),
         static_cast<u8>((v.get_blocklight_g())),
         static_cast<u8>((v.get_blocklight_b())),
         static_cast<u8>((v.get_sunlight_intensity())),
    };
}
BlockLight unpack_blocklight(PackedLightValue const& v){
    return BlockLight{
         static_cast<u8>(v.get_blocklight_r()),
         static_cast<u8>(v.get_blocklight_g()),
         static_cast<u8>(v.get_blocklight_b()),
    };
}
SunLight unpack_sunlight(PackedLightValue const& v){
    return v.get_sunlight_intensity();
}
