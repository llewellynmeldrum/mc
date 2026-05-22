#pragma once 

#include "Block.hpp"

#include "PendingBlockWrites.hpp"
#include "cppslop.hpp"
#include <mdspan>
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
    ChunkStore(const Chunk& chunk);
    ChunkStore();
    ~ChunkStore() = default;
    inline auto data(this auto& self){
        return self.buf.data(); 
    }
    inline decltype(auto) span(this auto& self){
        return std::mdspan(self.data(), CHUNK_XWIDTH, CHUNK_HEIGHT ,CHUNK_ZWIDTH);
    }

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

    inline auto begin(this auto& self){
        return self.buf.begin(); 
    }
    inline auto end(this auto& self){
        return self.buf.end(); 
    }
    auto& buffer(){ return buf; }

    // implicit conversion to a ChunkSpan. 
    operator ChunkSpan (){
        return {data()};
    }
};
