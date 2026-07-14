#pragma once 

#include "Block.hpp"

#include "ChunkInvariants.hpp"
#include "CoordIteration.hpp"
#include "CoordTypes.hpp"
#include "PendingBlockWrites.hpp"
#include "cppslop.hpp"
#include <mdspan>
#include "Assertion.hpp"
struct ChunkSpan{
    public:
    ChunkSpan(Block* _data):
        _span(_data){}
    ~ChunkSpan() = default;
    bool tryWrite(PendingBlockWrite write);
    using Extents = std::extents<i64, CHUNK_XWIDTH,CHUNK_HEIGHT,CHUNK_ZWIDTH>;
        std::mdspan<Block, Extents>_span;

    auto span(this auto& self){
        return self._span;
    }
    inline decltype(auto) at(this auto& self, i16 x, i16 y, i16 z) {
        return self.span()[x,y,z];
    }
    inline decltype(auto) at(this auto& self, ChunkBlockPos p) {
        assert(p.x>=0);
        assert(p.x<Chunk_Extents.x);

        assert(p.y>=0);
        assert(p.y<Chunk_Extents.y);

        assert(p.z>=0);
        assert(p.z<Chunk_Extents.z);
        return self.span()[p.x,p.y,p.z];
    }

    inline decltype(auto) operator[](this auto& self, i16 x, i16 y, i16 z) {
        return self.span()[x,y,z];
    }
    inline decltype(auto) operator[](this auto& self, ChunkBlockPos p) {
        return self.span()[p.x,p.y,p.z];
    }
};

// [x,y,z] accessible NON OWNING VIEW of chunk block storage.
FORWARD_DECL_STRUCT(Chunk)
struct ChunkStore{
private:
    std::vector<Block> buf={};
public:
    using key_type = ChunkBlockPos;
    using mapped_type = Block;
    constexpr std::size_t size()const noexcept{ return CHUNK_SIZE;}
    constexpr bool contains(ChunkBlockPos p)const noexcept{
        return  0 <= p.x && p.x < Chunk_Extents.x && 
                0 <= p.y && p.y < Chunk_Extents.y && 
                0 <= p.z && p.z < Chunk_Extents.z ;
    }
    ChunkStore(const Chunk& chunk);
    ChunkStore(const Chunk* chunk);
    ChunkStore();
    ~ChunkStore() = default;
    inline auto data(this auto& self){
        return self.buf.data(); 
    }
    inline decltype(auto) span(this auto& self){
        return std::mdspan(self.data(), CHUNK_XWIDTH, CHUNK_HEIGHT ,CHUNK_ZWIDTH);
    }

    // TODO: consider moving these into an i3D_Array
    inline decltype(auto) operator[](this auto& self, i16 x, i16 y, i16 z) {
        return self.span()[x, y, z];
    }
    inline decltype(auto) operator[](this auto& self, ChunkBlockPos p) {
        return self.span()[p.x, p.y, p.z];
    }

    inline decltype(auto) at(this auto& self, i16 x, i16 y, i16 z) {
        return self.span()[x, y, z];
    }
    inline decltype(auto) at(this auto& self, ChunkBlockPos p) {
        return self.span()[p.x, p.y, p.z];
    }

    inline void set(this auto& self, i16 x, i16 y, i16 z, BlockType bt) {
        assert(x>=0);
        assert(x<Chunk_Extents.x);

        assert(y>=0);
        assert(y<Chunk_Extents.y);

        assert(z>=0);
        assert(z<Chunk_Extents.z);
        self.span()[x, y, z] = {bt};
    }
    constexpr void setColumn(glm::ivec3 min, i32 ymax, BlockType bt){
        static_assert(std::same_as<int,int32_t>);
        ForEachInRangeEx(min.y, ymax, [&](i32 y){
            set(min.x, y, min.z, bt);
        });
    }
    auto begin(){
        return buf.begin();
    }
    auto end(){
        return buf.end();
    }

    auto& buffer(){ return buf; }

    // implicit conversion to a ChunkSpan. 
    operator ChunkSpan (){
        return {data()};
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
struct ChunkSlice2D{
private:
    std::vector<Block> buf={};
    i32 locked_axis_val{};
public:
    ChunkSlice2D(Chunk* src, SliceType _slice_type, ChunkBlockPos pos1, ChunkBlockPos pos2);
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

