#pragma once 

#include <mdspan>
#include <utility>
#include "Block.hpp"
#include "ChunkConstants.hpp"
#include "CoordIteration.hpp"
#include "CoordTypes.hpp"
#include "PendingBlockWrites.hpp"
#include "cppslop.hpp"
#include "Assertion.hpp"
#include "ChunkView.hpp"


struct ChunkStore{
public:
    // Default construct to be CHUNK_SIZE
    ChunkStore(): buf(CHUNK_SIZE) {}
    ChunkStore(const ChunkStore&) = default;
    ChunkStore(ChunkStore&&) = default;
    ChunkStore& operator=(const ChunkStore&) = default;
    ChunkStore& operator=(ChunkStore&&) = default;
    ~ChunkStore() = default;

    // Construct from view
    explicit ChunkStore(ConstChunkView src): buf(src.data_handle(), src.data_handle()+CHUNK_SIZE) {}

private:
    inline decltype(auto) span(this auto& self){
        return std::mdspan(self.buf.data(), CHUNK_XWIDTH, CHUNK_HEIGHT, CHUNK_ZWIDTH);
    }
public:
    decltype(auto) at(this auto& self, ChunkBlockPos p){
        self.bounds_check(p.x,p.y,p.z);
        return self.span()[p.x,p.y,p.z];
    }
    decltype(auto) at(this auto& self, i32 x, i32 y, i32 z){
        self.bounds_check(x,y,z);
        return self.span()[x,y,z];
    }
    decltype(auto) operator[](this auto& self, i32 x, i32 y, i32 z){
        self.bounds_check(x,y,z);
        return self.span()[x,y,z];
    }
    auto empty() const noexcept{
        return buf.empty();
    }
    // Obtain a non owning ChunkView from this chunk.
    auto view (){
        return ChunkView{span()};
    }

    // Obtain a non owning, ConstChunkView from this chunk.
    auto view () const{
        return ConstChunkView{span()};
    }

    ChunkStore clone() const {
        return ChunkStore{view()};
    }

    auto begin(){
        return buf.begin();
    }
    auto end(){
        return buf.end();
    }
    //


    // HACK: required stuff for MapLike<> so we dont upset my AT() wrapper
    using key_type = ChunkBlockPos;
    using mapped_type = Block;
    constexpr std::size_t size()const noexcept{ return CHUNK_SIZE;}
    constexpr bool contains(ChunkBlockPos p)const noexcept{
        return  0 <= p.x && p.x < Chunk_Extents.x && 
                0 <= p.y && p.y < Chunk_Extents.y && 
                0 <= p.z && p.z < Chunk_Extents.z ;
    }
    std::vector<Block> buf={};

private:
    void bounds_check(i32 cx, i32 cy, i32 cz) const {
        if (
            cx < 0 || cx >= CHUNK_XWIDTH ||
            cy < 0 || cy >= CHUNK_HEIGHT ||
            cz < 0 || cz >= CHUNK_ZWIDTH
        ){
            throw std::out_of_range("Outside of local chunk bounds");
        }
    }
};

static_assert(!array_like<ChunkStore>);
static_assert(map_like<ChunkStore>);

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
    std::size_t size()const noexcept{
        if (slice_type == SliceType::X){
             return CHUNK_HEIGHT * CHUNK_ZWIDTH;
        }else if (slice_type == SliceType::Y){
             return CHUNK_XWIDTH * CHUNK_ZWIDTH;
        }else{
             return CHUNK_XWIDTH * CHUNK_HEIGHT;
        }
    }

    constexpr bool contains(ChunkBlockPos p)const noexcept{
        if (slice_type == SliceType::X){
            return  
                    p.x == locked_axis_val && 
                    0 <= p.y && p.y < Chunk_Extents.y && 
                    0 <= p.z && p.z < Chunk_Extents.z ;
        }else if (slice_type == SliceType::Y){
            return  
                    0 <= p.x && p.x < Chunk_Extents.x && 
                    p.y == locked_axis_val && 
                    0 <= p.z && p.z < Chunk_Extents.z ;
        }else{
            return  
                    0 <= p.x && p.x < Chunk_Extents.x && 
                    0 <= p.y && p.y < Chunk_Extents.y &&
                    p.z == locked_axis_val;
        }
    }

    SliceType slice_type;
    inline auto data(this auto& self){
        return self.buf.data(); 
    }


    inline decltype(auto) at(this auto& self, i32 cx, i32 cy, i32 cz) {
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
    inline decltype(auto) at(this auto& self, ChunkBlockPos p) {
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
    inline decltype(auto) span2d(this auto& self){
        if (self.slice_type == SliceType::X){
            return std::mdspan(self.data(), CHUNK_HEIGHT, CHUNK_ZWIDTH);
        }else if (self.slice_type == SliceType::Y){
            return std::mdspan(self.data(), CHUNK_XWIDTH, CHUNK_ZWIDTH);
        }else{
            return std::mdspan(self.data(), CHUNK_XWIDTH, CHUNK_HEIGHT);
        }
    }
    // eg: if an SliceType::X, a==y, b==z
    inline decltype(auto) at_2d(this auto& self, i32 a, i32 b) {
        return self.span2d()[a,b];
    }
};

