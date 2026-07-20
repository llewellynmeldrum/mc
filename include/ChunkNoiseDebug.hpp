#pragma once 
#include "CoordTypes.hpp"
#include "CoordIteration.hpp"
#define CHUNK_NOISE_DEBUG
template<typename T>
struct PerColumnDebugStore{
private:
    std::vector<T> buf={};
public:
    using key_type = ChunkBlockPos;
    using mapped_type = T;
    PerColumnDebugStore(){
        buf.resize(ChunkInfo::XWIDTH * ChunkInfo::ZWIDTH);
    }
    ~PerColumnDebugStore() = default;
    inline auto data(this auto& self){
        return self.buf.data(); 
    }
    inline decltype(auto) span(this auto& self){
        return std::mdspan(self.data(), ChunkInfo::XWIDTH, ChunkInfo::ZWIDTH);
    }

    inline decltype(auto) operator[](this auto& self, i16 x, i16 z) {
        return self.span()[x, z];
    }

    inline void set(this auto& self, i32 x, i32 z, T bt) {
        assert(x>=0);
        assert(x<ChunkInfo::Extents3D.x);

        assert(z>=0);
        assert(z<ChunkInfo::Extents3D.z);
        self.span()[x, z] = bt;
    }

    auto begin(this auto& self){
        return self.buf.begin();
    }
    auto end(this auto& self){
        return self.buf.end();
    }
    auto buffer(this auto& self){
        return self.buf;
    }

};
