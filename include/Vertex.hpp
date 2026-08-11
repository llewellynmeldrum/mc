#pragma once
#include <span>
#include <string_view>
#include <type_traits>
#include "Block.hpp"
//#include "BlockAmbientOcclusion.hpp"
#include "CoordTypes.hpp"
#include "PackedLightValue.hpp"
#include "Types.h"
#include "glmWrapper.hpp"
#include "AttributeTraits.hpp"
#include "Assertion.hpp"
#include "Bitwise.hpp"
#include "SharedShaderConfig.hpp"
#include "meta_wrapper.hpp"
#include "BitField.hpp"

#include "AONeighbours.hpp"



struct Vertex {
    glm::vec3 pos;
    glm::vec2 tx_coords;
    using storage_type = u32;
    storage_type store{0};
    // sunlight_intensity ->  00000000000000000000000000001111 : 0-15
    // blocklight_r       ->  00000000000000000000000011110000
    // blocklight_g       ->  00000000000000000000111100000000
    // blocklight_b       ->  00000000000000001111000000000000
    // face_dir           ->  00000000000001110000000000000000
    // tex_atlas_id       ->  00000000001110000000000000000000 
    // face_opacity       ->  00111111000000000000000000000000 : 0-64
    // ao_state->  11000000000000000000000000000000 : 0-3

    // ao has 4 states
    // none -> 00
    // a|b ->  10
    // c   ->  01
    // a&b ->  11
 //   constexpr void set_ao_state(AONeighbours const& n);
//    constexpr u2 pack_ao_state(AONeighbours const& n);

//    storage_type store{0};
    #include "Packed0.def"
//    BITFIELD_MEMBER(sunlight_intensity, 0,  4L, v/15.0)
//    BITFIELD_MEMBER(blocklight_r,       4,  4L, v/15.0)
//    BITFIELD_MEMBER(blocklight_g,       8,  4L, v/15.0)
//    BITFIELD_MEMBER(blocklight_b,       12, 4L, v/15.0)
//    BITFIELD_MEMBER(face_dir,           16, 3L, v     )
//    BITFIELD_MEMBER(tex_atlas_id,       19, 3L, v     )
//    BITFIELD_MEMBER(face_opacity,       24, 6L, v/63.0)
//    BITFIELD_MEMBER(ao_state,           30, 2L, v     )


    static constexpr auto lighting_mask = sunlight_intensity.mask | blocklight_r.mask | blocklight_g.mask | blocklight_b.mask;
    constexpr void copy_light_data(PackedLightValue val) noexcept{
        store = (store & (~lighting_mask)) | (val.store & (lighting_mask));
    };

    constexpr void offset_by_chunk_pos(ChunkBlockPos local_pos)noexcept{
        pos.x += local_pos.x;
        pos.y += local_pos.y;
        pos.z += local_pos.z;
    }


};
template<BlockShape block_shape>
constexpr auto make_vtx(
    glm::vec3 _pos, 
    glm::vec2 _tx_coords,
    i32 face_direction
)   -> Vertex
{
    
    Vertex res{
        .pos = _pos,
        .tx_coords = _tx_coords,
    };
    res.set_face_dir(face_direction);
    res.set_tex_atlas_id(shape_atlas_id<block_shape>);
    return res;
}




static_assert(std::is_standard_layout_v<Vertex>,
              "Must be true for valid use of offsetof() in vtx attributes");
static_assert(std::is_trivially_copyable_v<Vertex>, "Must be true for upload to vertex buffer");

