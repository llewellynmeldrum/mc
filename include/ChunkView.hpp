#pragma once 
#include "Block.hpp"
#include "ChunkConstants.hpp"
#include "CoordTypes.hpp"
#include "cppslop.hpp"
#include <stdexcept>

FORWARD_DECL_STRUCT(Block)
    template<typename T>
struct BasicChunkView :std::mdspan<T, ChunkExtent>{
    using std::mdspan<T,ChunkExtent>::mdspan;
    using std::mdspan<T,ChunkExtent>::operator[];

    //NOTE: Access
    decltype(auto) operator[](this auto& self, ChunkBlockPos cpos){
        return (self)[cpos.x,cpos.y,cpos.z];
    }

    decltype(auto) at(this auto& self, ChunkBlockPos cpos){
        self.bounds_check(cpos.x, cpos.y, cpos.z);
        return self[cpos.x,cpos.y,cpos.z];
    }
    decltype(auto) at(this auto& self, i32 cx, i32 cy, i32 cz){
        self.bounds_check(cx,cy,cz);
        return self[cx,cy,cz];
    }

    auto begin()const noexcept{
        return this->data_handle();
    }
    auto end()const noexcept{
        return this->data_handle() + this->mapping().required_span_size();
    }


    // NOTE: Util
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

using ChunkView = BasicChunkView<Block>;
using ConstChunkView = BasicChunkView<const Block>;
