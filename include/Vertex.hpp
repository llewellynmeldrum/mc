#pragma once
#include <span>
#include <string_view>
#include <type_traits>
#include "Block.hpp"
#include "CoordTypes.hpp"
#include "Types.h"
#include "glmWrapper.hpp"
#include "AttributeTraits.hpp"
#include "Assertion.hpp"
#include "Bitwise.hpp"
#include "SharedShaderConfig.hpp"





struct Vertex {
    glm::vec3 pos;
    glm::vec2 txCoords;
    u32 packed_0{0};

    constexpr Vertex(
        glm::vec3 pos, 
        glm::vec2 txCoords,
        i32 face_direction,
        BlockShape block_shape
    )
        : pos(pos)
        , txCoords(txCoords)
    {
        i32 tex_atlas_id = block_shape_to_texture_atlas.at(block_shape);
        set_val(packed_0, face_dir_mask, face_dir_offset, face_direction);
        set_val(packed_0, tex_atlas_id_mask, tex_atlas_id_offset, tex_atlas_id);
    }

    constexpr void set_sunlight_intensity(u8 val) noexcept{
        set_val(packed_0, sunlight_intensity_mask, sunlight_intensity_offset, val);
    }

    constexpr void set_blocklight_r(u8 val) noexcept{
        set_val(packed_0, blocklight_r_mask, blocklight_r_offset, val);
    }

    constexpr void set_blocklight_g(u8 val) noexcept{
        set_val(packed_0, blocklight_g_mask, blocklight_g_offset, val);
    }

    constexpr void set_blocklight_b(u8 val) noexcept{
        set_val(packed_0, blocklight_b_mask, blocklight_b_offset, val);
    }

    constexpr void set_face_dir(u8 val) noexcept{
        set_val(packed_0, face_dir_mask, face_dir_offset, val);
    }

    constexpr void set_face_opacity(u8 val) noexcept{
        set_val(packed_0, face_opacity_mask, face_opacity_offset, val);
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
               make_attr<glm::vec2>(1, offsetof(Vertex, txCoords)),       // 08 bytes (4*2)
               make_attr<u32>      (2, offsetof(Vertex, packed_0)),       // 4 bytes ()
            }, 
        };
    }
};
static_assert(std::is_standard_layout_v<Vertex>,
              "Must be true for valid use of offsetof() in vtx attributes");
static_assert(std::is_trivially_copyable_v<Vertex>, "Must be true for upload to vertex buffer");

