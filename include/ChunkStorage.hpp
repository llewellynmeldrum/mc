#pragma once 

#include <concepts/concepts.hpp>
#include <filesystem>
#include <mdspan>
#include <utility>

#include "ChunkConstants.hpp"
#include "CoordIteration.hpp"
#include "CoordTypes.hpp"
#include "PendingBlockWrites.hpp"
#include "SharedShaderConfig.hpp"
#include "BlockLight.hpp"
#include "cppslop.hpp"
#include "Assertion.hpp"
#include "ChunkView.hpp"

#include "GenericChunkStore.hpp"
#include "PackedLightValue.hpp"

using ChunkBlockStore = GenericChunkStore<Block>;
using ChunkLightStore = GenericChunkStore<PackedLightValue>;
static_assert(!array_like<ChunkBlockStore>);
static_assert(map_like<ChunkBlockStore>);

// A slice can be taken out of a 
enum struct SliceType{
    X,
    Y,
    Z
};
// NOTE: this class is kinda gross. But it does save quite a bit of memory when meshing
template<typename V>
struct GenericChunkSlice{
public:
    using mapped_type = V;
private:
    std::vector<mapped_type> buf={};
    i32 locked_axis_val{};
public:
    bool is_empty{false};
    using key_type = ChunkBlockPos;
    using value_type = V;

    GenericChunkSlice(nullptr_t null_ctor):is_empty(true){}
    GenericChunkSlice(GenericChunkView<const mapped_type> src, SliceType _slice_type, ChunkBlockPos p1, ChunkBlockPos p2) 
        : slice_type(_slice_type) 
    {
        switch (slice_type){
            case SliceType::X:
                buf.resize(ChunkInfo::HEIGHT * ChunkInfo::ZWIDTH);
                locked_axis_val = p1.x;
                assert_eq(p1.x+1,p2.x);
            break;

            case SliceType::Y:
                buf.resize(ChunkInfo::XWIDTH * ChunkInfo::ZWIDTH);
                locked_axis_val = p1.y;
                assert_eq(p1.y+1,p2.y);
            break;

            case SliceType::Z:
                buf.resize(ChunkInfo::XWIDTH * ChunkInfo::HEIGHT);
                locked_axis_val = p1.z;
                assert_eq(p1.z+1,p2.z);
            break;
        };
        // copy all of them in 
        ForEachInRangeEx(p1, p2,[&](i32 cx, i32 cy, i32 cz){
            this->at(cx,cy,cz) = src.at(cx,cy,cz);
        });
    }
    ~GenericChunkSlice() = default;
    SliceType slice_type;


    auto data(this auto& self){
        return self.buf.data(); 
    }


    constexpr decltype(auto) at(this auto& self, i32 cx, i32 cy, i32 cz) {
        assert(!self.is_empty);
        switch(self.slice_type){
            case SliceType::X:
                assert_eq(cx,self.locked_axis_val);
                return self.at_2d(cy,cz);
            break;
            case SliceType::Y:
                assert_eq(cy,self.locked_axis_val);
                return self.at_2d(cx,cz);
            break;
            case SliceType::Z:
                assert_eq(cz,self.locked_axis_val);
                return self.at_2d(cx,cy);
            break;
        }
    }
    constexpr decltype(auto) at(this auto& self, ChunkBlockPos p) {
        return self.at(p.x,p.y,p.z);
    }

    // NOTE: 
    // Required to satisfy map_like concept
    size_t size() const noexcept{
        switch(slice_type) {
            case SliceType::X: return ChunkInfo::HEIGHT * ChunkInfo::ZWIDTH; break;
            case SliceType::Y: return ChunkInfo::XWIDTH * ChunkInfo::ZWIDTH; break;
            case SliceType::Z: return ChunkInfo::XWIDTH * ChunkInfo::HEIGHT; break;
        }
    }

    constexpr bool contains(ChunkBlockPos p)const noexcept{
        assert(!is_empty);
        switch(slice_type){
            case SliceType::X:
                return  p.x == locked_axis_val && 
                        0 <= p.y && p.y < ChunkInfo::Extents3D.y && 
                        0 <= p.z && p.z < ChunkInfo::Extents3D.z ;
            break;

            case SliceType::Y:
                return  0 <= p.x && p.x < ChunkInfo::Extents3D.x && 
                        p.y == locked_axis_val && 
                        0 <= p.z && p.z < ChunkInfo::Extents3D.z ;
            break;

            case SliceType::Z:
                return  0 <= p.x && p.x < ChunkInfo::Extents3D.x && 
                        0 <= p.y && p.y < ChunkInfo::Extents3D.y &&
                        p.z == locked_axis_val;
            break;
        }
    }
private:
    constexpr decltype(auto) span2d(this auto& self){
        switch(self.slice_type){
        case SliceType::X:
            return std::mdspan(self.data(), ChunkInfo::HEIGHT, ChunkInfo::ZWIDTH);
            break;

        case SliceType::Y:
            return std::mdspan(self.data(), ChunkInfo::XWIDTH, ChunkInfo::ZWIDTH);
            break;

        case SliceType::Z:
            return std::mdspan(self.data(), ChunkInfo::XWIDTH, ChunkInfo::HEIGHT);
            break;
        }
    }
    // eg: if an SliceType::X, a==y, b==z
    constexpr decltype(auto) at_2d(this auto& self, i32 a, i32 b) {
        return self.span2d()[a,b];
    }
};

using ChunkBlockSlice = GenericChunkSlice<Block>;
using ChunkLightSlice = GenericChunkSlice<PackedLightValue>;

