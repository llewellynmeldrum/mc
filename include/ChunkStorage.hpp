#pragma once 

#include <concepts/concepts.hpp>
#include <mdspan>
#include <utility>

#include "Block.hpp"
#include "ChunkConstants.hpp"
#include "CoordIteration.hpp"
#include "CoordTypes.hpp"
#include "PendingBlockWrites.hpp"
#include "SharedShaderConfig.hpp"
#include "cppslop.hpp"
#include "Assertion.hpp"
#include "ChunkView.hpp"

#include "GenericChunkStore.hpp"

struct BlockLightData{
    u16 packed_data{0};

    void incr_sunlight_intensity()noexcept{
		set_sunlight_intensity(get_sunlight_intensity()+1);
	}
    void incr_blocklight_r()noexcept{
		set_blocklight_r(get_blocklight_r()+1);
	}
    void incr_blocklight_g()noexcept{
		set_blocklight_g(get_blocklight_g()+1);
	}
    void incr_blocklight_b()noexcept{
		set_blocklight_b(get_blocklight_b()+1);
	}

    void decr_sunlight_intensity()noexcept{
		set_sunlight_intensity(get_sunlight_intensity()-1);
	}
    void decr_blocklight_r()noexcept{
		set_blocklight_r(get_blocklight_r()-1);
	}
    void decr_blocklight_g()noexcept{
		set_blocklight_g(get_blocklight_g()-1);
	}
    void decr_blocklight_b()noexcept{
		set_blocklight_b(get_blocklight_b()-1);
	}

    void set_sunlight_intensity(u8 val) noexcept{
        return ::set_sunlight_intensity(packed_data,val);
    }
    void set_blocklight_r(u8 val) noexcept {
        return ::set_blocklight_r(packed_data,val);
    }
    void set_blocklight_g(u8 val) noexcept {
        return ::set_blocklight_g(packed_data,val);
    }
    void set_blocklight_b(u8 val) noexcept {
        return ::set_blocklight_b(packed_data,val);
    }

    u8 get_sunlight_intensity() const noexcept{
        return UNPACK_BITFIELD_MEMBER(packed_data, sunlight_intensity_mask,sunlight_intensity_offset);
    }
    u8 get_blocklight_r() const noexcept {
        return UNPACK_BITFIELD_MEMBER(packed_data, blocklight_r_mask,blocklight_r_offset);
    }
    u8 get_blocklight_g() const noexcept {
        return UNPACK_BITFIELD_MEMBER(packed_data, blocklight_g_mask,blocklight_g_offset);
    }
    u8 get_blocklight_b() const noexcept {
        return UNPACK_BITFIELD_MEMBER(packed_data, blocklight_b_mask,blocklight_b_offset);
    }
};
using ChunkBlockStore = GenericChunkStore<Block>;
using ChunkLightStore = GenericChunkStore<BlockLightData>;
static_assert(!array_like<ChunkBlockStore>);
static_assert(map_like<ChunkBlockStore>);

// A slice can be taken out of a 
enum struct SliceType{
    X,
    Y,
    Z
};
// NOTE: this class is kinda gross. But it does save quite a bit of memory when meshing
struct ChunkSlice2D{
private:
    std::vector<Block> buf={};
    i32 locked_axis_val{};
public:
    ChunkSlice2D(ConstChunkView src, SliceType _slice_type, ChunkBlockPos pos1, ChunkBlockPos pos2);
    ~ChunkSlice2D() = default;
    using key_type = ChunkBlockPos;
    using mapped_type = Block;
    size_t size()const noexcept{
        if (slice_type == SliceType::X){
             return ChunkInfo::HEIGHT * ChunkInfo::ZWIDTH;
        }else if (slice_type == SliceType::Y){
             return ChunkInfo::XWIDTH * ChunkInfo::ZWIDTH;
        }else{
             return ChunkInfo::XWIDTH * ChunkInfo::HEIGHT;
        }
    }

    constexpr bool contains(ChunkBlockPos p)const noexcept{
        if (slice_type == SliceType::X){
            return  
                    p.x == locked_axis_val && 
                    0 <= p.y && p.y < ChunkInfo::Extents3D.y && 
                    0 <= p.z && p.z < ChunkInfo::Extents3D.z ;
        }else if (slice_type == SliceType::Y){
            return  
                    0 <= p.x && p.x < ChunkInfo::Extents3D.x && 
                    p.y == locked_axis_val && 
                    0 <= p.z && p.z < ChunkInfo::Extents3D.z ;
        }else{
            return  
                    0 <= p.x && p.x < ChunkInfo::Extents3D.x && 
                    0 <= p.y && p.y < ChunkInfo::Extents3D.y &&
                    p.z == locked_axis_val;
        }
    }

    SliceType slice_type;
    auto data(this auto& self){
        return self.buf.data(); 
    }


    constexpr decltype(auto) at(this auto& self, i32 cx, i32 cy, i32 cz) {
        if (self.slice_type == SliceType::X){
            assert_eq(cx,self.locked_axis_val);
            return self.at_2d(cy,cz);
        }else if (self.slice_type == SliceType::Y){
            assert_eq(cy,self.locked_axis_val);
            return self.at_2d(cx,cz);
        }else {
            assert_eq(cz,self.locked_axis_val);
            return self.at_2d(cx,cy);
        }
    }
    constexpr decltype(auto) at(this auto& self, ChunkBlockPos p) {
        if (self.slice_type == SliceType::X){
            assert_eq(p.x,self.locked_axis_val);
            return self.at_2d(p.y,p.z);
        }else if (self.slice_type == SliceType::Y){
            assert_eq(p.y,self.locked_axis_val);
            return self.at_2d(p.x,p.z);
        }else {
            assert_eq(p.z,self.locked_axis_val);
            return self.at_2d(p.x,p.y);
        }
    }
private:
    constexpr decltype(auto) span2d(this auto& self){
        if (self.slice_type == SliceType::X){
            return std::mdspan(self.data(), ChunkInfo::HEIGHT, ChunkInfo::ZWIDTH);
        }else if (self.slice_type == SliceType::Y){
            return std::mdspan(self.data(), ChunkInfo::XWIDTH, ChunkInfo::ZWIDTH);
        }else{
            return std::mdspan(self.data(), ChunkInfo::XWIDTH, ChunkInfo::HEIGHT);
        }
    }
    // eg: if an SliceType::X, a==y, b==z
    constexpr decltype(auto) at_2d(this auto& self, i32 a, i32 b) {
        return self.span2d()[a,b];
    }
};

