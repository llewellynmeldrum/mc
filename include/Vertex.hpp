#pragma once
#include <span>
#include <string_view>
#include <type_traits>
#include "Block.hpp"
#include "CoordTypes.hpp"
#include "PackedLightValue.hpp"
#include "Types.h"
#include "glmWrapper.hpp"
#include "AttributeTraits.hpp"
#include "Assertion.hpp"
#include "Bitwise.hpp"
#include "SharedShaderConfig.hpp"





struct Vertex {
    glm::vec3 pos;
    glm::vec2 tx_coords;
    u32 packed_0{0};
    // sunlight_intensity ->  00000000000000000000000000001111
    // blocklight_r       ->  00000000000000000000000011110000
    // blocklight_g       ->  00000000000000000000111100000000
    // blocklight_b       ->  00000000000000001111000000000000
    // face_dir           ->  00000000000001110000000000000000
    // tex_atlas_id       ->  00000000001110000000000000000000
    // face_opacity       ->  11111111000000000000000000000000

    constexpr Vertex(
        glm::vec3 pos, 
        glm::vec2 txCoords,
        i32 face_direction,
        BlockShape block_shape
    )
        : pos(pos)
        , tx_coords(txCoords)
    {
        i32 tex_atlas_id = block_shape_to_texture_atlas.at(block_shape);
        SET_BITFIELD_MEMBER_VAL(packed_0, FACE_DIR_MASK, FACE_DIR_OFFSET, face_direction);
        SET_BITFIELD_MEMBER_VAL(packed_0, TEX_ATLAS_ID_MASK, TEX_ATLAS_ID_OFFSET, tex_atlas_id);
    }

    constexpr void copy_light_data(PackedLightValue val) noexcept{
        packed_0 = (packed_0 & (~LIGHTING_MASK)) | (val.packed_data & (LIGHTING_MASK));
    };
    constexpr void set_sunlight_intensity(u8 val) noexcept{
        SET_BITFIELD_MEMBER_VAL(packed_0, SUNLIGHT_INTENSITY_MASK, SUNLIGHT_INTENSITY_OFFSET, val);
    }

    constexpr void set_blocklight_r(u8 val) noexcept{
        SET_BITFIELD_MEMBER_VAL(packed_0, BLOCKLIGHT_R_MASK, BLOCKLIGHT_R_OFFSET, val);
    }

    constexpr void set_blocklight_g(u8 val) noexcept{
        SET_BITFIELD_MEMBER_VAL(packed_0, BLOCKLIGHT_G_MASK, BLOCKLIGHT_G_OFFSET, val);
    }

    constexpr void set_blocklight_b(u8 val) noexcept{
        SET_BITFIELD_MEMBER_VAL(packed_0, BLOCKLIGHT_B_MASK, BLOCKLIGHT_B_OFFSET, val);
    }

    constexpr void set_face_dir(u8 val) noexcept{
        SET_BITFIELD_MEMBER_VAL(packed_0, FACE_DIR_MASK, FACE_DIR_OFFSET, val);
    }

    constexpr void set_face_opacity(u8 val) noexcept{
        SET_BITFIELD_MEMBER_VAL(packed_0, FACE_OPACITY_MASK, FACE_OPACITY_OFFSET, val);
    }

    constexpr void offset_by_chunk_pos(ChunkBlockPos local_pos)noexcept{
        pos.x += local_pos.x;
        pos.y += local_pos.y;
        pos.z += local_pos.z;
    }


    static constexpr auto layout() {
        return VertexLayout<3>{ 
            .stride = sizeof(Vertex),
            .attrs = {
               make_attr<glm::vec3>(0, offsetof(Vertex, pos)),            // 12 bytes (4*3)
               make_attr<glm::vec2>(1, offsetof(Vertex, tx_coords)),       // 08 bytes (4*2)
               make_attr<u32>      (2, offsetof(Vertex, packed_0)),       // 4 bytes ()
            }, 
        };
    }
};
static_assert(std::is_standard_layout_v<Vertex>,
              "Must be true for valid use of offsetof() in vtx attributes");
static_assert(std::is_trivially_copyable_v<Vertex>, "Must be true for upload to vertex buffer");

